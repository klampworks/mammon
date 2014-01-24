#include "shanachan_driver.hpp"
#include "wakachan_parser.hpp"
#include "../kyukon/kyukon.hpp"

shanachan_driver::shanachan_driver(std::vector<std::string> &&boards_p) :
	chan_driver("shanachan", new wakachan_parser(), 
	std::move(boards_p), "https://shanachan.org/") {}

shanachan_driver::~shanachan_driver() 
{
	delete parser;
}

/* TODO Duplicate code. */
void shanachan_driver::grab_post_img(const chan_post &post, 
	const std::string &referer) 
{

	//Not all posts have images.
	if (post.img_url.empty())
		return;

	std::string url = base_url + post.img_url.substr(1);

	chan_task *t = new chan_task(domain_id, url, referer, task::FILE, 
		std::bind(&chan_driver::process_image, this, std::placeholders::_1), post.board);

	t->set_priority(4);	
	kyukon::add_task(t);
}
