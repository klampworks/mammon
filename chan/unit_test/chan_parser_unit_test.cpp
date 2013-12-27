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

	const char *xml("<a name=\"2374\"></a>");
	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_postid(doc) == "2374");
}

BOOST_AUTO_TEST_CASE(check_text) {

	chan_parser p;

	const char *xml("<blockquote>\n"
			"<p><strong>NOTHING</strong></p><p><strong>NOTHING AT ALL</strong></p>\n"
			"</blockquote>");

	const char *expected = "NOTHING NOTHING AT ALL";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_text(doc) == expected);
}

BOOST_AUTO_TEST_CASE(check_img) {

	chan_parser p;

	const char *xml("<span class=\"filesize\">File: <a target=\"_blank\""
	" href=\"http://desuchan.net/tech/src/1356867546959.jpg\">1356867546959.jpg</a>"
	" -(<em>165.0 KiB, 636x603</em>)</span>");

	const char *expected = "http://desuchan.net/tech/src/1356867546959.jpg";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_img(doc) == expected);
}

