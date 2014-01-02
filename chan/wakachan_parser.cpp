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
	pugi::xml_node div;

START:
	div = new_doc.append_child("div");
	div.append_attribute("id") = "mammon";

	for (;st != en; st++) {

		if (!strcmp(st->name(), "hr")) {
			st++;
			goto START;
		}
		div.append_copy(*st);
	}

	return chan_parser::parse_threads("div[@id='mammon']", board, new_doc);
}

bool wakachan_parser::final_page(const std::string &xml) 
{
	return chan_parser::final_page("(//form[@method='get' and @action])[last()]", xml);
}
