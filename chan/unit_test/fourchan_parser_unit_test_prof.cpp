#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../fourchan_parser.hpp"
#include "parser_common.hpp"


BOOST_AUTO_TEST_CASE(check_parse_threads) 
{
	for (int i = 0; i < 100; ++i) {
		fourchan_parser p;
		BOOST_CHECK(
			parse_list_page("c2", &p, 16));
	}
}
