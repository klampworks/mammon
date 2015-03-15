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

/* Sometimes links are relative and need extra processing. */
std::string shanachan_driver::mk_file_url(std::string url)
{
    return mk_file_url_relative(url);
}
