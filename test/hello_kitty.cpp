#define PUGIXML_HEADER_ONLY
#include "pugi_lib/pugixml.cpp"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> parse_path(const char *filename, const char *xpath) {

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	pugi::xpath_node_set imgs = doc.select_nodes(xpath);

	std::vector<std::string> res;

	for (const auto &node : imgs)
	        res.push_back(node.node().attribute("src2").value());

	return std::move(res);
}

int main() {

	const char *filename = "input.html";
	const char *xpath = "//img[@class='img_hid']";

	std::vector<std::string> res = parse_path(filename, xpath);
	
	for (const auto &s : res) {

		std::cout << s << "\n";
	}


	std::cout << std::endl;
}
