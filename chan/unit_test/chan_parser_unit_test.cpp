#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../chan_parser.hpp"
#include <fstream>
#include <string>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open(filename);

	BOOST_REQUIRE(ifs.good());

	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	ifs.close();
	return std::move(xml);
}

BOOST_AUTO_TEST_CASE(final_page) {

	chan_parser p;

	const char *first_page = "first_page.html";
	const char *final_page = "final_page.html";
	std::string xml;

	xml = dump_file(first_page);
	BOOST_CHECK(!p.final_page(xml));

	xml = dump_file(final_page);
	BOOST_CHECK(p.final_page(xml));
}
