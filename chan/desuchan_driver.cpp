#include "desuchan_driver.hpp"
#include "desuchan_parser.hpp"

desuchan_driver::desuchan_driver(std::vector<std::string> &&board_p) :
	chan_driver("desuchan", new desuchan_parser(), std::move(board_p)){}

desuchan_driver::~desuchan_driver() 
{
	delete parser;
}
