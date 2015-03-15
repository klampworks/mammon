#include "eightchan_driver.hpp"
#include "eightchan_parser.hpp"

eightchan_driver::eightchan_driver(std::vector<std::string> &&board_p) :
	chan_driver("eightchan", new eightchan_parser(), 
	std::move(board_p), "http://fullchan4jtta4sx.onion/"){}

eightchan_driver::~eightchan_driver() 
{
	delete parser;
}

void eightchan_driver::increment_page()
{
	/* Page 1 leads to a 404 page instead of the actual page 1. */
	if (page == 0)
		page = 2;
	else
		++page;
}
