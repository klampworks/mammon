#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"
#include "../filesystem.hpp"

#include <iostream>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>

chan_driver::chan_driver(const char *table_name, chan_parser *p, 
	std::vector<std::string> &&boards_p, const char *url) : 
		base_driver(), boards(std::move(boards_p)) {

	this->table_name = table_name;
	this->base_url = url;
	this->parser = p;
	chan_db::init();
	chan_db::init_table(table_name);

	domain_id = kyukon::signup(30, std::bind(&chan_driver::fillup, this));
	kyukon::set_do_fillup(true, domain_id);
		
	/* Create a base directory for this class. */
	fs::create_path(std::string(table_name) + "/");
	page = 0;
	fillup();
}

unsigned board = 0;

void chan_driver::fillup() {

	if (page < 0) { 
		std::cout << "Done all pages for board " << boards[board]<< std::endl;
		
		if (++board >= boards.size()) {
			std::cout << "No more boards available." << std::endl;
			//TODO lol
			exit(0);
		} else {
			std::cout << "Moving on to board " << boards[board] << std::endl;
			page = 0;
		}
	}
	
	std::string path = create_path();
	std::string url = base_url + boards[board] + "/";

	if (page > 0) {
		url += std::to_string(page)/* + ".html"*/;
	}
	
	chan_task *t = new chan_task(domain_id, url, base_url, task::STRING, 
		std::bind(&chan_driver::process_list_page, this, std::placeholders::_1),
		boards[board]);

	t->set_priority(1);
	t->set_filepath(path);

	kyukon::add_task(t);

	increment_page();

	//Tell Kyukon we are done filling up and it's ok to call this function
	//again if the queue is empty again.
	//kyukon::set_do_fillup(true, domain_id);
}

void chan_driver::increment_page()
{
	++page;
}

/*
 * Given a task, wrap its callback to call the fillup function first.
 */
void chan_driver::mark_task(task *t)
{
	auto original_fn = t->get_callback();
	t->set_callback([this, original_fn](task *tt) {
		fillup();
		original_fn(tt);});
}

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t)) {

		dump_html("error", t);
		retry(t);
		return;	
	}

	dump_html("good", t);

	/* Parse a list of threads with a handful of the 
	 * most recent posts for each. */
	auto threads = parser->parse_threads(
		t->get_board().c_str(), t->get_data());

	std::vector<chan_post> posts_to_add;

	//Referer url for requesting links on this page.
	const std::string referer = t->get_url();

	std::vector<task*> tasks_to_add;

	for (const auto &thread : threads) {

		if (thread.empty()) {
			std::cout << "Empty thread!" << std::endl;
			continue;
		}

		/* If the final post already exists in the db then 
		 * skip this thread. */
		if (chan_db::post_exists(table_name, thread.back()))
			break;

		if (thread.size() == 1) {

			posts_to_add.push_back(thread[0]);

		} else if (!chan_db::post_exists(table_name, thread[1])) {
			//We must download the whole thread since there may be 
			//earlier posts that we do not have.

			std::string url = gen_thread_url(thread[0]);

			chan_task *t = new chan_task(domain_id, url, referer,
				task::STRING, std::bind(&chan_driver::process_thread, 
				this, std::placeholders::_1), thread[1].board);

			t->set_priority(2);
			t->set_filepath(tt->get_filepath());
	//		kyukon::add_task(t);
			tasks_to_add.push_back(t);

		} else {

			//Iterate through the remaining replies and add them.
			for (unsigned i = 3; i < (thread.size() - 1); i++)
				posts_to_add.push_back(thread[i]);
		}
	}

	if (tasks_to_add.empty()) {
		fillup();
	} else {
		mark_task(tasks_to_add.back());

		for (const auto &task_to_add : tasks_to_add)
			kyukon::add_task(task_to_add);
	}

	chan_db::insert_posts(table_name, posts_to_add);
	//posts_to_add now contains a list of posts that were new.	

	for (const auto &new_post : posts_to_add)
		grab_post_img(new_post, referer, tt->get_filepath());

	if (parser->final_page(t->get_data()))
		page = -1;

	delete t;
	kyukon::set_do_fillup(true, domain_id);
}

void chan_driver::grab_thread(
	const chan_post &post, 
	const std::string &referer,
	const std::string &filepath) 
{
	const std::string url = gen_thread_url(post);

	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&chan_driver::process_thread, this, 
			std::placeholders::_1));

	t->set_filepath(filepath);
	t->set_priority(3);
	kyukon::add_task(t);
}

void chan_driver::process_thread(task *tt) 
{
	std::cout << "Processing thread " << tt->get_url() << std::endl;
	chan_task *t = (chan_task*)tt;

	if (!check_error(t))
		goto ERROR;

{
	/* Parse the html into a list of post objects. */
	std::vector<chan_post> thread = parser->parse_thread(
		t->get_board().c_str(), t->get_data());

	std::cout << "This thread has " << thread.size() 
		<< " posts." << std::endl;

	/* TODO If this happens log why check_error failed to 
	 * 	detect 404 or 403. 
	 * 	Possible parsing issue.
	 */ 	
	if (thread.empty())
		goto ERROR;

	dump_html("good", t);

	/* Add the posts to the database and delete the existing 
	 * ones from the vector. */
	chan_db::insert_posts(table_name, thread);

	const std::string &referer = t->get_url();
	const std::string &filepath = t->get_filepath();

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer, filepath);

	delete tt;
	return;
}

ERROR:
	std::cout << "Error processing thread." << std::endl;

	dump_html("error", t);

	/* If retry decided to give up, fillup. */
	if (!retry(t))
		fillup();

	return;	
}

/* Sometimes links are relative and need extra processing. */
std::string chan_driver::mk_file_url_value(std::string url)
{
    return url;
}

std::string chan_driver::mk_file_url_relative(std::string url)
{
	return base_url + url.substr(1);
}

std::string chan_driver::mk_file_url(std::string url)
{
    return mk_file_url_value(url);
}

//TODO grab_post_files
void chan_driver::grab_post_img(
	const chan_post &post, 
	const std::string &referer,
	const std::string &filepath) 
{
	//Not all posts have images.
	for (const auto &file_url : post.file_urls) {

        auto url = mk_file_url(file_url);
        chan_task *t = new chan_task(domain_id, url, referer, task::FILE, 
            std::bind(&chan_driver::process_image, this, 
                std::placeholders::_1), post.board);

        t->set_filepath(filepath);
        t->set_priority(4);	
        kyukon::add_task(t);
    }
}

void chan_driver::process_image(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t) || !check_file_error(t)) {
		retry(t);
		return;	
	}

	delete tt;
}

/*
 * Generate a URL for a thread.
 */
std::string chan_driver::gen_thread_url(
	const chan_post &op) const
{
	assert(!op.board.empty());
	assert(!op.thread_id.empty());

	return base_url + op.board + "/res/" + op.thread_id;
}

#include <fstream>
void chan_driver::dump_html(std::string path, const chan_task *t)
{
	if (!path.empty() && !fs::create_path_m(path))
            path = "";

	std::string fp;
	size_t st = t->get_url().rfind('/');
	size_t en = std::string::npos;
	
	/* Deal with URLs that end with a backslash. */
	if (st + 1 == t->get_url().size()) {
		en = st - 1;
		st = t->get_url().rfind('/', st-1);
	}

	if (en != st) {
		fp = path + t->get_board() + "-" 
			+ t->get_url().substr(st+1, en-st);
	} else if (st == std::string::npos) {
		/* No forward slashes in this URL...*/
		fp = path + t->get_board() + "-" + t->get_url();
	} else {
		fp = "mangled_url";
	}

	fs::unique_fn(fp);

	std::ofstream ofs;
	ofs.open(fp);
	ofs << t->get_data();
	ofs.close();
}

std::string chan_driver::create_path()
{
    std::string path(table_name);
    path += "/" + boards[board] + "/";  

    if (fs::create_path(path.c_str()))
        return path;
    else
        return "";
}
