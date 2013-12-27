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

		//Interate through the posts backwards.
		for (int i = thread.size() - 1; i != -1; i--) {
			

		}

	}
}
