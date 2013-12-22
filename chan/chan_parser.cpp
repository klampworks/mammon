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
		ret.push_back(ids[i]);
	}
	
	return ret;
}

std::vector<std::string> convert_id_to_url(const std::vector<std::string> &ids) {

	const char *str_url = "http://desuchan.net/tech/res/";
	const char *end_url = ".html";

	std::vector<std::string> ret;

	for (const auto &id : ids)
		ret.push_back(str_url + id.substr(1) + end_url);

	return std::move(ret);
}


int main(int argc, char **argv) {

	std::vector<std::string> res = parse_thread_ids();
	res = convert_id_to_url(res);

	for (const auto &a : res) {
		if (!a.empty())
			std::cout << a << std::endl;
	}
}
