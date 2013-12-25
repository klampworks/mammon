#include "chan_parser.hpp"
#include <iostream>

std::vector<std::string> convert_id_to_url(const std::vector<std::string> &ids) {

	const char *str_url = "http://desuchan.net/tech/res/";
	const char *end_url = ".html";

	std::vector<std::string> ret;

	for (const auto &id : ids)
		ret.push_back(str_url + id.substr(1) + end_url);

	return std::move(ret);
}

int main() {

	chan_parser p;
	p.parse_posts("input_2.html");

	return 1;
}
