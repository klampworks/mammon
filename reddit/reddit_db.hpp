#ifndef REDDIT_DB_HPP
#define REDDIT_DB_HPP

#include <string>

namespace reddit_db {

	void init();
	void init_table(const char *table_name);
	bool insert_url(const char*, const std::string&);
	bool url_exists(const char *table_name, const std::string&);

}
#endif
