#ifndef CHAN_PARSER_HPP
    #define CHAN_PARSER_HPP

    #include "../base_parser.hpp"

    struct chan_parser : public base_parser {

    	std::vector<std::string> parse_thread_ids();
    	std::string parse_postid(pugi::xml_node &&node);
	std::string parse_post_text(pugi::xml_node &&node);
    	std::string parse_post_img(pugi::xml_node &&node);
	void parse_posts(const char *filename);
    	std::string flatten(pugi::xml_node &&quote);

    };
#endif
