#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "../base_parser.hpp"
#include <iostream>
#include "chan_parser.hpp"
#include "chan_post.hpp"

//List page.
std::vector<std::string> chan_parser::parse_thread_ids() {

	const char *filename = "input.html";
	const char *xpath = "//form/div[@id]";
	const char *att = "id";

	std::vector<std::string> ids = base_parser::parse_path(filename, xpath, att);
	std::vector<std::string> ret;

	//Only take the odd numbered values.
	for (unsigned i = 1; i < ids.size(); i+=2) {
		ret.push_back(ids[i]);
	}
	
	return ret;
}

//Post page
std::string chan_parser::parse_postid(pugi::xml_node &&node) {

	return base_parser::parse_first_path(std::move(node), "a[@name]", "name");
}

std::string chan_parser::parse_post_text(pugi::xml_node &&node) {

	pugi::xpath_node quote = node.select_single_node("blockquote");

	//Flatten the subtree into a single string.
	return flatten(quote.node());
}

std::string chan_parser::parse_post_img(pugi::xml_node &&node) {

	return base_parser::parse_first_path(std::move(node), "span[@class='filesize']/a", "href");
}

std::string chan_parser::parse_post_img_name(pugi::xml_node &&node) {

	pugi::xpath_node img = node.select_single_node("span[@class='filesize']/a");
	return img.node().child_value();
}

//parse thread?
void chan_parser::parse_posts(const char *filename) {

	//Parse file into an AST.
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if (!result) {
		std::cout << "Error parsing document: " << result.description() << std::endl;
	}

	//Parse the original post subtree.
	pugi::xpath_node op = doc.select_single_node("//form/div[2]");

	std::string op_postid = parse_postid(op.node());
	std::string op_text = parse_post_text(op.node());
	std::string op_img_name = parse_post_img_name(op.node());
	std::string op_img = parse_post_img(op.node());

	pugi::xpath_node title_node = doc.select_single_node("//span[@class='filetitle']");
	std::string title = title_node.node().value();

	std::cout << "OP id = " << op_postid << std::endl;
	std::cout << "OP title = " << title << std::endl;
	std::cout << "OP img = " << op_img << std::endl;
	std::cout << "OP img name = " << op_img_name << std::endl;
	std::cout << "OP text = " << op_text << std::endl;
	//
	//Parse the thread responses into a node set.
	pugi::xpath_node_set posts = doc.select_nodes("//table/tbody/tr/td");

	std::vector<std::string> res;

	for (const auto &node : posts) {

		std::string post_id = parse_postid(node.node());

		//Sometimes this matches a node that is not a reply.
		if (post_id.empty())
			continue;

		std::string text = parse_post_text(node.node());
		std::string img_src = parse_post_img(node.node());
		std::string img_name = parse_post_img_name(node.node());

		std::cout << "##############################" << std::endl;
		std::cout << "Id = " << post_id << std::endl;
		std::cout << "Image = " << img_src << std::endl;
		std::cout << "Image = " << img_name << std::endl;
		std::cout << "Text = " << text << std::endl;
		std::cout << "##############################" << std::endl;
	}
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

