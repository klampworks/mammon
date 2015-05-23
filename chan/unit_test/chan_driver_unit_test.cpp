#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../chan_driver.hpp"
#include "../desuchan_driver.hpp"
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

BOOST_AUTO_TEST_CASE(create_file) 
{
	chdir("/tmp");

	/* Make sure we have write permissions here.*/
	BOOST_REQUIRE(!mkdir("mammon", 0777));

	/* Make sure we can clean everything up too. */
	BOOST_REQUIRE(!rmdir("mammon"));

	BOOST_REQUIRE(!mkdir("mammon", 0777));

	chdir("/tmp/mammon");
}

#if 0
BOOST_AUTO_TEST_CASE(constructor) 
{	
	std::vector<std::string> k;
	desuchan_driver d(std::move(k));

	struct stat stat_buf;
	int res = stat("desuchan", &stat_buf);
	BOOST_CHECK(!res);
	BOOST_CHECK(S_ISDIR(stat_buf.st_mode));
}
#endif

#if 0
BOOST_AUTO_TEST_CASE(create_dir) 
{	
	std::vector<std::string> k({"tmp"});;
	desuchan_driver d(std::move(k));
	std::string t(d.create_path());

	BOOST_CHECK(!t.empty());
	BOOST_CHECK(t == "desuchan/tmp/");
}
#endif

BOOST_AUTO_TEST_CASE(cleanup) 
{
	BOOST_CHECK(!system("rm -rf /tmp/mammon"));
}

BOOST_AUTO_TEST_CASE(create_dir) 
{	
	std::vector<std::string> k({"tmp"});;
	desuchan_driver d(std::move(k));
	std::string t(d.create_path());

	BOOST_CHECK(!t.empty());
	BOOST_CHECK(t == "desuchan/tmp/");
}
