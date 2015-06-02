#include <boost/algorithm/string.hpp> 
#include <fstream>
#include <vector>
#include "../fourchan_parser_json.hpp"
#include "common_ut.hpp"
#include "../chan_post.hpp"

/* TODO This could be a template with a chan_post member function to print out
 * its own fields. Would need a way of printing out the class name too.
 */
void mkgold_chan_posts(
    const char *filename, 
    const std::vector<chan_post> &posts,
    const char *name)
{
    std::ofstream ofs;
    ofs.open(filename);
    ofs << "#pragma once\n";
    ofs << "namespace golden {\n";

    ofs << "const std::vector<chan_post> " << name << "({\n";
    for (const auto &r : posts) {
        auto t = r.get_com();
        boost::replace_all(t, "\"", "\\\"");

        ofs  
        << "{\"" << r.get_board() << "\",\n"
        << "\"" << r.get_thread_id() << "\",\n"
        << "\"" << r.get_id() << "\",\n"
        << "\"" << r.get_sub() << "\",\n"
        << "\"" << t << "\",\n"
        << "\"" << r.get_filename() << "\"},\n";
    }
    ofs << "});\n";
    ofs << "} //namespace";
}

void mkgold_parse_chan_posts()
{
    const std::string input_file = "47962086.json";
    auto json = common_ut::read_file_string(input_file.c_str());

    fourchan_parser_json p;
    auto res = p.parse_posts(json, chan_post("x", "666"));

    mkgold_chan_posts(std::string(input_file + ".g.hpp").c_str(), res, "posts_g");
}

int main()
{
    mkgold_parse_chan_posts();
}
