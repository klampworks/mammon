#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "fourchan_parser.hpp"
#include <vector>
#include "chan_post.hpp"
#include "../tidy.hpp"

std::vector<chan_post> fourchan_parser::parse_a_thread(
	const char *board, 
	const pugi::xml_node &node) 
{
	return chan_parser::parse_a_thread(
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

	std::vector<std::vector<chan_post>> ret;
	for (;st != en; ++st) {

		if (!strcmp(st->name(), "hr"))
			continue;

		ret.push_back(parse_a_thread(board, *st));	
	}

	return ret;
}

bool fourchan_parser::final_page(const std::string &input) 
{
	std::string xml = input;
	tidy::tidy(xml);

	pugi::xml_document doc;
	doc.load(xml.c_str());

	/* TODO test this. */
	const char *xpath = 
	"/html/body/div[@class='pagelist desktop']/div[@class='next']/"
	"form[@class='pageSwitcherForm' and @action='2']/input[@value='Next']";

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
	const char *id =  nn.attribute("id").value();

	/* Remove leading 'p'. */
	if (*id) ++id;

	return std::string(id);
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
	std::string url =  base_parser::parse_first_path(nn, 
		"div[@class='file']/div[@class='fileText']/a", "href");

	/* Remove the leading "//" from the url. */
	if (url.size() > 1)
		url = url.substr(2);

	return url;
}

std::string fourchan_parser::parse_post_img_name(const pugi::xml_node &node) 
{
	auto nn = parse_post_node(node);
	pugi::xpath_node res = nn.select_single_node(
		"div[@class='file']/div[@class='fileText']/a");

	return res.node().text().get();
}

