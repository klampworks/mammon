#ifndef CHAN_DB_HPP
#define CHAN_DB_HPP

#include <string>

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
};
namespace chan_db {

	void init();
	void init_table(const char *table_name);
	void insert_post(const char*, const std::string&, const std::string&,
		const std::string&, const std::string&, const std::string&);


}//namespace

#endif
