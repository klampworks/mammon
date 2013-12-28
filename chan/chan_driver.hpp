#ifndef CHAN_DRIVER_HPP
#define CHAN_DRIVER_HPP

#include <string>
#include "chan_post.hpp"
#include "chan_parser.hpp"

struct chan_driver {

	chan_parser parser;
	const char *table_name = "desuchan";

	void process_list_page(const std::string &xml);
	void grab_post_img(const chan_post &post);
	void grab_thread(const chan_post &post);
	void process_thread(const std::string &html);
	chan_driver();
	void fillup();
};
#endif
