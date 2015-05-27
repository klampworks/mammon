#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../fourchan_parser_json.hpp"
#include "common_ut.hpp"

BOOST_AUTO_TEST_CASE(parse_threads) 
{
    const char *input_file = "threads.json";
    const char *golden_file = "threads.json.g";
    
    auto json = read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_threads(json);

    auto g = read_file_vector(golden_file);

    CHECK_VECTOR(res, g);
}

    #include "47962086.json.g.hpp"
BOOST_AUTO_TEST_CASE(parse_posts)
{
    const char *input_file = "47962086.json";
    
    auto json = read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_posts(json);

    CHECK_VECTOR(res, golden::posts_g);
}
