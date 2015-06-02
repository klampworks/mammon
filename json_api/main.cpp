#include <string>
#include <fstream>
#include <iostream>
#include "fourchan_parser_json.hpp"

std::string read_file(const char *filename)
{
    std::ifstream ifs;
    ifs.open(filename);

    std::string out, tmp;

    while(std::getline(ifs, tmp))
        out += tmp;

    ifs.close();
    return out;
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        std::cout << "Enter a filename." << std::endl;
        return 1;
    }

    auto s = read_file(argv[1]);
    fourchan_parser_json p;
    auto posts = p.parse_posts(s);

    for (const auto &post : posts)
        std::cout << post.get_id() << std::endl;
    /*
    auto threads = parse_threads(s);
    for (const auto &thread : threads)
        std::cout << thread << std::endl;
    */
}
