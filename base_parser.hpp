#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class base_parser {

	std::vector<std::string> parse_path(const char*, const char*, const char*);
	std::string parse_first_path(pugi::xml_node &&node, const char *xpath, const char *att);

};

#endif
