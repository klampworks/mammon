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

BOOST_AUTO_TEST_CASE(check_img_name) {

	chan_parser p;

	const char *xml("<span class=\"filesize\">File: <a target=\"_blank\""
	" href=\"http://desuchan.net/tech/src/1356867546959.jpg\">1356867546959.jpg</a>"
	" -(<em>165.0 KiB, 636x603</em>)</span>");

	const char *expected = "1356867546959.jpg";

	pugi::xml_document doc;
	doc.load(xml);

	BOOST_CHECK(p.parse_post_img_name(doc) == expected);
}


BOOST_AUTO_TEST_CASE(check_parse_a_thread) {

	chan_parser p;

std::vector<chan_post> expected({
{"test", "2374", "2374", "1356806391743.png", "http://desuchan.net/tech/src/1356806391743.png",
"What are you working on?"},
{"test", "2374", "2375", "1356867546959.jpg", "http://desuchan.net/tech/src/1356867546959.jpg", "NOTHING NOTHING AT ALL"},
{"test", "2374", "2378", "", "", "A java fizzbuzz program"},
{"test", "2374", "2400", "1369856779897.jpg", "http://desuchan.net/tech/src/1369856779897.jpg", ">>2378 Tryin` to make some shit in Python. Now it`s MAL parser. http://rghost.net/46363268"},
{"test", "2374", "2404", "1372814015705.jpg", "http://desuchan.net/tech/src/1372814015705.jpg", ">>2374 nothing"},
{"test", "2374", "2411", "1376735987000.jpg", "http://desuchan.net/tech/src/1376735987000.jpg", "figuring out how to make access with vba scripting take an exported excel file, convert it to a drop down table, and email it, just at the press of a button... because the people i work with are too stupid to attach a file and send an email themselves Last edited 13/08/17(Sat)06:39."},
});

	std::string xml = dump_file("thread_page.html");

	std::vector<chan_post> ret = p.parse_thread("test", xml);

	std::cout << ret.size() << " posts parsed." << std::endl;

	for (unsigned i = 0; i < ret.size(); i++) {
		BOOST_CHECK(ret[i].thread_id == expected[i].thread_id);
		BOOST_CHECK(ret[i].post_id == expected[i].post_id);
		BOOST_CHECK(ret[i].content == expected[i].content);
		BOOST_CHECK(ret[i].img_url == expected[i].img_url);
		BOOST_CHECK(ret[i].img == expected[i].img);
	}
}

BOOST_AUTO_TEST_CASE(check_parse_threads) {

	chan_parser p;

	std::string xml = dump_file("list_page.html");

	std::vector<std::vector<chan_post>> ret = p.parse_threads("test", xml);

	for (const auto &r : ret) {

		for (const auto &a : r) {
			std::cout << a.thread_id << std::endl;
			std::cout << a.post_id << std::endl;
			std::cout << a.img << std::endl;
			std::cout << a.img_url << std::endl;
			std::cout << a.content << std::endl;
		}
		std::cout << std::endl;
	}

}
