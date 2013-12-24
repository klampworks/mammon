#include "base_db.hpp"

namespace base_db {

//Returns true if the table name exists along with all its required columns.
bool check_table(const char *table_name, const std::vector<std::string> &columns) {

	sqlite3_stmt *stmt;

	std::string statement =  "PRAGMA table_info(" + table_name + ")";

	int result =  sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0); 

	if (result != SQLITE_OK)
		return false;
	
	unsigned int cols = sqlite3_column_count(stmt);

	if (cols < columns.size())
		return false;
	
	while (sqlite3_step(stmt) == SQLITE_ROW) {

		//this is important to avoid segfaults (assigning a null char* to a std::string)
		std::string tmp("");
		char *tmp_ptr =(char*)sqlite3_column_text(stmt, 1);

		if (tmp_ptr)
			tmp = tmp_ptr;
		else
			std::cout << "sf";

		bool flag = false;

		for (unsigned j = 0; j < columns.size(); j++) {

			if (tmp.compare(columns[j]) == 0) {
		
				flag = true;
				break;
			}	
		}

		if (!flag)
			return false;

	}

	return true;

}

} //namespace
