#ifndef PARSER_COMMON_HPP
#define PARSER_COMMON_HPP
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open(filename);

	assert(ifs.good());

	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	ifs.close();
	return std::move(xml);
}

void print_post(const chan_post &r) 
{
	if (r.board.empty())
		return;

	std::cout << r.board << std::endl;
	std::cout << r.thread_id << std::endl;
	std::cout << r.post_id << std::endl;
	std::cout << r.img << std::endl;
	std::cout << r.img_url << std::endl;
	std::cout << r.content << std::endl;
}

void print_posts(const std::vector<chan_post> &thread)
{
	for (const auto &r : thread)
		print_post(r);
}

void print_threads(const std::vector<std::vector<chan_post>> &threads)
{
	for (const auto &thread : threads) {
		std::cout << "\n##############################\n" << std::endl;
		print_posts(thread);
	}
}

bool parse_list_page(
	const char *filename,
	chan_parser p,
	size_t expected_size)
{
	std::string xml = dump_file(filename);

	auto ret = p.parse_thread("test", xml);

	return ret.size() == expected_size;
}

bool parse_final_page(
	const char *filename,
	chan_parser p)
{
	std::string xml = dump_file(filename);
	return p.final_page(xml);
}
