#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"

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
	create_path(std::string(table_name) + "/");
	page = 0;
}

unsigned board = 0;

void chan_driver::fillup() {

	if (page < 0) { 
		std::cout << "Done all pages for board " << boards[board]<< std::endl;
		
		if (++board >= boards.size()) {
			std::cout << "No more boards available." << std::endl;
			return;
		} else {
			std::cout << "Moving on to board " << boards[board] << std::endl;
			page = 0;
		}
	}
	
	std::string path = create_path();
	std::string url = base_url + boards[board] + "/";

	if (page > 0) {
		url += std::to_string(page) + ".html";
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

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t)) {

		retry(t);
		return;	
	}

//	std::cout << t->get_data() << std::endl;
	//Get a list of threads with a handful of the most recent posts for each.
	auto threads = parser->parse_threads(t->get_board().c_str(), t->get_data());

	std::vector<chan_post> posts_to_add;

	//Referer url for requesting links on this page.
	const std::string referer = t->get_url();

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

			std::string url = base_url + thread[1].board +
				"/res/" + thread[1].thread_id + ".html";

			chan_task *t = new chan_task(domain_id, url, referer,
				task::STRING, std::bind(&chan_driver::process_thread, 
				this, std::placeholders::_1), thread[1].board);

			t->set_priority(2);
			t->set_filepath(tt->get_filepath());
			kyukon::add_task(t);

		} else {

			//Iterate through the remaining replies and add them.
			for (unsigned i = 3; i < (thread.size() - 1); i++)
				posts_to_add.push_back(thread[i]);
		}
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
	const std::string &board = post.board;
	const std::string &thread_id = post.thread_id;

	const std::string url = base_url + board + "/res/" + thread_id + ".html"; 


	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&chan_driver::process_thread, this, std::placeholders::_1));

	t->set_filepath(filepath);
	t->set_priority(3);
	kyukon::add_task(t);
	
}

void chan_driver::process_thread(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t)) {

		retry(t);
		return;	
	}

	//Parse the html into a list of post objects.
	std::vector<chan_post> thread = parser->parse_thread(t->get_board().c_str(), t->get_data());

	//Add the posts to the database and delete the existing ones from the vector.
	chan_db::insert_posts(table_name, thread);

	const std::string &referer = t->get_url();
	const std::string &filepath = t->get_filepath();

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer, filepath);

	delete tt;
}

void chan_driver::grab_post_img(
	const chan_post &post, 
	const std::string &referer,
	const std::string &filepath) 
{
	//Not all posts have images.
	if (post.img_url.empty())
		return;

	chan_task *t = new chan_task(domain_id, post.img_url, referer, task::FILE, 
		std::bind(&chan_driver::process_image, this, std::placeholders::_1), post.board);

	t->set_filepath(filepath);
	t->set_priority(4);	
	kyukon::add_task(t);
}

void chan_driver::process_image(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t) || !check_file_error(t)) {
		retry(t);
		return;	
	}

	delete tt;
}

bool chan_driver::create_path(const std::string &path)
{
	struct stat stat_buf;
	int res = stat(path.c_str(), &stat_buf);

	/* No errors from stat. */
	if (!res) {
		
		/* What we want exists and is a directory. */
		if (S_ISDIR(stat_buf.st_mode)) {
			goto good;
		}

		std::cout << "Error file exists but is not a directory." 
			<< std::endl; 
		goto bad;
	}

	/* Path does not exist, good. */
	if (errno == ENOENT) {
		if (!mkdir(path.c_str(), 0777)) {
			goto good;
		} else {
			std::cout << "Error creating directory: " << 
			errno << std::endl;
			goto bad;
		}
	}

	std::cout << "Stat error " << errno << std::endl;
	goto bad;

	good:
		return true;
	bad:
		return false;
}

std::string chan_driver::create_path()
{
	std::string path(table_name);
	path += "/" + boards[board] + "/";  

	if (create_path(path.c_str()))
		return path;
	else
		return "";
}

