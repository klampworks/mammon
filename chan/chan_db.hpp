#ifndef CHAN_DB_HPP
#define CHAN_DB_HPP

#include <string>
#include <vector>
#include "chan_post.hpp"

namespace chan_db {

	void init();
	void init_table(const char *table_name);
	bool insert_post(const char*, chan_post post);
	void insert_posts(const char*, std::vector<chan_post>&);
	bool post_exists(const char *table_name, chan_post post);

}//namespace

#endif
