#include "chan_post.hpp"
#include <string>

chan_post chan_post::finish(std::string id, std::string sub, 
    std::string com, std::string tim, std::string ext) const
{
    chan_post ret = *this;
    ret.id = id;
    ret.sub = sub;
    ret.com = com;
    ret.filename = mk_filename(tim, ext);
    return ret;
}

bool chan_post::operator==(const chan_post &other) const
{
    return this->id == other.id 
        && this->sub == other.sub
        && this->filename == other.filename
        && this->thread_id == other.thread_id
        && this->com == other.com;
}

std::string chan_post::mk_filename(std::string tim, std::string ext)
{
    return tim + ext;
}

void chan_post::set_filename(std::string tim, std::string ext)
{
    filename = mk_filename(tim, ext);
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

std::string chan_post::get_filename() const
{
    return filename;
}

#include <iostream>
void chan_post::print() const {
    std::cout 
        << "Board = " << get_board() << "\n"
        << "Thread_id = " << get_thread_id() << "\n"
        << "Post_id = " << get_id() << "\n"
        << "Content = " << get_com().substr(0, 160) << std::endl;
}
