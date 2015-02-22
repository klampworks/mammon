#pragma once
#include <vector>
#include "chan_post_multi.hpp"
struct eightchan_parser {
    eightchan_parser();
	std::vector<std::vector<chan_post_multi>> parse_threads(
        const char *board, const std::string &xml);

    bool final_page(const std::string &xml);
};
