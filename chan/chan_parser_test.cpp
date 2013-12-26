#include "chan_parser.hpp"
#include <iostream>
#include <fstream>

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

	std::ifstream ifs;
	ifs.open("input_2.html");
	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	std::vector<chan_post> thread = p.parse_thread(xml);

	for (const auto &post : thread) {

		std::cout << "##############################" << std::endl;
		std::cout << "Board = " << post.board << std::endl;
		std::cout << "Thread = " << post.thread_id << std::endl;
		std::cout << "Id = " << post.post_id << std::endl;
		std::cout << "Image = " << post.img_url << std::endl;
		std::cout << "Image = " << post.img<< std::endl;
		std::cout << "Text = " << post.content << std::endl;
		std::cout << "##############################" << std::endl;
	}

	return 1;
}
