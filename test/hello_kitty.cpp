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

	for (pugi::xml_node img = doc.child("img"); img; img = img.next_sibling("img")) {

	    std::cout << "img " << img.attribute("src").value() << "\n";
	}
/*
	pugi::xpath_node_set bounds = doc.select_nodes("/");

	std::cout << "Bounds:";
*/

}
