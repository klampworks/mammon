#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../wakachan_parser.hpp"
#include "../desuchan_parser.hpp"
#include "parser_common.hpp"
#include <vector>
#include <map>
#include <string>

struct fixture {

	std::map<const char*, chan_parser*> parsers;

	fixture()
	{
		parsers["desuchan"] = new desuchan_parser;
		parsers["wakachan"] = new wakachan_parser;
	}

	~fixture()
	{
		delete parsers["desuchan"];
		delete parsers["wakachan"];
	}
};

BOOST_FIXTURE_TEST_CASE(check_parse_a_thread, fixture) 
{
	for (const auto &parser: fixture::parsers) {
		std::cout << parser.first << " parser." << std::endl;

		print_thread_page("initial_thread_page.html", parser.second);

		std::cout << "##############################\n\n" << std::endl;
	}
}

BOOST_FIXTURE_TEST_CASE(check_parse_threads, fixture) 
{
	for (const auto &parser: fixture::parsers) {
		std::cout << parser.first << " parser." << std::endl;

		print_list_page("initial_list_page.html", parser.second);

		std::cout << "##############################\n\n" << std::endl;
	}
}

BOOST_FIXTURE_TEST_CASE(check_not_final_page, fixture)
{
	for (const auto &parser: fixture::parsers) {
		std::cout << parser.first << " parser." << std::endl;

		BOOST_CHECK(
			!parse_final_page("initial_list_page.html", parser.second));

		std::cout << "##############################\n\n" << std::endl;
	}
}

BOOST_FIXTURE_TEST_CASE(check_final_page, fixture)
{
	for (const auto &parser: fixture::parsers) {
		std::cout << parser.first << " parser." << std::endl;

		BOOST_CHECK(
			parse_final_page("initial_final_page.html", parser.second));

		std::cout << "##############################\n\n" << std::endl;
	}
}
