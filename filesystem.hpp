#pragma once
#include <string>

namespace fs {

    bool move(const std::string &from, const std::string &to);
    void append_slash(std::string &path);
    bool create_path(const std::string &path);
    void unique_fn(std::string &fn);
    bool file_exists(const std::string &path);
    std::string get_ext(const std::string fn);
    std::pair<std::string, std::string> split_ext(const std::string fn);
}
