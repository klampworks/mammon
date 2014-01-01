#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "desuchan_parser.hpp"
#include <vector>
#include "chan_post.hpp"

std::vector<chan_post> desuchan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	return chan_parser::parse_a_thread("table/tbody/tr/td", board, node);
}

std::vector<std::vector<chan_post>> desuchan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{
	return chan_parser::parse_threads("//form/div[@id and not(@style)]", board, xml);
}

bool desuchan_parser::final_page(const std::string &xml) 
{
	return chan_parser::final_page("(//form[@method='get' and @action])[last()]", xml);
}
