#include "wakachan_driver.hpp"
#include "wakachan_parser.hpp"

wakachan_driver::wakachan_driver(std::vector<std::string> &&boards_p) :
	chan_driver("wakachan", new wakachan_parser(), 
	std::move(boards_p), "http://wakachan.org/") {}

wakachan_driver::~wakachan_driver() 
{
	delete parser;
}
