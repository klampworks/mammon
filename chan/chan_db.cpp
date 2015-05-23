#include "../base_db.hpp"
#include "chan_db.hpp"
#include <iostream>
#include <algorithm> //std::remove_if

namespace chan_db {

//	std::vector<std::string>

std::vector<std::string> columns =
		std::vector<std::string>({
		"board",
		"thread_id",
		"post_id",
		"img",
		"content"
	});

void init(std::string db_file) {
	base_db::init(db_file);
}

void init_table(const char *table_name) {

	std::cout << "Initialising table " << table_name << std::endl;
	base_db::init_table(table_name, columns);
}

bool post_exists(
	std::string table_name, 
	chan_post post)
{
	base_db::quote(table_name);
	std::string statment("select ROWID from " + 
		table_name + " where "
		"board=? and thread_id=? and post_id=?");

	const std::vector<std::string> values({
		post.board, post.thread_id, post.post_id});
	std::string res = base_db::lookup_single_value(statment, values);

	return !res.empty();
}

//Inserts a vector of posts. If the post already exists it is removed from the original vector.
void insert_posts(const char *table_name, std::vector<chan_post> &posts) 
{
	if (posts.empty())
		return;

	posts.erase(std::remove_if(posts.begin(), posts.end(), 
		[table_name](const chan_post &post) {
			//If the post already exists, will return true and the post 
			//will be removed from the original vector.
			return insert_post(table_name, post);}));
}

//Returns true if the post already exists.
bool insert_post(const char *table_name, chan_post post)
{

	if (post_exists(table_name, post))
		return true;

	base_db::insert_row(table_name, std::vector<std::string>({
		//It is important that these are in the order that the columns are
		//created in the database.
		post.board,
		post.thread_id,
		post.post_id,
		chan_post::join(post.file_ids),
		post.content}));

	return false;
}

}//namespace
