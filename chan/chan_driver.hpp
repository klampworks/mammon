#ifndef CHAN_DRIVER_HPP
#define CHAN_DRIVER_HPP

#include <string>
#include "chan_post.hpp"
#include "chan_parser.hpp"
#include "../kyukon/task.hpp"

struct chan_driver {

	chan_parser parser;
	const char *table_name = "desuchan";
	unsigned domain_id;

	void process_list_page(task *t);
	void grab_post_img(const chan_post &post, const std::string &referer);
	void grab_thread(const chan_post &post, const std::string &referer);
	void process_thread(task *t);
	chan_driver();
	void fillup();
};
#endif
