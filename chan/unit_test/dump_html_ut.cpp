#define BOOST_TEST_MODULE example
//#include <boost/test/included/unit_test.hpp>
#include "but.hpp"
#include "../chan_driver.hpp"
#include <fstream>

struct dh_fix {

	chan_task *t;
	std::string board = "test";
	std::string data;
	std::string page = "000000001";
	std::string url = "http://blah.com/test/" + page;
	std::string exp_fn = board + "-" + page;
	bool got_file = false;
	std::fstream f;
	int data_len = 10;

	dh_fix()

	{
		/* Use random data since the function doesn't (and shouldn't)
		 * tell us which file it actually created. */
		FILE *fp = fopen("/dev/urandom", "r");
		char rd[data_len + 1];

		/* The uninitialised value of rd is fine too I guess. */
		if (fp)
			fread(rd, 1, data_len, fp);

		rd[data_len] = '\0';

		data = std::string(rd);

		t = new chan_task;
		t->set_board(board);
		t->set_data(data);
		t->set_url(url);
	}

	~dh_fix()
	{
		f.close();
		delete t;
		
		if (got_file)
			remove(exp_fn.c_str());
	}
};

/*
 * HTML dumped without a path shoud appear in a file in the cwd.
 * HTML inside the file should match what we sent to the function.
 */
BOOST_FIXTURE_TEST_CASE(dump, dh_fix) 
{
	chan_driver::dump_html("", t);

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	std::string out(data_len, '\0');
	f.read(&out[0], data_len);
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));
}

/*
 * The file name should not contain forward slashes, even if the URL ends
 * with one.
 */
BOOST_FIXTURE_TEST_CASE(dump_endslash, dh_fix) 
{
	t->set_url(t->get_url() + "/");
	chan_driver::dump_html("", t);

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	std::string out(data_len, '\0');
	f.read(&out[0], data_len);
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));
}

/*
 * If the URL contains a double forward slash, use alternative file name.
 */
BOOST_FIXTURE_TEST_CASE(dump_doubleslash, dh_fix) 
{
	t->set_url(t->get_url() + "//");
	exp_fn = "mangled_url";

	chan_driver::dump_html("", t);

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	std::string out(data_len, '\0');
	f.read(&out[0], data_len);
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));
}

/*
 * A URL with no slashes should be used as is.
 */
BOOST_FIXTURE_TEST_CASE(dump_noslash, dh_fix) 
{
	t->set_url("noslashes");
	exp_fn = board + "-noslashes";

	chan_driver::dump_html("", t);

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	std::string out(data_len, '\0');
	f.read(&out[0], data_len);
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));
}

BOOST_FIXTURE_TEST_CASE(dump_duplicate, dh_fix) 
{
	chan_task *tt = new chan_task;
	tt->set_board(board);
	tt->set_data("junk");
	tt->set_url(url);

	std::string exp_fn_2 = exp_fn;
	exp_fn = exp_fn + "_1";

	chan_driver::dump_html("", tt);
	chan_driver::dump_html("", t);

	remove(exp_fn_2.c_str());
	delete tt;

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	/* The data from the file should be nul terminated  anyway. */
	std::string out(data_len, '\0');
	f.read(&out[0], data_len);

	/* std::strings are not nul terminated. */
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));
}

BOOST_FIXTURE_TEST_CASE(dump_dir, dh_fix) 
{
	const char *path = "dump_test/";
	chan_driver::dump_html(path, t);
	exp_fn = path + exp_fn;

	f.open(exp_fn);
	BOOST_REQUIRE((got_file = f.good()));

	std::string out(data_len, '\0');
	f.read(&out[0], data_len);
	BOOST_CHECK(!strcmp(out.c_str(), data.c_str()));

}

