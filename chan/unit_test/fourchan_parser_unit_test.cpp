#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../fourchan_parser.hpp"
#include "parser_common.hpp"

#if 0
BOOST_AUTO_TEST_CASE(check_parse_a_thread) 
{
	fourchan_parser p;
	BOOST_CHECK(
		parse_thread_page("fourchan_thread_page.html", &p, 5));
}
#endif

BOOST_AUTO_TEST_CASE(check_parse_threads) 
{
	fourchan_parser p;
	BOOST_CHECK(
		parse_list_page("fourchan_list_page.html", &p, 16));
}

#if 0
BOOST_AUTO_TEST_CASE(check_not_final_page)
{
	fourchan_parser p;
	BOOST_CHECK(!parse_final_page("fourchan_list_page.html", &p));
}

BOOST_AUTO_TEST_CASE(check_final_page)
{
	fourchan_parser p;
	BOOST_CHECK(parse_final_page("fourchan_final_page.html", &p));
}


#endif
