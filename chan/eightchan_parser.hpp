#pragma once
#include <vector>
#include <string>

class chan_post;
struct eightchan_parser {
    eightchan_parser();
	std::vector<std::vector<chan_post>> parse_threads(
        const char *board, const std::string &xml);

    bool final_page(const std::string &xml);
};
