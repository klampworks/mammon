#include "desuchan_driver.hpp"
#include "desuchan_parser.hpp"

desuchan_driver::desuchan_driver() :
	chan_driver("desuchan", new desuchan_parser()) {}

desuchan_driver::~desuchan_driver() 
{
	delete parser;
}
