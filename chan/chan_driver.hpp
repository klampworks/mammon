#ifndef CHAN_DRIVER_HPP
#define CHAN_DRIVER_HPP

#include <string>
#include "chan_post.hpp"
#include "chan_parser.hpp"
#include "../kyukon/task.hpp"

class chan_task : public task {

	std::string board;

	public:
	chan_task(unsigned dom, const std::string &url, const std::string &ref, task_target target,
		const std::function<void(task*)> &fn, const std::string &board_p) :
			task(dom, url, ref, target, fn), board(board_p) {}

	std::string get_board() {
		return board;
	}
};

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
