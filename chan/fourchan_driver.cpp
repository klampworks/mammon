#include "fourchan_driver.hpp"
#include "fourchan_parser.hpp"

fourchan_driver::fourchan_driver(std::vector<std::string> &&board_p) :
	chan_driver("fourchan", new fourchan_parser(), 
	std::move(board_p), "http://fourchan.org/"){}

fourchan_driver::~fourchan_driver() 
{
	delete parser;
}
