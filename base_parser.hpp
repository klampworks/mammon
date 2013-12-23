#ifndef PARSER_HPP
#define PARSER_HPP

#define PUGIXML_HEADER_ONLY
#include "pugi_lib/pugixml.cpp"
#include <string>
#include <vector>

struct base_parser {

	std::vector<std::string> parse_path(const char*, const char*, const char*);
	std::string parse_first_path(pugi::xml_node &&node, const char *xpath, const char *att);

};

#endif
