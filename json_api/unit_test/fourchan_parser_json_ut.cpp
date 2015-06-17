#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../fourchan_parser_json.hpp"
#include "common_ut.hpp"
#include "../chan_post.hpp"

BOOST_AUTO_TEST_CASE(parse_threads) 
{
    const char *input_file = "threads.json";
    const char *golden_file = "threads.json.g";
    
    auto json = common_ut::read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_threads(json);

    auto g = common_ut::read_file_vector(golden_file);

    CHECK_VECTOR(res, g);
}

#include "47962086.json.g.hpp"
BOOST_AUTO_TEST_CASE(parse_posts)
{
    const char *input_file = "47962086.json";
    
    auto json = common_ut::read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_posts(json, chan_post("x", "666"));

    CHECK_VECTOR(res, golden::posts_g);
}

BOOST_AUTO_TEST_CASE(parse_invalid_posts_1)
{
    const std::string json("null");
    fourchan_parser_json p;
    auto res = p.parse_posts(json);

    std::vector<chan_post> g;
    CHECK_VECTOR(res, g);
}

BOOST_AUTO_TEST_CASE(parse_invalid_posts_2)
{
    const std::string json("");
    fourchan_parser_json p;
    auto res = p.parse_posts(json);

    std::vector<chan_post> g;
    CHECK_VECTOR(res, g);
}

BOOST_AUTO_TEST_CASE(parse_invalid_threads_1)
{
    const std::string json("null");
    fourchan_parser_json p;
    auto res = p.parse_threads(json);

    std::vector<std::string> g;
    CHECK_VECTOR(res, g);
}

BOOST_AUTO_TEST_CASE(parse_invalid_threads_2)
{
    const std::string json("");
    fourchan_parser_json p;
    auto res = p.parse_threads(json);

    std::vector<std::string> g;
    CHECK_VECTOR(res, g);
}

#include "3343928.json.g.hpp"
BOOST_AUTO_TEST_CASE(parse_multiple_files)
{
    const char *input_file = "3343928.json";
    auto json = common_ut::read_file_string(input_file);

    fourchan_parser_json p;

    auto res = p.parse_posts(json, chan_post("x", "666"));
    CHECK_VECTOR(res, golden::multi_file_posts_g);
}
