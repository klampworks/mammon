#include "secchan_driver.hpp"
#include "wakachan_parser.hpp"
#include "../kyukon/kyukon.hpp"

secchan_driver::secchan_driver(std::vector<std::string> &&boards_p) :
	chan_driver("secchan", new wakachan_parser(), 
	std::move(boards_p), "http://secchan.net/") {}

secchan_driver::~secchan_driver() 
{
	delete parser;
}

/* Sometimes links are relative and need extra processing. */
std::string secchan_driver::mk_file_url(std::string url)
{
    return mk_file_url_relative(url);
}
