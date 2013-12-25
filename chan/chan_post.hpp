#ifndef CHAN_POST_HPP
#define CHAN_POST_HPP

struct chan_post {

	std::string
		board,
		thread_id,
		post_id,
		img,
		content;
	
	chan_post(const std::string &board_p, const std::string &thread_id_p,
		const std::string &post_id_p, const std::string &img_p, 
		const std::string &content_p) :
			board(board_p), thread_id(thread_id_p), post_id(post_id_p),
			img(img_p), content(content_p) {}

	chan_post(const std::string &board_p, const std::string &thread_id_p,
		std::string &&post_id_p, std::string &&img_p, std::string &&content_p) :
			board(board_p), thread_id(thread_id_p), post_id(std::move(post_id_p)),
			img(std::move(img_p)), content(std::move(content_p)) {}
};

#endif