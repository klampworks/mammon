#include "base_db.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>

namespace base_db {

sqlite3 *database;
const char *database_file = "test.sqlite";

bool check_table(const char *table_name, const std::vector<std::string> &columns);
void format_table(const char *table_name, const std::vector<std::string> &columns);

//Initialise connection with database file.
void init() {
 
	sqlite3 *database;
	sqlite3_open(database_file, &database);
}

//Check if a table exists, if not them create it.
void init_table(const char *table_name, const std::vector<std::string> &columns) {

	if (!check_table(table_name, columns))
		format_table(table_name, columns);
}

//Returns true if the table name exists along with all its required columns.
bool check_table(const char *table_name, const std::vector<std::string> &columns) {

	sqlite3_stmt *stmt;

	std::string statement =  "PRAGMA table_info(" + std::string(table_name) + ")";

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
			return false;

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

//Create a table with columns.
void format_table(const char *table_name, const std::vector<std::string> &columns) {

	sqlite3_stmt *stmt;
	
	if (columns.size() == 0)
		return;
	
	std::string insert(columns[0] + " TEXT");
	
	for (unsigned int i = 1; i < columns.size(); i++)
		insert += ", " + columns[i] + " TEXT";
	
	std::string statement("create table " + std::string(table_name) + "(" + insert + ");");
	
	int result = sqlite3_prepare(database, statement.c_str(), -1, &stmt, 0);
	
	if (result != SQLITE_OK)
		return;
	
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}	

} //namespace
