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

void insert_post(
	const char *table_name,
	const std::string &board,
	const std::string &thread_id,
	const std::string &post_id,
	const std::string &img,
	const std::string &text)
{
	//First check to see if this post already exists.
	std::string statment("select ROWID from " + std::string(table_name) + " where "
		"board=? and thread_id=? and post_id=?");

	const std::vector<std::string> values({board, thread_id, post_id});
	std::string res = base_db::lookup_single_value(statment, values);

	if (!res.empty())
		//This post already exists.
		return;

	base_db::insert_row(table_name, std::vector<std::string>({
		//It is important that these are in the order that the columns are
		//created in the database.
		board,
		thread_id,
		post_id,
		img,
		text)});
}

}//namespace
