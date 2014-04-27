#ifndef FOURCHAN_PARSER_HPP
#define FOURCHAN_PARSER_HPP

#include "chan_parser.hpp"

class xml_node;
class vector;
class chan_post;

struct fourchan_parser : public chan_parser {

	std::vector<chan_post> parse_a_thread(
		const char *board, 
		const pugi::xml_node &node);
	
	std::vector<std::vector<chan_post>> parse_threads(
		const char *board,
		const std::string &xml);

	bool final_page(const std::string &xml);

	std::string parse_postid(const pugi::xml_node &node);
	std::string parse_post_text(const pugi::xml_node &node);
	std::string parse_post_img(const pugi::xml_node &node);
	std::string parse_post_img_name(const pugi::xml_node &node);
	pugi::xml_node parse_post_node(const pugi::xml_node &node);
};
#endif
