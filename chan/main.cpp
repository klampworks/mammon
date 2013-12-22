#include "../parser.hpp"

int main() {

	const char *filename = "input.html";
	const char *xpath = "";

	std::vector<std::string> res = parser::parse_path(filename, xpath);
}
