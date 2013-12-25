#include "base_parser.hpp"

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
	pugi::xml_node &&node, 
	const char *xpath, 
	const char *att) 
{

	pugi::xpath_node res = node.select_single_node(xpath);
	return res.node().attribute(att).value();
}

