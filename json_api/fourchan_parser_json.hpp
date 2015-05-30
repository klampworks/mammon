#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree_fwd.hpp>

struct chan_post {
    std::string thread_id;
    std::string id;
    std::string sub;
    std::string com;
    std::string filename;

    /* Constructor for golden output unit tests. */
    chan_post(std::string thread_id, std::string id, std::string sub, 
        std::string com, std::string filename) :
            thread_id(thread_id), id(id), sub(sub), com(com), filename(filename)
    {}

    /* Constructor for partially constructing a post and using it as a prototype
     * to base over posts on (i.e. members of the same thread. 
     */
    chan_post(std::string thread_id) :
        thread_id(thread_id)
    {}

    chan_post(std::string thread_id, std::string id, std::string sub, 
        std::string com, std::string tim, std::string ext) :
            thread_id(thread_id), id(id), sub(sub), com(com), 
            filename(mk_filename(tim, ext))
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

    static std::string mk_filename(std::string tim, std::string ext)
    {
        return tim + ext;
    }

    void set_filename(std::string tim, std::string ext)
    {
        filename = mk_filename(tim, ext);
    }

    bool operator==(const chan_post &other) const
    {
        return this->id == other.id 
            && this->sub == other.sub
            && this->filename == other.filename
            && this->thread_id == other.thread_id
            && this->com == other.com;
    }
};

class fourchan_parser_json {
    public:
        std::vector<std::string> parse_threads(const std::string &json);
        std::vector<chan_post> parse_posts(const std::string &json, 
            const chan_post=chan_post("0"));
    protected:
        boost::property_tree::ptree parse_json(const std::string &json);

        template <typename T>
        std::string sget(T &pt, const char *node);
};
