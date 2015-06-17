#pragma once

#include <string>
#include <vector>
#include "chan_parser.hpp"
#include <boost/property_tree/ptree_fwd.hpp>

/* Default argument cannot be forward declared. */
#include "chan_post.hpp"

class fourchan_parser_json : public chan_parser {
    public:
        std::vector<std::string> parse_threads(const std::string &json);
        std::vector<chan_post> parse_posts(const std::string &json, 
            const chan_post=chan_post("_", "0"));
    protected:
        boost::property_tree::ptree parse_json(const std::string &json);

        template <typename T>
        std::string sget(T &pt, const char *node);
        template <typename T>
        std::vector<std::string> get_filenames(T &pt);
};
