#include "eightchan_proc.hpp"
#include <string>

std::string eightchan_proc::mk_base_url()
{
    return "http://fullchan4jtta4sx.onion/";
}

std::string eightchan_proc::mk_file_url(const std::string b, const std::string filename)
{
    return mk_base_url() + b + "/src/" + filename;
}

std::string eightchan_proc::mk_thread_url(const std::string b, 
    const std::string thread_id)
{
    return mk_base_url() + b + "/res/" + thread_id + ".json";
}

std::string eightchan_proc::mk_board_url(const std::string b)
{
    return mk_base_url() + b + "/threads.json";
}

eightchan_proc::eightchan_proc() : 
    chan_proc(kon("127.0.0.1:9050", true), chan_db("eightchan"), 
        new fourchan_parser_json()) {}
