#ifndef CHAN_POST_HPP
#define CHAN_POST_HPP
#include "chan_post.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

struct chan_post
{
    // TODO Not in the mood for C++'s retarded static initialisation syntax.
	std::string
		board,
		thread_id,
		post_id,
	//	img,
	//	img_url,
		content;

    std::vector<std::string> file_ids, file_urls;
    static const char delim = '\n';

	chan_post() {}

	chan_post(
		std::string board_p, 
		std::string thread_id_p,
		std::string post_id_p, 
		std::vector<std::string> file_ids_p, 
		std::vector<std::string> file_urls_p, 
		std::string content_p) :
                board(board_p), thread_id(thread_id_p), post_id(post_id_p), 
                content(content_p), file_ids(file_ids_p), 
                file_urls(file_urls_p) {}

    static std::string join(const std::vector<std::string> &arr)
    {
        std::string ret("");
        if (ret.empty())
            return ret;

        ret += arr[0];
        for (size_t i = 0; i < arr.size(); ++i)
            ret += delim + arr[i];

        return ret;
    }
            
    static std::vector<std::string> split(const std::string &arr)
    {
        std::vector<std::string> ret;
        std::stringstream ss(arr);
        std::string tmp;
        while (std::getline(ss, tmp, delim)) {
            ret.push_back(tmp);
        }
        
        return ret;
    }
            

	virtual void print() const {
		std::cout 
			<< "Board = " << board << "\n"
			<< "Thread_id = " << thread_id << "\n"
			<< "Post_id = " << post_id << "\n"
			<< "Content = " << content.substr(0, 160) << "\n";
        if (file_ids.size() == file_urls.size()) {
            for (size_t i = 0; i < file_ids.size(); ++i) {
                std::cout << file_ids[i] << " --> " << file_urls[i] << std::endl;
            }
        }
	}
};
#endif
