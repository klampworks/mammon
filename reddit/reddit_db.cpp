#include "../base_db.hpp"
#include "reddit_db.hpp"

namespace reddit_db {

void init() {

	base_db::init();
}

void init_table(const char *table_name) {

	std::vector<std::string> columns({
		"url",
	});

	base_db::init_table(table_name, columns);
}

bool url_exists(
	const char *table_name, 
	const std::string &url)
{
	std::string statment("select ROWID from " + 
		std::string(table_name) + " where url=?");

	const std::vector<std::string> values({url});
	std::string res = base_db::lookup_single_value(statment, values);

	return !res.empty();
}

//Returns true if the post already exists.
bool insert_url(const char *table_name, const std::string &url)
{
	if (url_exists(table_name, url))
		return true;

	base_db::insert_row(table_name, std::vector<std::string>({url}));
	return false;
}

}
