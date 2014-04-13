#include "reddit_parser.hpp"
#include "../base_driver.hpp"

struct reddit_driver : public base_driver {

	reddit_driver(std::vector<std::string> &&subreddits_p);
	void fillup();
	void process_list_page(task *tt);
	void process_image(task *t);
	bool create_path(const std::string &path);
	std::string create_path();

	reddit_parser parser;
	std::vector<std::string> subreddits;
	const char *table_name;
	const char *base_url;
	int domain_id;
};
