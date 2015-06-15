#include "fourchan_proc.hpp"
#include <string>

std::string fourchan_proc::mk_textbase_url()
{
    return "http://a.4cdn.org/";
}

std::string fourchan_proc::mk_filebase_url()
{
    return "http://i.4cdn.org/";
}

std::string fourchan_proc::mk_file_url(const std::string b, const std::string filename)
{
    return mk_filebase_url() + b + "/" + filename;
}

std::string fourchan_proc::mk_thread_url(const std::string b, 
    const std::string thread_id)
{
    return mk_textbase_url() + b + "/thread/" + thread_id + ".json";
}

std::string fourchan_proc::mk_board_url(const std::string b)
{
    return mk_textbase_url() + b + "/threads.json";
}

fourchan_proc::fourchan_proc() : 
    chan_proc(kon("127.0.0.1:9050", true), chan_db("fourchan")) {}
