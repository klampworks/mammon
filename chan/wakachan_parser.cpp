#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "wakachan_parser.hpp"
#include <vector>
#include "chan_post.hpp"

std::vector<chan_post> wakachan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	return chan_parser::parse_a_thread("table/tbody/tr/td", board, node);
}

#include <iostream>
std::vector<std::vector<chan_post>> wakachan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{
	pugi::xml_document doc;
	doc.load(xml.c_str());
	
	const char *xpath = "//form[@id='delform']";

	//Wakacahn does not wrap each thread in a <div>.
	//Instead they separate each tread with a <hr>
	auto subtree = doc.select_single_node(xpath);

	std::vector<std::vector<chan_post>> threads;

	auto st = subtree.node().begin();
	auto en = subtree.node().end();

	threads.push_back(std::vector<chan_post>());
	unsigned index = 0;

	for (;st != en; st++) {

		//std::cout << st->name() << std::endl;
		if (!strcmp(st->name(), "table")) {

			auto node = st->select_single_node("tbody/tr/td[@class='reply']").node();

			if (threads[index].empty())
				threads[index].push_back(chan_parser::parse_post(board, node));
			else 
				threads[index].push_back(
					chan_parser::parse_post(board, node, 
						threads[index].front().thread_id));

		} else if (!strcmp(st->name(), "hr")) {

			threads.push_back(std::vector<chan_post>());
			index++;	
		}
			

	}

	return threads;
	//return chan_parser::parse_threads( board, xml);
}

bool wakachan_parser::final_page(const std::string &xml) 
{
	return chan_parser::final_page("(//form[@method='get' and @action])[last()]", xml);
}
