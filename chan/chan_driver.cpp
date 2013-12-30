#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"

#include <iostream>
#include <cassert>

chan_driver::chan_driver() {

	chan_db::init();
	domain_id = kyukon::signup(5, std::bind(&chan_driver::fillup, this));
	kyukon::set_do_fillup(true, domain_id);
		
}

std::string board = "tech";
int page = 0;

void chan_driver::fillup() {

	if (page > 0) { 
		std::cout << "Done all pages." << std::endl;
	}

	std::string url = "http://desuchan.net/" + board + "/";

	if (page > 0) {
		url += std::to_string(page) + ".html";
	}
	
	chan_task *t = new chan_task(domain_id, url, "", task::STRING, 
		std::bind(&chan_driver::process_list_page, this, std::placeholders::_1),
		board);

	kyukon::add_task(t);

	page++;

	kyukon::set_do_fillup(false, domain_id);
}

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(task *tt) {

	chan_task *t = (chan_task*)tt;
	
	//Get a list of threads with a handful of the most recent posts for each.
	auto threads = parser.parse_threads(t->get_board().c_str(), t->get_data());

	std::vector<chan_post> posts_to_add;

	//Referer url for requesting links on this page.
	const std::string referer = t->get_url();

	for (const auto &thread : threads) {

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

	if (parser.final_page(t->get_data()))
		page = -1;
}

void chan_driver::grab_thread(const chan_post &post, const std::string &referer) {

	const std::string &board = post.board;
	const std::string &thread_id = post.thread_id;

	const std::string url = "http://desuchan.net/" + board + "/res/" + thread_id + ".html"; 


	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&chan_driver::process_thread, this, std::placeholders::_1));

	kyukon::add_task(t);
	
}

void chan_driver::process_thread(task *tt) {

	chan_task *t = (chan_task*)tt;

	//Parse the html into a list of post objects.
	std::vector<chan_post> thread = parser.parse_thread(t->get_board().c_str(), t->get_data());

	//Add the posts tot the database and delete the existing ones from the vector.
	chan_db::insert_posts(table_name, thread);

	const std::string &referer = t->get_url();

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer);
}

void chan_driver::grab_post_img(const chan_post &post, const std::string &referer) {

	//Not all posts have images.
	if (post.img_url.empty())
		return;

	chan_task *t = new chan_task(domain_id, post.img_url, referer, 
		task::FILE, nullptr, post.board);

	kyukon::add_task(t);
}
