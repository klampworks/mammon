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
    chan_post(std::string id, std::string sub, 
        std::string com, std::string filename) :
            thread_id("0"), id(id), sub(sub), com(com), filename(filename)
    {}
    
    chan_post(std::string thread_id) :
        thread_id(thread_id)
    {}

    void set_filename(std::string tim, std::string ext)
    {
        filename = tim + ext;
    }

    bool operator==(const chan_post &other) const
    {
        return this->id == other.id 
            && this->sub == other.sub
            && this->filename == other.filename
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
