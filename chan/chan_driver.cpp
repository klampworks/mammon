#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"

#include <iostream>
#include <cassert>

chan_driver::chan_driver(const char *table_name, chan_parser *p, 
	std::vector<std::string> &&boards_p, const char *url) : 
		base_driver(), boards(std::move(boards_p)) {

	this->table_name = table_name;
	this->base_url = url;
	this->parser = p;
	chan_db::init();
	chan_db::init_table(table_name);

	domain_id = kyukon::signup(10, std::bind(&chan_driver::fillup, this));
	kyukon::set_do_fillup(true, domain_id);
		
}

unsigned board = 0;
int page = 0;

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

	std::string url = base_url + boards[board] + "/";

	if (page > 0) {
		url += std::to_string(page) + ".html";
	}
	
	chan_task *t = new chan_task(domain_id, url, "", task::STRING, 
		std::bind(&chan_driver::process_list_page, this, std::placeholders::_1),
		boards[board]);

	t->set_priority(1);

	kyukon::add_task(t);

	page++;

	//Tell Kyukon we are done filling up and it's ok to call this function
	//again if the queue is empty again.
	kyukon::set_do_fillup(true, domain_id);
}

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(task *tt) {

	chan_task *t = (chan_task*)tt;

	if (!check_error(t)) {

		retry(t);
		return;	
	}

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

		//If the final post already exists in the db then skip this thread.
		if (chan_db::post_exists(table_name, thread.back()))
			break;

		if (thread.size() == 1) {

			posts_to_add.push_back(thread[0]);

		} else if (!chan_db::post_exists(table_name, thread[1])) {
			//We must download the whole thread since there may be 
			//earlier posts that we do not have.

			std::string url = "http://desuchan.net/" + thread[1].board +
				"/res/" + thread[1].thread_id + ".html";

			chan_task *t = new chan_task(domain_id, url, referer,
				task::STRING, std::bind(&chan_driver::process_thread, 
				this, std::placeholders::_1), thread[1].board);

			t->set_priority(2);
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
		grab_post_img(new_post, referer);

	if (parser->final_page(t->get_data()))
		page = -1;

	delete t;
}

void chan_driver::grab_thread(const chan_post &post, const std::string &referer) {

	const std::string &board = post.board;
	const std::string &thread_id = post.thread_id;

	const std::string url = "http://desuchan.net/" + board + "/res/" + thread_id + ".html"; 


	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&chan_driver::process_thread, this, std::placeholders::_1));

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

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer);

	delete tt;
}

void chan_driver::grab_post_img(const chan_post &post, const std::string &referer) {

	//Not all posts have images.
	if (post.img_url.empty())
		return;

	chan_task *t = new chan_task(domain_id, post.img_url, referer, task::FILE, 
		std::bind(&chan_driver::process_image, this, std::placeholders::_1), post.board);

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
