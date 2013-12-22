#include "../parser.hpp"
#include <iostream>

std::vector<std::string> parse_thread_ids() {

	const char *filename = "input.html";
	const char *xpath = "//form/div[@id]";
	const char *att = "id";

	std::vector<std::string> ids =  parser::parse_path(filename, xpath, att);
	std::vector<std::string> ret;

	//Only take the odd numbered values.
	for (unsigned i = 1; i < ids.size(); i+=2) {
		ret.push_back(i);
	}

	return ret;

}

int main(int argc, char **argv) {

	std::vector<std::string> res = parse_thread_ids();

	for (const auto &a : res) {
		if (!a.empty())
			std::cout << a << std::endl;
	}
}
