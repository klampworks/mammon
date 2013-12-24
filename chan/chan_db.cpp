#include "../base_db.hpp"
#include "chan_db.hpp"

namespace chan_db {

void init() {

	base_db::init();
}

void init_table(const char *table_name) {

	std::vector<std::string> columns({
		"board",
		"thread_id",
		"post_id",
		"img",
		"text"
	});

	base_db::init_table(table_name, columns);
}

}//namespace
