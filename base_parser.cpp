#include "base_parser.hpp"
#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>

std::vector<std::string> base_parser::parse_path(
	const char *filename, 
	const char *xpath, 
	const char *att) 
{

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if (!result) {
		std::cout << "Error parsing document: " << result.description() << std::endl;
	}

	pugi::xpath_node_set imgs = doc.select_nodes(xpath);

	std::vector<std::string> res;

	for (const auto &node : imgs)
	        //res.push_back(node.node().attribute("src2").value());
	        res.push_back(node.node().attribute(att).value());

	return std::move(res);
}

std::string base_parser::parse_first_path(
	const pugi::xml_node &node, 
	const char *xpath, 
	const char *att) 
{

	pugi::xpath_node res = node.select_single_node(xpath);
	return res.node().attribute(att).value();
}

std::vector<std::string> base_parser::parse_list(
	const std::string &input,
	const boost::regex &expr)
{
	std::vector<std::string> ret;
	std::string::const_iterator st, en;
	st= input.begin();
	en = input.end();
	boost::match_results<std::string::const_iterator> what;

	while(regex_search(st, en, what, expr, boost::match_default)) {

		ret.push_back(what[1]);
		st= what[0].second;
	}

	return ret;
}

void base_parser::refine_list(
	std::vector<std::string> &input,
	const std::function<bool(const std::string&)> &pred)
{
	input.erase(std::remove_if(input.begin(), input.end(), 
		pred), input.end());
}
