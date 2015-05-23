#pragma once
#include <string>

namespace fs {

    void append_slash(std::string &path);
    bool create_path(const std::string &path);
    void unique_fn(std::string &fn);

}
