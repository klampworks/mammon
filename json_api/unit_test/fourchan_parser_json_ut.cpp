#define BOOST_TEST_MODULE fourchan_parser_json_ut
#include "../../chan/unit_test/but.hpp"
#include "../fourchan_parser_json.hpp"
#include <sstream>
#include <functional>

template <typename T>
T read_file(   
    const char *filename, 
    std::function<void(T&, const std::string&)> fn)
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

#define CHECK_VECTOR(a, b) \
    { \
    BOOST_REQUIRE_MESSAGE(a.size() == b.size(), \
        "Length mismatch, vector one has " << a.size() << " item(s) and " \
        "vector two has " << b.size() << " item(s)."); \
    if (a.size() == b.size()) \
        for (size_t i = 0; i < a.size(); ++i) \
            BOOST_CHECK(a[i] == b[i]); \
    }

#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp> 
void mkgold(
    const char *filename, 
    const std::vector<chan_post> &posts,
    const char *name)
{
    std::ofstream ofs;
    ofs.open(filename);
    ofs << "#pragma once\n";
    ofs << "namespace golden {\n";

    ofs << "const std::vector<chan_post> " << name << "({\n";
    for (const auto &r : posts) {
        auto t = r.com;
        boost::replace_all(t, "\"", "\\\"");

        ofs << "{\"" << r.id << "\",\n"
        << "\"" << r.sub << "\",\n"
        << "\"" << t << "\",\n"
        << "\"" << r.filename << "\"},\n";
    }
    ofs << "});\n";
    ofs << "} //namespace";
}

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

BOOST_AUTO_TEST_CASE(parse_posts)
{
    const char *input_file = "47962086.json";
   //const char *golden_file = "47962086.json.g";
   #include "47962086.json.g.hpp"
    
    auto json = read_file_string(input_file);

    fourchan_parser_json p;
    auto res = p.parse_posts(json);

//    mkgold("47962086.json.g.hpp", res, "posts_g");
    CHECK_VECTOR(res, golden::posts_g);
}
