#pragma once

#include <string>

class chan_post {
    std::string board;
    std::string thread_id;
    std::string id;
    std::string sub;
    std::string com;
    std::string filename;

public:
    /* Constructor for golden output unit tests. */
    chan_post(std::string board, std::string thread_id, std::string id, 
        std::string sub, std::string com, std::string filename) :
            board(board), thread_id(thread_id), id(id), sub(sub), 
                com(com), filename(filename)
    {}

    /* Constructor for partially constructing a post and using it as a prototype
     * to base other posts on (i.e. members of the same thread. 
     */
    chan_post(std::string board, std::string thread_id) :
        board(board), thread_id(thread_id)
    {}

    chan_post finish(std::string id, std::string sub, 
        std::string com, std::string tim, std::string ext) const
    {
        chan_post ret = *this;
        ret.id = id;
        ret.sub = sub;
        ret.com = com;
        ret.filename = mk_filename(tim, ext);
        return ret;
    }

    bool operator==(const chan_post &other) const
    {
        return this->id == other.id 
            && this->sub == other.sub
            && this->filename == other.filename
            && this->thread_id == other.thread_id
            && this->com == other.com;
    }

    static std::string mk_filename(std::string tim, std::string ext)
    {
        return tim + ext;
    }

    void set_filename(std::string tim, std::string ext)
    {
        filename = mk_filename(tim, ext);
    }

    std::string get_board() const
    {
        return board;
    }

    std::string get_thread_id() const
    {
        return thread_id;
    }

    std::string get_id() const
    {
        return id;
    }

    std::string get_sub() const
    {
        return sub;
    }

    std::string get_com() const
    {
        return com;
    }

    std::string get_filename() const
    {
        return filename;
    }
};
