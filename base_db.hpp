#ifndef BASE_DB_HPP
#define BASE_DB_HPP

namespace base_db {

	void init();
	void init_table(const char *table_name, const std::vector<std::string> &columns);
}
#endif
