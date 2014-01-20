#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../wakachan_parser.hpp"
#include "parser_common.hpp"

BOOST_AUTO_TEST_CASE(check_parse_a_thread) 
{
	wakachan_parser p;
	BOOST_CHECK(
		parse_thread_page("wakachan_thread_page.html", &p, 5));
}

BOOST_AUTO_TEST_CASE(check_parse_threads) 
{
	wakachan_parser p;
	BOOST_CHECK(
		parse_list_page("wakachan_list_page.html", &p, 11));
}

BOOST_AUTO_TEST_CASE(check_not_final_page)
{
	wakachan_parser p;
	BOOST_CHECK(!parse_final_page("wakachan_list_page.html", &p));
}

BOOST_AUTO_TEST_CASE(check_final_page)
{
	wakachan_parser p;
	BOOST_CHECK(parse_final_page("wakachan_final_page.html", &p));
}
