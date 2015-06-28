#ifndef PARSER_HPP
#define PARSER_HPP

#define PUGIXML_HEADER_ONLY
#include <pugixml.hpp>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <functional>

struct base_parser {

	std::vector<std::string> parse_path(const char*, const char*, const char*);
	std::string parse_first_path(const pugi::xml_node &node, const char *xpath, const char *att);

	std::vector<std::string> parse_list(
		const std::string &input,
		const boost::regex &expr);
	
	void refine_list(
		std::vector<std::string> &input,
		const std::function<bool(const std::string&)> &pred);

};
#endif
