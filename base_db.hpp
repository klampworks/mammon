#ifndef BASE_DB_HPP
#define BASE_DB_HPP

#include <string>
#include <vector>

namespace base_db {

	void init(std::string db_file = "test.sqlite");

	void init_table(std::string table_name, std::vector<std::string> columns);

	std::string lookup_single_value(
		const std::string&, const std::vector<std::string>&);

	std::string lookup_single_value(
		const std::string&, const std::string&);

	std::string insert_row(std::string, std::vector<std::string>);
	void quote(std::string&);

	bool check_table(std::string, std::vector<std::string>);
	void format_table(std::string, std::vector<std::string>); 
}
#endif
