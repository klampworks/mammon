#include "base_db.hpp"
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

namespace base_db {

sqlite3 *database;
std::string database_file;

bool check_table(const char *table_name, const std::vector<std::string> &columns);
void format_table(const char *table_name, const std::vector<std::string> &columns);

/* TODO sqlite3_step returns DONE not OK. And ROW may not be an error either.*/
#define SQL_CHK_RES \
	if (res != SQLITE_OK && res != SQLITE_DONE && res != SQLITE_ROW) { \
		std::cout << "Error " << __FILE__ << ": " << __LINE__ << " "\
		<< res << " " << sqlite3_errstr(res) << std::endl; \
		goto END; \
	}

void quote(std::string &val)
{
	val = "\"" + val + "\"";
}

void quote(std::vector<std::string> &vals)
{
	for (auto &val : vals)
		quote(val);
}

//this is important to avoid segfaults (assigning a null char* 
//to a std::string)
std::string sql_get_string(sqlite3_stmt *stmt, int col)
{
	const char *tmp_ptr = (char*)sqlite3_column_text(stmt, col);
	return tmp_ptr? std::string(tmp_ptr) : "";
}

//Initialise connection with database file.
void init(std::string db_file) 
{
	database_file = db_file;
 
	if (sqlite3_open(database_file.c_str(), &database) != SQLITE_OK) {
		std::cout << "Error opening database, " 
			<< sqlite3_errmsg(database) << std::endl;
		exit(1);
	}
}

//Check if a table exists, if not them create it.
void init_table(std::string table_name, std::vector<std::string> columns) {

	if (!check_table(table_name, columns))
		format_table(table_name, columns);
}

//Returns true if the table name exists along with all its required columns.
bool check_table(
	std::string table_name, 
	std::vector<std::string> columns) 
{
	sqlite3_stmt *stmt;
	quote(table_name);

	std::string statement =  "PRAGMA table_info(" + table_name + ");";

	int result = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0); 

	if (result != SQLITE_OK) {
		std::cout << "Prepare failed for query <" << statement 
			<< "> because " << sqlite3_errstr(result) << std::endl;
		return false;
	}
	
	bool ret = true;

	/* TODO, why divide by 2? */
	unsigned cols = (sqlite3_column_count(stmt) / 2);

	if (cols != columns.size()) {
		ret = false;
		goto END;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW) {

		std::string s = sql_get_string(stmt, 1);
		
		if (s.empty()) {
			ret = false;
			goto END;
		}

		/* TODO is this a linear search...? */
		if (std::find(columns.begin(), columns.end(), s) == columns.end()) {
			ret = false;
			goto END;
		}
	}

	END:
	sqlite3_finalize(stmt);
	return ret;
}

//Create a table with columns.
void format_table(std::string table_name, std::vector<std::string> columns) 
{
	if (columns.size() == 0) {
		std::cout << "No columns have been supplied!" << std::endl;
		return;
	}
	
	quote(table_name);
	quote(columns);

	sqlite3_stmt *stmt;
	std::string insert(columns[0] + " TEXT");
	
	for (unsigned int i = 1; i < columns.size(); i++)
		insert += ", " + columns[i] + " TEXT";
	
	std::string statement("create table " + std::string(table_name) + 
		" (" + insert + ");");
	
	int res = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0);
	SQL_CHK_RES

	res = sqlite3_step(stmt);
	SQL_CHK_RES

	END:
	sqlite3_finalize(stmt);
}	

std::string lookup_single_value(
	const std::string &statement, 
	const std::vector<std::string> &value) 
{
	sqlite3_stmt *stmt;
	std::string ret("");
		
	int res = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0);
	SQL_CHK_RES

	{
	unsigned i = 0;
	for (const auto &val : value) {
	
		res = sqlite3_bind_text(stmt, ++i, val.c_str(), val.length(), 0); 
		SQL_CHK_RES
	}
	}

	res = sqlite3_step(stmt);
	SQL_CHK_RES

	ret = sql_get_string(stmt, 0);

	END:
	sqlite3_finalize(stmt);
	return ret;
}

std::string lookup_single_value(
	const std::string &statement, 
	const std::string &value) 
{
	sqlite3_stmt *stmt;
	std::string ret("");

	int res = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0); 
	SQL_CHK_RES

	res = sqlite3_bind_text(stmt, 1, value.c_str(), value.length(), 0); 
	SQL_CHK_RES

	res = sqlite3_step(stmt);
	SQL_CHK_RES

	ret = sql_get_string(stmt, 0);

	END:
	sqlite3_finalize(stmt);
	return ret;
}

std::string insert_row(
	std::string table_name, 
	std::vector<std::string> values) 
{

	if (values.empty())
		return "";

	quote(table_name);

	std::string columns("?");
	std::string ret("");
	
	for (unsigned i = 1; i < values.size(); ++i)
		columns += ",?";

	sqlite3_stmt *stmt;
	
	std::string statement = "insert into " + table_name + 
		" values (" + columns + ")";
	
	int res = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, 0);
	SQL_CHK_RES

	{
	unsigned i = 0;
	for (const auto &val : values) {

		res = sqlite3_bind_text(stmt, ++i, val.c_str(), val.length(), 0);
		SQL_CHK_RES	
	}
	}
		
	res = sqlite3_step(stmt);
	SQL_CHK_RES


	{
	long long tmp = (long long)sqlite3_last_insert_rowid(database);

	//TODO is this the best way to long long --> std::string?
	std::stringstream ss;
	ss << tmp;
	ret = ss.str();
	}

	END:
	sqlite3_finalize(stmt);
	return ret;
}
} //namespace
