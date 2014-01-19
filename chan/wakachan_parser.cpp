#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "wakachan_parser.hpp"
#include <vector>
#include "chan_post.hpp"
#include "../tidy.hpp"

#include <iostream>

std::vector<chan_post> wakachan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	return chan_parser::parse_a_thread("table/tbody/tr/td", board, node);
}

std::vector<std::vector<chan_post>> wakachan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{
	/* TODO rethink this. Actually the tidy should be elswhere anyway. */
	std::string xml2 = xml;

	/* The wakachan HTML source is beyond condemned. */
	tidy::tidy(xml2);

	pugi::xml_document doc;
	doc.load(xml2.c_str());
	
	const char *xpath = "//form[@id='delform']";
	//const char *xpath = "/html/body/form[@id='delform']";

	//Wakacahn does not wrap each thread in a <div>.
	//Instead they separate each tread with a <hr>
	auto subtree = doc.select_single_node(xpath);

	std::cout << (bool)subtree << std::endl;

	auto st = subtree.node().begin();
	auto en = subtree.node().end();

	//If we can't parse this then don't waste time with the other crap.
	if (st == en) {
		std::cout << "Initial xpath failed." << std::endl;
		return std::vector<std::vector<chan_post>>();
	}

	pugi::xml_document new_doc;
	pugi::xml_node div;

START:
	div = new_doc.append_child("div");
	div.append_attribute("id") = "mammon";

	do {
		if (!strcmp(st->name(), "hr")) {
			st++;
			goto START;
		}

		div.append_copy(*st);
	} while (st++, st != en);

	return chan_parser::parse_threads("div[@id='mammon']", board, new_doc);
}

bool wakachan_parser::final_page(const std::string &input) 
{
	std::string xml = input;
	tidy::tidy(xml);

	pugi::xml_document doc;
	doc.load(xml.c_str());

	const char *xpath = "//form[@method='get' and @action]/input[@value='Next']";
	return !doc.select_single_node(xpath);
}
