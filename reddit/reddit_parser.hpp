#ifndef REDDIT_PARSER_HPP
#define REDDIT_PARSER_HPP

#include "../base_parser.hpp"
#include <vector>
#include <string>

struct reddit_parser : base_parser {

	std::vector<std::string> parse_urls(const std::string &);
	std::vector<std::string> parse_images(const std::string &);
	std::string parse_next(const std::string &value);

};
#endif
