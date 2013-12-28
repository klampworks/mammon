#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"

chan_driver::chan_driver() {

	domain_id = kyukon::signup(5, std::bind(&chan_driver::fillup, this));
	
}

void chan_driver::fillup(){

}

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(task *t) {

	//Get a list of threads with a handful of the most recent posts for each.
	auto threads = parser.parse_threads("test_board", t->get_data());

	std::vector<chan_post> posts_to_add;

	for (const auto &thread : threads) {

		//If the final post already exists in the db then skip this thread.
		if (chan_db::post_exists(table_name, thread.back()))
			break;

		//This thread has no replies, skip.
		if (thread.size() == 1)
			break;
			
		//Check if the second post down not exist.
		if (!chan_db::post_exists(table_name, thread[2])) {
			//We must download the whole thread since there may be 
			//earlier posts that we do not have.

		} else {

			//Iterate through the remaining replies and add them.
			for (unsigned i = 3; i < (thread.size() - 1); i++)
				posts_to_add.push_back(thread[i]);
		}
	}

	chan_db::insert_posts(table_name, posts_to_add);
	//posts_to_add now contains a list of posts that were new.	
	
	//Referer url for requesting links on this page.
	const std::string referer = t->get_url();

	for (const auto &new_post : posts_to_add)
		grab_post_img(new_post, referer);
}

void chan_driver::grab_thread(const chan_post &post, const std::string &referer) {

	const std::string &board = post.board;
	const std::string &thread_id = post.thread_id;

	const std::string url = "http://desuchan.net/" + board + "/res/" + thread_id + ".html"; 


	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&chan_driver::process_thread, this, std::placeholders::_1));

	kyukon::add_task(t);
	
}

void chan_driver::process_thread(task *t) {

	//Parse the html into a list of post objects.
	std::vector<chan_post> thread = parser.parse_thread("test_board", t->get_data());

	//Add the posts tot the database and delete the existing ones from the vector.
	chan_db::insert_posts(table_name, thread);

	const std::string &referer = t->get_url();

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer);
}

void chan_driver::grab_post_img(const chan_post &post, const std::string &referer) {

	task *t = new task(domain_id, post.img_url, referer, task::FILE, nullptr);
	kyukon::add_task(t);
}
