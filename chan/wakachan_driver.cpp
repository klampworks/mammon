#include "wakachan_driver.hpp"
#include "wakachan_parser.hpp"
#include "../kyukon/kyukon.hpp"

wakachan_driver::wakachan_driver(std::vector<std::string> &&boards_p) :
	chan_driver("wakachan", new wakachan_parser(), 
	std::move(boards_p), "http://www.wakachan.org/") {}

wakachan_driver::~wakachan_driver() 
{
	delete parser;
}

/* Sometimes links are relative and need extra processing. */
std::string wakachan_driver::mk_file_url(std::string url)
{
    return mk_file_url_relative(url);
}
