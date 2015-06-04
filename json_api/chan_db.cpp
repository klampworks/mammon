#include "chan_db.hpp"
#include "chan_post.hpp"
#include "../base_db.hpp"


chan_db::chan_db(const std::string &chan_name)
    : cname(chan_name)
{
    p_table = chan_name + "_posts";
    f_table = chan_name + "_files";

    base_db::init(chan_name + ".sqlite");
    base_db::format_table(p_table, {
        "board"
        "thread_id"
        "id"
        "sub"
        "com"});

    base_db::format_table(f_table, {
        "id"
        "filename"});
}

bool store_post(const chan_post&);
bool post_exists(const chan_post&);
