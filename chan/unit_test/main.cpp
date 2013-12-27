#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../chan_parser.hpp"
#include <fstream>
#include <string>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open("input_2.html");
	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	return std::move(xml);
}

BOOST_AUTO_TEST_CASE(final_page) {

	chan_parser p;
	BOOST_CHECK(true);
}
