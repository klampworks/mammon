#pragma once
#include <string>

namespace hash {

    void md5(const char *filename, char *hash);
    std::string md5(const char *filename);
};
