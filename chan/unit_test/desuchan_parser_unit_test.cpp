#define BOOST_TEST_MODULE desu_parser
#include <boost/test/included/unit_test.hpp>
#include "../desuchan_parser.hpp"
#include "parser_common.hpp"

BOOST_AUTO_TEST_CASE(check_parse_a_thread) 
{
	desuchan_parser p;
	BOOST_CHECK(
		parse_thread_page("desuchan_thread_page.html", &p, 5));
}

BOOST_AUTO_TEST_CASE(check_parse_threads) 
{
	desuchan_parser p;
	BOOST_CHECK(
		parse_list_page("desuchan_list_page.html", &p, 10));
}

BOOST_AUTO_TEST_CASE(check_not_final_page)
{
	desuchan_parser p;
	BOOST_CHECK(!parse_final_page("desuchan_list_page.html", &p));
}

BOOST_AUTO_TEST_CASE(check_final_page)
{
	desuchan_parser p;
	BOOST_CHECK(parse_final_page("desuchan_final_page.html", &p));
}
