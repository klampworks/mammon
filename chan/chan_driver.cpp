#include "chan_db.hpp"
#include "chan_parser.hpp"
#include "chan_driver.hpp"

//Given the html souce, figure out which threads need crawling.
void chan_driver::process_list_page(const std::string &hmtl) {

	//Get a list of threads with a handful of the most recent posts for each.
	std::vector<std::vector<chan_post>> threads = parser.parse_threads(html);

	for (const auto &thread : threads) {

		//If the final post already exists in the db then skip this thread.
		if (chan_db::post_exists(table_name, thread.back()))
			break;

		//Check if second post exists
		//
		//if it does not it means there may be earlier replies that we don't have either
		//Download the entire thread.
		//
		//If it does exist then iterate through the posts in between and add them to
		//the data base. Start from low index to high index. 
		//Don't check thread[2] or thread.back() twice...
		//
		//Interate through the posts backwards.
		for (int i = thread.size() - 1; i != -1; i--) {
			

		}

	}
}
