#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(const std::string &hmtl) {

	//Get a list of threads with a handful of the most recent posts for each.
	std::vector<std::vector<chan_post>> threads = parser.parse_threads(html);

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
			for (int i = 3; i < (thread.size() - 1); i++)
				posts_to_get.push_back(thread[i]);
		}
	}

	chan_db::insert_posts(table_name, posts_to_add);
	//posts_to_add now contains a list of posts that were new.	
	
	for (const auto &new_post : posts_to_add) {
		//Download the image from the img_url field.
	}
}

void chan_driver::grab_thread(const chan_post &post) {

	const std::string &board = post.board;
	const std::string &thread_id = post.thread_id;

	const std::string url = "http://desuchan.net/" + board + "/res/" + thread_id + ".html"; 
	std::string ref = "";
}


void chan_driver::grab_post_img(const chan_post &post) {

}
