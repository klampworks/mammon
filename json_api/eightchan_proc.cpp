#include "eightchan_proc.hpp"
#include <string>
#include <iostream>

std::string eightchan_proc::mk_base_url()
{
    static const char *site_name = "MAMMON_8CHAN_URL";

    if (base_url.empty()) {
        try {
            base_url = std::string(getenv(site_name));
        } catch (const std::logic_error &e) {
            std::cout << "\n~~~ Sorry, please make sure that the environmental "
                "variable \n   $" << site_name << " is set correctly. ~~~\n"
                << std::endl;
            throw;
        }
    }

    return base_url;
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
