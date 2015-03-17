#pragma once
#include <string>

namespace fs {

    bool create_path_m(std::string &path);
    bool create_path(const std::string &path);
    void unique_fn(std::string &fn);

}
