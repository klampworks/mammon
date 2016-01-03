#include "chan_db.hpp"
#include "chan_post.hpp"
#include "../base_db.hpp"

chan_db::chan_db(const std::string &chan_name)
    : cname(chan_name)
{
    p_table = chan_name + "_posts";
    f_table = chan_name + "_files";

    base_db::init(chan_name + ".sqlite");
    base_db::init_table(p_table, {
        "board",
        "thread_id",
        "id",
        "sub",
        "com"});

    base_db::format_table(f_table, {
        "id",
        "filename",
        "board",
        "md5"});
}

bool chan_db::store_post(const chan_post &cp)
{
    if (post_exists(cp))
        return true;

	base_db::insert_row(p_table, {
        cp.get_board(), 
        cp.get_thread_id(), 
        cp.get_id(), 
        cp.get_sub(), 
        cp.get_com()});

    //for (const auto &fn : cp.get_filenames())
    //   base_db::insert_row(f_table, {cp.get_id(), fn, cp.get_board()});

    return true;
}

bool chan_db::store_file(const std::string &id, const std::string &board, 
    const std::string &filename, const std::string &md5)
{
    if (file_exists(board, filename))
        return true;

    base_db::insert_row(f_table, {id, filename, board, md5});
    return true;
}

bool chan_db::post_exists(const chan_post &cp)
{
    const std::string query = 
        "select * from " + p_table + " where board=? and id=?";

    const std::string res = base_db::lookup_single_value(
        query.c_str(), std::vector<std::string>({cp.get_board(), cp.get_id()}));

    return !res.empty();
}

bool chan_db::file_exists(const std::string &board, const std::string &filename)
{
    const std::string query = 
        "select * from " + f_table + " where board=? and filename=?";

    const std::string res = base_db::lookup_single_value(
        query.c_str(), std::vector<std::string>({board, filename}));

    return !res.empty();
}

bool chan_db::file_hash_exists(const std::string &hash)
{
    const std::string query = 
        "select * from " + f_table + " md5=?";

    const std::string res = base_db::lookup_single_value(
        query.c_str(), std::vector<std::string>({hash}));

    return !res.empty();
}
