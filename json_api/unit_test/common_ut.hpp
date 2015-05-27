#pragma once

#include <string>
#include <vector>

namespace common_ut {
    std::string read_file_string(const char *filename);
    std::vector<std::string> read_file_vector(const char *filename);
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

