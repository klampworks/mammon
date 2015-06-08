#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../kyukon/kon.hpp"
#include "../kyukon/task.hpp"

BOOST_AUTO_TEST_CASE(grab_wiki_page) 
{
	auto t = task(1, 
        "https://en.wikipedia.org/w/index.php?title=Mammon&oldid=665006469", 
        "666", task::STRING, nullptr);

    kon k;
    k.grab(&t);

    const std::string gold("<title>Mammon - Wikipedia, the free encyclopedia</title>");
    BOOST_CHECK(t.get_data().find(gold) != std::string::npos);
}

BOOST_AUTO_TEST_CASE(grab_wiki_page_tor) 
{
	auto t = task(1, 
        "https://en.wikipedia.org/w/index.php?title=Mammon&oldid=665006469", 
        "666", task::STRING, nullptr);

    kon k("127.0.0.1:9050", true);
    k.grab(&t);

    const std::string gold("<title>Mammon - Wikipedia, the free encyclopedia</title>");
    BOOST_CHECK(t.get_data().find(gold) != std::string::npos);
}
