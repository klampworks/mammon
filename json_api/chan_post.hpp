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
        std::string com, std::string tim, std::string ext) const;

    bool operator==(const chan_post &other) const;

    static std::string mk_filename(std::string tim, std::string ext);

    void set_filename(std::string tim, std::string ext);

    std::string get_board() const;
    std::string get_thread_id() const;
    std::string get_id() const;
    std::string get_sub() const;
    std::string get_com() const;
    std::string get_filename() const;
	void print() const;
};
