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


	auto st = subtree.node().begin();
	auto en = subtree.node().end();

	pugi::xml_document new_doc;
	pugi::xml_node div = new_doc.append_child("div");

	for (;st != en; st++) {

		if (!strcmp(st->name(), "hr")) {

			div = new_doc.append_child("div");
			assert(div);
		} else {

			div.append_copy(*st);
		}
	}

	auto ops = new_doc.select_nodes("div");

	std::cout << ops.size() << " divs" << std::endl;
	std::vector<std::vector<chan_post>> threads;

	//For each thread...
	for (auto op : ops)
		threads.push_back(parse_a_thread(board, op.node()));

	return threads;
	//return chan_parser::parse_threads(board, );
}

bool wakachan_parser::final_page(const std::string &xml) 
{
	return chan_parser::final_page("(//form[@method='get' and @action])[last()]", xml);
}
