#include "fourchan_parser_json.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

#include <iostream>

boost::property_tree::ptree parse_json(const std::string &json)
{
    boost::property_tree::ptree pt;
    std::stringstream ss; 
    ss << json;
    read_json(ss, pt);

    return pt;
}

/* Parse the thread ids from a threads.json page. */
std::vector<std::string> parse_threads(const std::string &json)
{
    auto pt(parse_json(json));

    std::vector<std::string> ret;
    for (const auto &v : pt) {
        for (const auto &w : v.second.get_child("threads")) {
            ret.push_back(w.second.get<std::string>("no"));
        }
    }

    return ret;
}

template <typename T>
std::string sget(T &pt, const char *node)
{
    try {
        return pt.second.template get<std::string>(node);
    } catch (...) {
        return "";
    }
}

std::vector<chan_post> parse_posts(const std::string &json)
{
    auto pt(parse_json(json));

    std::vector<chan_post> ret;
    for (const auto &v : pt.get_child("posts")) {
        chan_post cp;
        cp.id = v.second.get<std::string>("no");
        std::cout << sget(v, "sub") << std::endl;
        std::cout << sget(v, "com") << std::endl;
        std::cout << sget(v, "tim") << std::endl;
        std::cout << sget(v, "ext") << std::endl;
    }

    return ret;
}
