#include "../parser.hpp"
#include <iostream>

int main() {

	const char *filename = "input.html";
	const char *xpath = "//img[@class='img_hid']";

	std::vector<std::string> res = parser::parse_path(filename, xpath);
	
	for (const auto &s : res) {

		std::cout << s << "\n";
	}


	std::cout << std::endl;
}
