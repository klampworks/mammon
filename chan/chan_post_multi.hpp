#ifndef CHAN_POST_MULTI_HPP
#define CHAN_POST_MULTI_HPP
#include "chan_post.hpp"
#include <string>
#include <vector>

struct chan_post_multi : chan_post
{
    std::vector<std::string> file_ids, file_urls;

	chan_post_multi() {}

	chan_post_multi(
		std::string board_p, 
		std::string thread_id_p,
		std::string post_id_p, 
		std::vector<std::string> file_ids_p, 
		std::vector<std::string> file_urls_p, 
		std::string content_p) :
            chan_post::chan_post(
                board_p, thread_id_p, post_id_p, "", "", content_p),
                file_ids(file_ids_p), file_urls(file_urls_p) {}
};
#endif
