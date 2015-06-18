#pragma once

#include <string>
class chan_post;

class chan_db {

    std::string cname, p_table, f_table;
    public:
        chan_db(const std::string &chan_name);
        bool store_post(const chan_post&);
        bool post_exists(const chan_post&);
        bool file_exists(const std::string &board, const std::string &filename);
};
