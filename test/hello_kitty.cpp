#define PUGIXML_HEADER_ONLY
#include "pugi_lib/pugixml.cpp"
#include <iostream>

int main() {

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("input.html");

	if (!result) {
		std::cout << "Bad parse." << std::endl;
		std::cout << "Error description: " << result.description() << "\n";
	}

	pugi::xpath_node_set imgs = doc.select_nodes("//img");

	std::cout << "Images...:";

	for (const auto &node : imgs) {
	
	        std::cout << " " << node.node().name();
	}

}
