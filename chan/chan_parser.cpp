#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "../parser.hpp"
#include <iostream>

std::string flatten(pugi::xml_node &&quote);

std::vector<std::string> parse_thread_ids() {

	const char *filename = "input.html";
	const char *xpath = "//form/div[@id]";
	const char *att = "id";

	std::vector<std::string> ids =  parser::parse_path(filename, xpath, att);
	std::vector<std::string> ret;

	//Only take the odd numbered values.
	for (unsigned i = 1; i < ids.size(); i+=2) {
		ret.push_back(ids[i]);
	}
	
	return ret;
}

std::vector<std::string> convert_id_to_url(const std::vector<std::string> &ids) {

	const char *str_url = "http://desuchan.net/tech/res/";
	const char *end_url = ".html";

	std::vector<std::string> ret;

	for (const auto &id : ids)
		ret.push_back(str_url + id.substr(1) + end_url);

	return std::move(ret);
}

std::string parse_postid(pugi::xml_node &&node) {

	pugi::xpath_node id = node.select_single_node("a[@name]");
	return id.node().attribute("name").value();
}

std::string parse_post_text(pugi::xml_node &&node) {

	pugi::xpath_node quote = node.select_single_node("blockquote");

	//Flatten the subtree into a single string.
	return flatten(quote.node());
}

std::string parse_post_img(pugi::xml_node &&node) {

	pugi::xpath_node file = node.select_single_node("span[@class='filesize']/a");
	return file.node().attribute("href").value();
}

void parse_posts(const char *filename) {

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);


	pugi::xpath_node op = doc.select_single_node("//form/div[2]");
	std::string op_postid = parse_postid(op.node());
	std::string op_text = parse_post_text(op.node());
	std::string op_img = parse_post_img(op.node());

	std::cout << "OP id = " << op_postid << std::endl;
	std::cout << "OP img = " << op_img << std::endl;
	std::cout << "OP text = " << op_text << std::endl;
/*
<a name="2374"></a>
<label><input type="checkbox" name="num" value="2374" />
<span class="filetitle">Programming Thread</span>

<form id="delform"
*/
	pugi::xpath_node_set posts = doc.select_nodes("//table/tbody/tr/td");

	std::vector<std::string> res;

	for (const auto &node : posts) {

		std::string post_id = parse_postid(node.node());

		if (post_id.empty())
			continue;

		std::string text = parse_post_text(node.node());

		std::string img_src = parse_post_img(node.node());

		std::cout << "##############################" << std::endl;
		std::cout << "Id = " << post_id << std::endl;
		std::cout << "Image = " << img_src << std::endl;
		std::cout << "Text = " << text << std::endl;
		std::cout << "##############################" << std::endl;

	}
}


int main(int argc, char **argv) {

	parse_posts("input_2.html");
	return 1;
	std::vector<std::string> res = parse_thread_ids();
	res = convert_id_to_url(res);

	for (const auto &a : res) {
		if (!a.empty())
			std::cout << a << std::endl;
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

std::string flatten(pugi::xml_node &&quote) {

	my_walker k;
	quote.traverse(k);
	return std::move(k.value);
}

