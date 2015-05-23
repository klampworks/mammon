#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

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

class fourchan_parser_json {
    public:
        std::vector<std::string> parse_threads(const std::string &json);
        std::vector<chan_post> parse_posts(const std::string &json);
    protected:
        boost::property_tree::ptree parse_json(const std::string &json);

        template <typename T>
        std::string sget(T &pt, const char *node);
};
