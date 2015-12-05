#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../chan_db.hpp"
#include "../chan_post.hpp"

BOOST_AUTO_TEST_CASE(helllo) 
{
    remove("test_chan.sqlite");
    chan_db db("test_chan"); 
    chan_post cp("x", "666", "111", "hello wolrd", "I like puppies.", 
        {"hello.jpg", "world.jpg"});

    BOOST_CHECK(!db.post_exists(cp));
    BOOST_CHECK(db.store_post(cp));
    BOOST_CHECK(db.post_exists(cp));

    BOOST_CHECK(!db.file_exists("x", "hello.jpg"));
    BOOST_CHECK(!db.file_exists("x", "world.jpg"));

    BOOST_CHECK(db.store_file(cp.get_id(), cp.get_board(), 
        cp.get_filenames()[0]));
    BOOST_CHECK(db.file_exists("x", "hello.jpg"));
    BOOST_CHECK(!db.file_exists("x", "world.jpg"));

    BOOST_CHECK(db.store_file(cp.get_id(), cp.get_board(), 
        cp.get_filenames()[1]));
    BOOST_CHECK(db.file_exists("x", "hello.jpg"));
    BOOST_CHECK(db.file_exists("x", "world.jpg"));
}
