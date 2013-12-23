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


void parse_posts(const char *filename) {

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	pugi::xpath_node_set posts = doc.select_nodes("//table/tbody/tr");

	std::vector<std::string> res;

	for (const auto &node : posts) {

		pugi::xpath_node quote = node.node().select_single_node("td/blockquote");

		//Flatten the subtree into a single string.
		std::string text = flatten(quote.node());

		std::cout << text << std::endl;

		pugi::xpath_node file = node.node().select_single_node("td/span[@class='filesize']/a");
		std::string img_src = file.node().attribute("href").value();
		std::cout << img_src << std::endl;

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

