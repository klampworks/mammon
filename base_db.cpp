#include "base_db.hpp"
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace base_db {

sqlite3 *database;
const char *database_file = "test.sqlite";

bool check_table(const char *table_name, const std::vector<std::string> &columns);
void format_table(const char *table_name, const std::vector<std::string> &columns);

//Initialise connection with database file.
void init() {
 
	if (sqlite3_open(database_file, &database) != SQLITE_OK) {
		std::cout << "Error opening database, " 
			<< sqlite3_errmsg(database) << std::endl;
		exit(1);
	}
}

//Check if a table exists, if not them create it.
void init_table(const char *table_name, const std::vector<std::string> &columns) {

	if (!check_table(table_name, columns))
		format_table(table_name, columns);
	else
		std::cout << "Table already exists." << std::endl;
}

//Returns true if the table name exists along with all its required columns.
bool check_table(
	const char *table_name, 
	const std::vector<std::string> &columns) 
{
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

	sqlite3_finalize(stmt);
	return true;
}

//Create a table with columns.
void format_table(const char *table_name, const std::vector<std::string> &columns) {

	sqlite3_stmt *stmt;
	
	if (columns.size() == 0) {
		std::cout << "No columns have been supplied!" << std::endl;
		return;
	}
	
	std::string insert(columns[0] + " TEXT");
	
	for (unsigned int i = 1; i < columns.size(); i++)
		insert += ", " + columns[i] + " TEXT";
	
	std::string statement("create table " + std::string(table_name) + "(" + insert + ");");
	
	int result = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0);
	
	if (result != SQLITE_OK) {
		std::cout << "Error, could not prepare statment " << statement << 
			"\nReason: " << sqlite3_errstr(result) << std::endl;
		return;
	}
	
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}	

std::string lookup_single_value(
	const std::string &statement, 
	const std::vector<std::string> &value) 
{
	sqlite3_stmt *stmt;
		
	if (sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0) 
		!= SQLITE_OK) {

		std::cout << "Error preparing statment." << std::endl;
		return "";
	}

	unsigned i = 0;
	for (const auto &val : value) {
	
		if (sqlite3_bind_text(stmt, ++i, val.c_str(), val.length(), 0) 
			!= SQLITE_OK) {

			std::cout << "Error binding value <" 
				<< val << ">." << std::endl;
			return "";

		}
	}

	sqlite3_step(stmt);

	char *tmp_ptr = (char*)sqlite3_column_text(stmt, 0);
	
	std::string res = tmp_ptr? std::string(tmp_ptr) : "";

	sqlite3_finalize(stmt);
	
	return res;
}

std::string lookup_single_value(
	const std::string &statement, 
	const std::string &value) 
{
	sqlite3_stmt *stmt;
		
	if (sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0 ) 
		!= SQLITE_OK) {

		std::cout << "Error preparing statment." << std::endl;
		return "";
	}

	
	if (sqlite3_bind_text(stmt, 1, value.c_str(), value.length(), 0 ) 
		!= SQLITE_OK) {

		std::cout << "Error binding statment." << std::endl;
		return "";
	}

	sqlite3_step(stmt);

	char *tmp_ptr = (char*)sqlite3_column_text(stmt, 0);
	
	std::string res = tmp_ptr? std::string(tmp_ptr) : "";

	sqlite3_finalize(stmt);
	return res;
}

std::string insert_row(const char *table_name, const std::vector<std::string> &values) {

	if (values.empty())
		return "";

	std::string columns("?");
	
	for (unsigned int i = 1; i < values.size(); i++)
		columns += ",?";

	sqlite3_stmt *stmt;
	
	std::string statement = "insert into " + std::string(table_name) + 
		" values (" + columns + ")";
	
	if (sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0) != SQLITE_OK)
		return "";

	unsigned i = 0;
	for (const auto &val : values)
		sqlite3_bind_text(stmt, ++i, val.c_str(), val.length(), 0);
		
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	long long tmp = (long long)sqlite3_last_insert_rowid(database);

	//TODO is this the best way to long long --> std::string?
	std::stringstream ss;
	ss << tmp;
	return ss.str();
}
} //namespace
