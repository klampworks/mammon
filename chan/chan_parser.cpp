#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "../base_parser.hpp"
#include "chan_parser.hpp"
#include "chan_post.hpp"

std::vector<chan_post> chan_parser::parse_a_thread(
	const char *xpath,
	const char *board, 
	const pugi::xml_node &node) 
{
	//Parse the op.
	chan_post thread = parse_post(board, node);
	std::string thread_id = thread.post_id;

	//Parse each reply (not OP)
	//auto nodes = node.select_nodes("table/tbody/tr/td");
	auto nodes = node.select_nodes(xpath);
	auto replies = parse_posts(board, thread_id, std::move(nodes));

	//TODO This is horrible.
	replies.insert(replies.begin(), thread);

	return std::move(replies);
}
 
//Given a list page, return a list of thread previews i.e.
//op + the last few replies.
std::vector<std::vector<chan_post>> chan_parser::parse_threads(
	const char *xpath,
	const char *board,
	const std::string &xml)
{
	pugi::xml_document doc;
	doc.load(xml.c_str());
	
	return parse_threads(xpath, board, doc);
}

std::vector<std::vector<chan_post>> chan_parser::parse_threads(
	const char *xpath,
	const char *board,
	const pugi::xml_document &doc)
{
	//Parse each thread <div>
	auto ops = doc.select_nodes(xpath);

	std::vector<std::vector<chan_post>> threads;

	//For each thread...
	for (auto op : ops)
		threads.push_back(parse_a_thread(board, op.node()));

	return threads;
}

bool chan_parser::final_page(const char *xpath, const std::string &xml) {

	//Looking for the "next" button that does nothing
	//i.e. there are no more pages.
	
	//TODO reparsing this is inefficiant.
	pugi::xml_document doc;
	doc.load(xml.c_str());

	auto res = doc.select_single_node(xpath);

	//If action is "none" then this is the final page, return true.
	return (res.node().attribute("action").value() == std::string("none"));
}

//Post page
std::string chan_parser::parse_postid(const pugi::xml_node &node) {

	return base_parser::parse_first_path(node, "a[@name]", "name");
}

std::string chan_parser::parse_post_text(const pugi::xml_node &node) {

	pugi::xpath_node quote = node.select_single_node("blockquote");

	//Flatten the subtree into a single string.
	return flatten(quote.node());
}

std::string chan_parser::parse_post_img(const pugi::xml_node &node) {

	return base_parser::parse_first_path(node, "span[@class='filesize']/a", "href");
}

std::string chan_parser::parse_post_img_name(const pugi::xml_node &node) {

	pugi::xpath_node img = node.select_single_node("span[@class='filesize']/a");
	return img.node().child_value();
}

//Given a subtree will return a vector of posts.
std::vector<chan_post> chan_parser::parse_posts(const char *board, 
	const std::string &thread_id, pugi::xpath_node_set &&posts) 
{

	std::vector<chan_post> ret;

	for (const auto &node : posts) {

		chan_post cp = parse_post(board, node.node(), thread_id);

		if (!cp.post_id.empty())
			ret.push_back(std::move(cp));
	}

	return std::move(ret);
}

//Returns an empty object if it cannot be parsed.
chan_post chan_parser::parse_post(const char *board, const pugi::xml_node &node, 
	const std::string &thread_id) 
{

	std::string post_id = parse_postid(node);

	if (post_id.empty())
		return chan_post();

	std::string content = parse_post_text(node);
	std::string img_src = parse_post_img(node);
	std::string img_name = parse_post_img_name(node);

	const std::string &thread = thread_id.empty()? post_id : thread_id;

	return chan_post(board, thread, std::move(post_id), 
			std::move(img_name), std::move(img_src), std::move(content));
}

std::vector<chan_post> chan_parser::parse_thread(const char *board, const std::string &xml) {

	//Reuse te parse_threads method and only return the first element.
	auto res = parse_threads(board, xml);

	if (res.size() == 1)
		return res.front();

	return std::vector<chan_post>();
}

//Pugi XML developers have been writing too much Java...
struct my_walker : public pugi::xml_tree_walker {

	std::string value;
	bool for_each(pugi::xml_node &node) {

		std::string tmp(node.value());

		if (!tmp.empty()) {

			if (!value.empty())
				value += " ";

			value += tmp;
		}

		return true;
	}
};

std::string chan_parser::flatten(pugi::xml_node &&quote) {

	my_walker k;
	quote.traverse(k);
	return std::move(k.value);
}

