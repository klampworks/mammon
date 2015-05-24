#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../fourchan_parser_json.hpp"
#include <sstream>
#include <functional>

template <typename T>
T read_file(const char *filename, std::function<void(T&, const std::string&)> fn)
{
    std::ifstream ifs;
    ifs.open(filename);

    std::string tmp;
    T out;

    while(std::getline(ifs, tmp))
        fn(out, tmp);

    ifs.close();
    return out;
}

std::string read_file_string(const char *filename)
{
    return read_file<std::string>(filename, 
        [](std::string &col, const std::string &line) {
            return col += line; });
}

std::vector<std::string> read_file_vector(const char *filename)
{
    return read_file<std::vector<std::string>>(filename,
        [](std::vector<std::string> &col, const std::string &line) {
            col.push_back(line); });
}

#include <iostream>
#include <boost/algorithm/string.hpp> // include Boost, a C++ library
void mkgold(const std::vector<chan_post> &posts)
{
    std::cout << "std::vector<chan_post>({" << std::endl;
    for (const auto &r : posts) {
        auto t = r.com;
        boost::replace_all(t, "\"", "\\\"");

        std::cout << "{\"" << r.id << "\",\n"
        << "\"" << r.sub << "\",\n"
        << "\"" << t << "\",\n"
        << "\"" << r.filename << "\"}," << std::endl;
    }
    std::cout << "});" << std::endl;

}

BOOST_AUTO_TEST_CASE(parse_threads) 
{
    const char *input_file = "threads.json";
    const char *golden_file = "threads.json.g";
    
    auto json = read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_threads(json);

    auto g = read_file_vector(golden_file);

    BOOST_REQUIRE_MESSAGE(res.size() == g.size(), 
        "Length mismatch, vector one has " << res.size() << " item(s) and "
        "vector two has " << g.size() << " item(s).");
    for (size_t i = 0; i < res.size(); ++i)
        BOOST_CHECK(res[i] == g[i]);
}

BOOST_AUTO_TEST_CASE(parse_posts)
{
    const char *input_file = "47962086.json";
   //const char *golden_file = "47962086.json.g";
   #include "47962086.json.g.hpp"
    
    auto json = read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_posts(json);

    BOOST_REQUIRE_MESSAGE(res.size() == posts_g.size(), 
        "Length mismatch, vector one has " << res.size() << " item(s) and "
        "vector two has " << posts_g.size() << " item(s).");
    for (size_t i = 0; i < res.size(); ++i)
        BOOST_CHECK(res[i] == posts_g[i]);
    
}
