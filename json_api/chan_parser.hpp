#pragma once

#include <string>
#include <vector>

/* Default argument cannot be forward declared. */
#include "chan_post.hpp"

class chan_parser {
    public:
        virtual std::vector<std::string> parse_threads(const std::string &json)=0;
        virtual std::vector<chan_post> parse_posts(const std::string &json, 
            const chan_post=chan_post("_", "0"))=0;
};
