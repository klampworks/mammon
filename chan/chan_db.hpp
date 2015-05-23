#ifndef CHAN_DB_HPP
#define CHAN_DB_HPP

#include <string>
#include <vector>
#include "chan_post.hpp"

namespace chan_db {

	void init(std::string db_file = "test.sqlite");
	void init_table(const char *table_name);
	bool insert_post(const char*, chan_post post);
	void insert_posts(const char*, std::vector<chan_post>&);
	bool post_exists(std::string table_name, chan_post post);

	extern std::vector<std::string> columns;

}//namespace

#endif
