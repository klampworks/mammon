#ifndef CHAN_POST_HPP
#define CHAN_POST_HPP

	#include <iostream>
struct chan_post {

	std::string
		board,
		thread_id,
		post_id,
		img,
		img_url,
		content;
	
	chan_post(){}

	chan_post(
		const std::string &board_p, 
		const std::string &thread_id_p,
		const std::string &post_id_p, 
		const std::string &img_p, 
		const std::string &img_url_p, 
		const std::string &content_p) :
			board(board_p), 
			thread_id(thread_id_p), 
			post_id(post_id_p),
			img(img_p), img_url(img_url_p), 
			content(content_p) {}

	chan_post(
		const std::string &board_p, 
		const std::string &thread_id_p,
		std::string &&post_id_p, 
		std::string &&img_p, 
		std::string &&img_url_p, 
		std::string &&content_p) :
			board(board_p), 
			thread_id(thread_id_p), 
			post_id(std::move(post_id_p)),
			img(std::move(img_p)), 
			img_url(std::move(img_url_p)), 
			content(std::move(content_p)) {}

	/* TODO ifdef debug. */
	void print() const {
		std::cout 
			<< "Board = " << board << "\n"
			<< "Thread_id = " << thread_id << "\n"
			<< "Post_id = " << post_id << "\n"
			<< "Img = " << img << "\n"
			<< "Img_url = " << img_url << "\n"
			<< "Content = " << content.substr(0, 160) << "\n"
			<< std::endl;
	}

};

#endif
