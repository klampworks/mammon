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
		//"//form[@id='delform']/div[@class='thread']", board, node);
		"*", board, node);
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

	//std::cout << subtree.node().begin()->attribute("id").value() 
		//<< std::endl;

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

	std::vector<std::vector<chan_post>> ret;
	for (;st != en; ++st) {

		if (!strcmp(st->name(), "hr"))
			continue;

		std::cout << st->attribute("id").value() << std::endl;
		auto a = parse_a_thread(board, *st);
		
		//std::cout << "Image name = " << a[0].img << std::endl;
	}
	return ret;

	//pugi::xml_document new_doc;
	//new_doc.append_child(*st);
	//pugi::xml_node div;

	/* Parse all divs from the subtree. */
	/* TODO Check that these are all relevant. */
//	return chan_parser::parse_threads("div", board, new_doc);
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

/* Return a subnode div which contains the rest of the post,
 * 4chan devs seem to call this postContainter. */
pugi::xml_node fourchan_parser::parse_post_node(const pugi::xml_node &node)
{
	return node.select_single_node(
		"(div[@class='post op'] | div[@class='post reply'])").node();
}

std::string fourchan_parser::parse_postid(const pugi::xml_node &node) 
{
	auto nn = parse_post_node(node);
	return nn.attribute("id").value();
}

std::string fourchan_parser::parse_post_text(const pugi::xml_node &node) 
{
	auto nn = parse_post_node(node);
	pugi::xpath_node quote = nn.select_single_node("blockquote");

	//Flatten the subtree of html nodes into a single string.
	return flatten(quote.node());
}

std::string fourchan_parser::parse_post_img(const pugi::xml_node &node) 
{
	auto nn = parse_post_node(node);
	return base_parser::parse_first_path(nn, 
		"div[@class='file']/div[@class='fileText']/a", "href");
}

std::string fourchan_parser::parse_post_img_name(const pugi::xml_node &node) 
{
	auto nn = parse_post_node(node);
	return base_parser::parse_first_path(nn, 
		"div[@class='file']/div[@class='fileText']/a", "title");
}
