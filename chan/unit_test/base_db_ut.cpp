#define BOOST_TEST_MODULE base_db_ut
#include "but.hpp"
#include "../../base_db.hpp"
#include <iostream>
#include <fstream>
#include "test_common.hpp"

struct db_fix {

	std::string dbname = "test_db_sdjfhsdjf.sqlite";
	std::string table_name = "test table";
	std::vector<std::string> columns = {
		"one", "two", "thr ee"};
	db_fix()
	{
		rm(dbname);
		base_db::init(dbname);
	}

	~db_fix()
	{
		rm(dbname);
	}
};

/* Creating a database file. */
BOOST_FIXTURE_TEST_CASE(db_init, db_fix)
{
	/* Database will be created in fixture constructor. */
	BOOST_REQUIRE(file_exists(dbname));
	/* Database will be deleted in fixture destructor. */
}

/* Table should not exist in an empty file. */
BOOST_FIXTURE_TEST_CASE(db_check_table, db_fix)
{
	BOOST_CHECK(!base_db::check_table(table_name, columns));
}

BOOST_FIXTURE_TEST_CASE(db_check_table_2, db_fix)
{
	base_db::format_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));
	columns.push_back("extra");
	BOOST_CHECK(!base_db::check_table(table_name, columns));
}

BOOST_FIXTURE_TEST_CASE(db_format_table, db_fix)
{
	base_db::format_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));
}

BOOST_FIXTURE_TEST_CASE(db_init_table, db_fix)
{
	base_db::init_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));
}

BOOST_FIXTURE_TEST_CASE(db_lookup_value, db_fix)
{
	base_db::init_table(table_name, columns);
	const std::string sql("select 'two' from '" + table_name + "' where 'one'=?;");
	auto ret = base_db::lookup_single_value(sql, "1");
	BOOST_CHECK(ret.empty());
}

BOOST_FIXTURE_TEST_CASE(db_insert_row, db_fix)
{
	base_db::init_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));

	base_db::insert_row(table_name, {"1", "2", "3"});

	const std::string sql("select two from '" + table_name + "' where one=?");
	auto ret = base_db::lookup_single_value(sql, "1");
	BOOST_CHECK(ret == "2");
}

BOOST_FIXTURE_TEST_CASE(db_lookup_value_2, db_fix)
{
	base_db::init_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));

	base_db::insert_row(table_name, {"1", "2", "3"});

	const std::string sql("select \"thr ee\" from '" + table_name + 
		"' where one=? and two=?");
	auto ret = base_db::lookup_single_value(sql, 
		std::vector<std::string>({"1", "2"}));
	BOOST_CHECK(ret == "3");
}

BOOST_FIXTURE_TEST_CASE(db_lookup_value_3, db_fix)
{
	base_db::init_table(table_name, columns);
	BOOST_CHECK(base_db::check_table(table_name, columns));

	base_db::insert_row(table_name, {"1", "2", "3"});

	const std::string sql("select two from '" + table_name + 
		"' where one=? and two=? and \"thr ee\"=?");
	auto ret = base_db::lookup_single_value(sql, 
		std::vector<std::string>({"1", "2", "3"}));
	BOOST_CHECK(ret == "2");
}
