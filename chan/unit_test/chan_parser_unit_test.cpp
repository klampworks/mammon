#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "../chan_parser.hpp"
#include <fstream>
#include <string>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open(filename);

	BOOST_REQUIRE_MESSAGE(ifs.good(), "Could not open file " << filename);

	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	ifs.close();
	return std::move(xml);
}

BOOST_AUTO_TEST_CASE(check_for_final_page) {

	chan_parser p;

	const char *first_page = "first_page.html";
	const char *final_page = "final_page.html";
	std::string xml;

	xml = dump_file(first_page);
	BOOST_CHECK(!p.final_page(xml));

	xml = dump_file(final_page);
	BOOST_CHECK(p.final_page(xml));
}

BOOST_AUTO_TEST_CASE(check_post_id) {

	chan_parser p;

	std::string xml("<a name=\"2374\"></a>");
	pugi::xml_document doc;
	doc.load(xml.c_str());

	BOOST_CHECK(p.parse_postid(doc) == "2374");
}
