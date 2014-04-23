#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "fourchan_parser.hpp"
#include <vector>
#include "chan_post.hpp"
#include "../tidy.hpp"

#include <iostream>

std::vector<chan_post> fourchan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	/* TODO test this. Is it class=thread? */
	return chan_parser::parse_a_thread(
		"//form[@id='delform']/div[@class='thread']" board, node);
}

std::vector<std::vector<chan_post>> fourchan_parser::parse_threads(
	const char *board,
	const std::string &xml)
{
	/* TODO rethink this. Actually the tidy should be elswhere anyway. */
	std::string xml2 = xml;

	/* This is required for 4chan. */
	tidy::tidy(xml2);

	pugi::xml_document doc;
	doc.load(xml2.c_str());
	
	const char *xpath = "//form[@id='delform']/div[@class='board']";

	/* Each thread is separated with a <hr> */
	auto subtree = doc.select_single_node(xpath);

	auto st = subtree.node().begin();
	auto en = subtree.node().end();

	//If we can't parse this then don't waste time with the other crap.
	/* TODO Why does this test fail? */
	#if 0
	if (st == en) {
		std::cout << "Initial xpath failed." << std::endl;
		std::cout << "##############################" << std::endl;
//		std::cout << xml << std::endl;
		std::cout << "##############################" << std::endl;
		return std::vector<std::vector<chan_post>>();
	}
	#endif

	pugi::xml_document new_doc;
	pugi::xml_node div;

	/* Parse all divs from the subtree. */
	/* TODO Check that these are all relevant. */
	return chan_parser::parse_threads("div", board, new_doc);
}

bool fourchan_parser::final_page(const std::string &input) 
{
	std::string xml = input;
	//tidy::tidy(xml);

	pugi::xml_document doc;
	doc.load(xml.c_str());

	/* TODO test this. */
	const char *xpath = "//form[@class='classSwitcherForm' and @action]/input[@value='Next']";
	return !doc.select_single_node(xpath);
}
