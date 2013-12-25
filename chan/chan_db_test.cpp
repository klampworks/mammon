#include "chan_db.hpp"
#include <string>

int main() {
	
	const char *table_name = "desuchan";
	chan_db::init();
	chan_db::init_table(table_name);

	std::string 
		thread_id("666"),
		post_id("666"),
		board("test"),
		text("This is a test."),
		img("test.jpg");

	chan_post post(board, thread_id, post_id, img, text);

	chan_db::insert_post(table_name, post);

	return 0;
}
