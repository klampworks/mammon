#include "fourchan_driver.hpp"
#include "fourchan_parser.hpp"

fourchan_driver::fourchan_driver(std::vector<std::string> &&board_p) :
	chan_driver("fourchan", new fourchan_parser(), 
	std::move(board_p), "http://boards.4chan.org/"){}

fourchan_driver::~fourchan_driver() 
{
	delete parser;
}

void fourchan_driver::increment_page()
{
	/* Page 1 leads to a 404 page instead of the actual page 1. */
	if (page == 0)
		page = 2;
	else
		++page;
}
