#include "fourchan_parser_json.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

boost::property_tree::ptree fourchan_parser_json::parse_json(
    const std::string &json)
{
    boost::property_tree::ptree pt;
    std::stringstream ss; 
    ss << json;
    read_json(ss, pt);

    return pt;
}

/* Parse the thread ids from a threads.json page. */
std::vector<std::string> fourchan_parser_json::parse_threads(
    const std::string &json)
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
std::string fourchan_parser_json::sget(T &pt, const char *node)
{
    try {
        /* https://stackoverflow.com/a/1840318 */
        return pt.second.template get<std::string>(node);
    } catch (...) {
        return "";
    }
}

std::vector<chan_post> fourchan_parser_json::parse_posts(
    const std::string &json)
{
    auto pt(parse_json(json));

    std::vector<chan_post> ret;
    for (const auto &v : pt.get_child("posts")) {

        chan_post cp;
        cp.id = v.second.get<std::string>("no");
        cp.sub = sget(v, "sub");
        cp.com = sget(v, "com");
        cp.set_filename(sget(v, "tim"), sget(v, "ext"));
        ret.push_back(cp);
    }

    return ret;
}
