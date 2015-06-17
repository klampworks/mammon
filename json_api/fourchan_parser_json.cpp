#include "fourchan_parser_json.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include "chan_post.hpp"

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
    std::vector<std::string> ret;
    boost::property_tree::ptree pt;

    try {
        pt = (parse_json(json));
    } catch (...) {
        return ret;
    }

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

template <typename T>
std::vector<std::string> fourchan_parser_json::get_filenames(T &pt)
{
    
    std::vector<std::string> ret = 
        {chan_post::mk_filename(sget(pt, "tim"), sget(pt, "ext"))};

    if (!ret.empty()) {
        try {
            auto it = pt.second.template get_child("extra_files");
            for (const auto i : it) {
                ret.push_back(chan_post::mk_filename(
                    i.second.template get<std::string>("tim"),
                    i.second.template get<std::string>("ext")));
             }
         } catch (...) {}
    }
    return ret;
}

std::vector<chan_post> fourchan_parser_json::parse_posts(
    const std::string &json,
    const chan_post proto)
{
    std::vector<chan_post> ret;
    boost::property_tree::ptree pt;

    try {
        pt = (parse_json(json));
    } catch (...) {
        return ret;
    }

    for (const auto &v : pt.get_child("posts")) {

        chan_post cp = proto.finish(
            v.second.get<std::string>("no"),
            sget(v, "sub"),
            sget(v, "com"),
            get_filenames(v));
            //{chan_post::mk_filename(sget(v, "tim"), sget(v, "ext"))});

        ret.push_back(cp);
    }

    return ret;
}
