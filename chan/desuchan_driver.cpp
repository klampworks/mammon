#include "desuchan_driver.hpp"
#include "desuchan_parser.hpp"

desuchan_driver::desuchan_driver(std::vector<std::string> &&board_p) :
	chan_driver("desuchan", new desuchan_parser(), 
	std::move(board_p), "http://desuchan.net/"){}

desuchan_driver::~desuchan_driver() 
{
	delete parser;
}

/*
 * Generate a URL for a thread.
 *
 * Desuchan requires a .html prefix.
 */
std::string desuchan_driver::gen_thread_url(
	const chan_post &op) const
{
	return chan_driver::gen_thread_url(op) + ".html";
}
