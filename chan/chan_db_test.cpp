#include "chan_db.hpp"

int main() {
	
	chan_db::init();
	chan_db::init_table("desuchan");

	std::string 
		thread_id("666"),
		post_id("666"),
		board("test"),
		text("This is a test."),
		img("test.jpg");

	return 0;
}
