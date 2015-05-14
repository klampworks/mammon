#pragma once

#include <string>
#include <vector>

struct chan_post {
    std::string id;
    std::string sub;
    std::string com;
    std::string filename;

    void set_filename(std::string tim, std::string ext)
    {
        filename = tim + ext;
    }
};

std::vector<std::string> parse_threads(const std::string &json);
std::vector<chan_post> parse_posts(const std::string &json);
