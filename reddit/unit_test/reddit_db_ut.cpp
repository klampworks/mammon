#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../reddit_db.hpp"
#include <fstream>
#include <string>
#include <iostream>

const char *tn = "reddit_ut";

/* Make sure that the database file is removed before and after each test. */
struct db_fix {
	
	const char *db_fn = "test.sqlite";

	db_fix() {
		unlink(db_fn);
	}

	~db_fix() {
		unlink(db_fn);
	}
};

BOOST_FIXTURE_TEST_CASE(init_ut, db_fix)
{
	reddit_db::init();
	reddit_db::init_table(tn);

	/* Make sure the databse file exists. */
	std::ifstream ifs;
	ifs.open(db_fn);
	BOOST_CHECK(ifs.good());
	ifs.close();

	/* Check the table exists? */
}

/* CHeck the existance of a value in an empty database. */
BOOST_FIXTURE_TEST_CASE(lookup_nonexistant, db_fix)
{
	reddit_db::init();
	reddit_db::init_table(tn);
	BOOST_CHECK(!reddit_db::url_exists(tn, "yui"));
}

/* Insert an item and check it is there. */
BOOST_FIXTURE_TEST_CASE(insert, db_fix)
{
	reddit_db::init();
	reddit_db::init_table(tn);
	reddit_db::insert_url(tn, "yui");
	BOOST_CHECK(reddit_db::url_exists(tn, "yui"));
}
