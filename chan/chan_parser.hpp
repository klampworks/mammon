#ifndef CHAN_PARSER_HPP
    #define CHAN_PARSER_HPP

    #include "../base_parser.hpp"
    #include "chan_post.hpp"

    struct chan_parser : public base_parser {

    	std::string parse_postid(const pugi::xml_node &node);
	std::string parse_post_text(const pugi::xml_node &node);
    	std::string parse_post_img(const pugi::xml_node &node);
	std::string parse_post_img_name(const pugi::xml_node &node);

	bool final_page(const srd::string &xml);

	std::vector<chan_post> parse_thread(const std::string &xml);
    	std::string flatten(pugi::xml_node &&quote);

	std::vector<chan_post> parse_posts(const char*, 
	const std::string&, pugi::xpath_node_set&&);

	std::vector<chan_post> parse_a_thread(const char *board, 
		const pugi::xml_node &node);

	std::vector<std::vector<chan_post>> parse_threads(
		const char *board,
		const std::string &xml);

	chan_post parse_post(const char *board, const pugi::xml_node &node, const std::string &thread=std::string());
    };
#endif
