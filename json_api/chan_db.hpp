#pragma once

#include <string>
class chan_post;

class chan_db {

    std::string cname, p_table, f_table;
    chan_db(const std::string &db_file, const std::string &chan_name);
    bool store_post(const chan_post&);
    bool post_exists(const chan_post&);
}
