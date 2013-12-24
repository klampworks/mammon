#ifndef CHAN_DB_HPP
#define CHAN_DB_HPP

#include <string>

namespace chan_db {

	void init();
	void init_table(const char *table_name);
	void insert_post(const char*, const std::string&, const std::string&,
		const std::string&, const std::string&, const std::string&);


}//namespace

#endif
