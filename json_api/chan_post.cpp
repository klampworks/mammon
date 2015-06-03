#include "chan_post.hpp"
#include <string>
#include <algorithm>

chan_post chan_post::finish(std::string id, std::string sub, 
    std::string com, std::vector<std::string> filenames) const
{
    chan_post ret = *this;
    ret.id = id;
    ret.sub = sub;
    ret.com = com;
    ret.filenames = filenames;
    return ret;
}

bool chan_post::operator==(const chan_post &other) const
{
    if (this->id != other.id 
        || this->sub != other.sub
        || this->thread_id != other.thread_id
        || this->com != other.com) 
    {
        return false;
    }
    
    const auto &o = other.get_filenames();
    for (const auto &a : get_filenames()) {
        if (std::find(o.begin(), o.end(), a) == o.end()) {
            return false;
        }
    }

    return true;
}

std::string chan_post::mk_filename(std::string tim, std::string ext)
{
    return tim + ext;
}

std::string chan_post::get_board() const
{
    return board;
}

std::string chan_post::get_thread_id() const
{
    return thread_id;
}

std::string chan_post::get_id() const
{
    return id;
}

std::string chan_post::get_sub() const
{
    return sub;
}

std::string chan_post::get_com() const
{
    return com;
}

std::vector<std::string> chan_post::get_filenames() const
{
    return filenames;
}

#include <iostream>
void chan_post::print() const {
    std::cout 
        << "Board = " << get_board() << "\n"
        << "Thread_id = " << get_thread_id() << "\n"
        << "Post_id = " << get_id() << "\n"
        << "Content = " << get_com().substr(0, 160) << "\n"
        << "Filenames = [" << std::endl;
    for (const auto &a : get_filenames()) {
        std::cout << a << "\n";
    }
        std::cout << "]" << std::endl;
}
