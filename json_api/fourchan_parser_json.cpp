#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

#include <iostream>

std::vector<std::string> parse_threads(const std::string &json)
{
    boost::property_tree::ptree pt;
    std::stringstream ss; ss << json;
    read_json(ss, pt);

    //for (const auto &v : pt.get_child("page")) {
    for (const auto &v : pt) {
        for (const auto &w : v.second.get_child("threads")) {
        //std::cout << w.first << std::endl;
        std::cout << w.second.get<std::string>("no") << std::endl;
        }
    }
}
