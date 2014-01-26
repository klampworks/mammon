#ifndef CHAN_DRIVER_HPP
#define CHAN_DRIVER_HPP

#include <string>
#include "chan_post.hpp"
#include "chan_parser.hpp"
#include "../kyukon/task.hpp"
#include "../base_driver.hpp"

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

struct chan_driver : public base_driver {

	chan_parser *parser;
	const char *table_name;
	const char *base_url;
	unsigned domain_id;

	std::vector<std::string> boards;

	void process_list_page(task *t);
	virtual void grab_post_img(const chan_post &post, const std::string &referer);
	void grab_thread(const chan_post &post, const std::string &referer);
	void process_thread(task *t);
	void process_image(task *t);
	void fillup();
	void quit();
	std::string create_path();
	bool create_path(const std::string &path);

	protected:
		chan_driver(const char *, chan_parser*, 
			std::vector<std::string>&&, const char*);
};
#endif
