#include "../parser.hpp"
#include <iostream>

int main() {

	const char *filename = "input.html";
	const char *xpath = "//span[@class='reflink']/span/a";
	const char *att = "href";

	std::vector<std::string> res = parser::parse_path(filename, xpath);

	for (const auto &a : res) {
		std::cout << a << std::endl;
	}
}
