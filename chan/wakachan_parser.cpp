#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "wakachan_parser.hpp"
#include <vector>
#include "chan_post.hpp"

std::vector<chan_post> wakachan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	return chan_parser::parse_a_thread("table/tbody/tr/td", board, node);
}

std::vector<std::vector<chan_post>> wakachan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{
	return chan_parser::parse_threads("//form[@id='delform']", board, xml);
}

bool wakachan_parser::final_page(const std::string &xml) 
{
	return chan_parser::final_page("(//form[@method='get' and @action])[last()]", xml);
}
