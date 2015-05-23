
#define BOOST_TEST_MODULE base_db_ut
#include "but.hpp"
#include "../../base_db.hpp"
#include "../chan_db.hpp"
#include <iostream>
#include <fstream>
#include "test_common.hpp"

struct cdb_fix {

	std::string dbname = "test_db_skdfj.sqlite";
	std::string table_name = "testtable";
	cdb_fix()
	{
		rm(dbname);
		chan_db::init(dbname);
	}

	~cdb_fix()
	{
		rm(dbname);
	}
};

BOOST_FIXTURE_TEST_CASE(cdb_init, cdb_fix)
{
	BOOST_REQUIRE(file_exists(dbname));
	BOOST_CHECK(!base_db::check_table(table_name, chan_db::columns));
	chan_db::init_table(table_name.c_str());
	BOOST_CHECK(base_db::check_table(table_name, chan_db::columns));

}
