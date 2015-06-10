#include "fourchan_proc.hpp"

std::string fourchan_proc::domain = "4cdn.org";
std::string fourchan_proc::text_sub = "a";
std::string fourchan_proc::file_sub = "i";


std::string fourchan_proc::mk_file_url(const std::string b, const std::string filename)
{
    return "http://" + file_sub + "." + domain + "/" + b + "/" + filename;
}

std::string fourchan_proc::mk_thread_url(const std::string b, 
    const std::string thread_id)
{
    return "http://" + text_sub + "." + domain + "/" + b + "/" + thread_id + ".json";
}

std::string fourchan_proc::mk_board_url(const std::string b)
{
    return "http://" + text_sub + "." + domain + "/" + b + "/threads.json";
}

