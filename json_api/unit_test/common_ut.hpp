#pragma once

#include <sstream>
#include <functional>
#include <string>
#include <vector>

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

