#ifndef BASE_DB_HPP
#define BASE_DB_HPP

#include <string>
#include <vector>
namespace base_db {

	void init();
	void init_table(const char *table_name, const std::vector<std::string> &columns);
	std::string lookup_single_value(
		const std::string&, const std::vector<std::string>&);

	std::string lookup_single_value(
		const std::string&, const std::string&);
}
#endif
