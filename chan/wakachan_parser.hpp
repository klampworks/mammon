#ifndef WAKACHAN_PARSER_HPP
#define WAKACHAN_PARSER_HPP

#include "chan_parser.hpp"

class xml_node;
class vector;
class chan_post;

struct wakachan_parser : public chan_parser {

	std::vector<chan_post> parse_a_thread(
		const char *board, 
		const pugi::xml_node &node);
	
	std::vector<std::vector<chan_post>> parse_threads(
		const char *board,
		const std::string &xml);

	bool final_page(const std::string &xml);
};
#endif
