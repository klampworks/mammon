#include "../base_db.hpp"
#include "chan_db.hpp"
#include <iostream>

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
		"content"
	});

	std::cout << "Initialising table " << table_name << std::endl;
	base_db::init_table(table_name, columns);
}

bool post_exists(
	const char *table_name, 
	const std::string &board,
	const std::string &thread_id,
	const std::string &post_id)
{
	std::string statment("select ROWID from " + std::string(table_name) + " where "
		"board=? and thread_id=? and post_id=?");

	const std::vector<std::string> values({board, thread_id, post_id});
	std::string res = base_db::lookup_single_value(statment, values);

	return !res.empty();
}

void insert_post(const char *table_name, chan_post post)
{
	if (post_exists)
		//This post already exists.
		return;

	base_db::insert_row(table_name, std::vector<std::string>({
		//It is important that these are in the order that the columns are
		//created in the database.
		post.board,
		post.thread_id,
		post.post_id,
		post.img,
		post.content}));
}

}//namespace
