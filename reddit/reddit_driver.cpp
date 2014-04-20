#include "reddit_db.hpp"
#include "reddit_parser.hpp"
#include "reddit_driver.hpp"
#include "../kyukon/kyukon.hpp"

#include <iostream>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>

reddit_driver::reddit_driver(std::vector<std::string> &&subreddits_p) 
	: base_driver(), subreddits(std::move(subreddits_p)) 
{
	this->table_name = "reddit";
	this->base_url = "http://www.reddit.com/r/";

	reddit_db::init();
	reddit_db::init_table(table_name);

	domain_id = kyukon::signup(30, std::bind(&reddit_driver::fillup, this));
	kyukon::set_do_fillup(true, domain_id);
	std::cout << domain_id << std::endl;
		
	/* Create a base directory for this class. */
	create_path(std::string(table_name) + "/");
}

unsigned subreddit = 0;
std::string next_url;

void reddit_driver::fillup() {

	/* TODO This logic ignores the first subreddit given. */
	if (next_url.empty()) { 
		std::cout << "Done all pages for subreddit " << 
			subreddits[subreddit]<< std::endl;
		
		if (++subreddit >= subreddits.size()) {
			std::cout << "No more subreddits available." 
				<< std::endl;
			return;
		} else {
			std::cout << "Moving on to subreddit " << 
				subreddits[subreddit] << std::endl;
			next_url = base_url + subreddits[subreddit];
		}
	}
	
	std::string path = create_path();

	task *t = new task(
		domain_id, next_url, base_url, task::STRING, 
		std::bind(&reddit_driver::process_list_page, this, 
		std::placeholders::_1));

	t->set_priority(1);
	t->set_filepath(path);

	std::cout << t->get_url() << std::endl;
	kyukon::add_task(t);

	//Tell Kyukon we are done filling up and it's ok to call this function
	//again if the queue is empty again.
	//kyukon::set_do_fillup(true, domain_id);
}

//Given the html souce, figure out which threads need crawling.
void reddit_driver::process_list_page(task *t) {

	if (!check_error(t)) {

		retry(t);
		return;	
	}

	auto urls = parser.parse_images(t->get_data());
	for (const auto url : urls) {

		/* Skip this url if it already in the database. */
		if (reddit_db::url_exists(table_name, url))
			continue;

		task *tt = new task(domain_id, url, t->get_url(), task::FILE, 
			std::bind(&reddit_driver::process_image, this, 
			std::placeholders::_1));

		tt->set_filepath(t->get_filepath());
		tt->set_priority(4);	
		kyukon::add_task(tt);
	}

	next_url = parser.parse_next(t->get_data());

	delete t;
	kyukon::set_do_fillup(true, domain_id);
}

void reddit_driver::process_image(task *t) {

#if 0
	if (!check_error(t) || !check_file_error(t)) {
		retry(t);
		return;	
	}
#endif

	delete t;
}

#if 0

void reddit_driver::grab_thread(
	const reddit_post &post, 
	const std::string &referer,
	const std::string &filepath) 
{
	const std::string &subreddit = post.subreddit;
	const std::string &thread_id = post.thread_id;

	const std::string url = base_url + subreddit + "/res/" + thread_id + ".html"; 


	task *t = new task(domain_id, url, referer, task::STRING, 
		std::bind(&reddit_driver::process_thread, this, std::placeholders::_1));

	t->set_filepath(filepath);
	t->set_priority(3);
	kyukon::add_task(t);
	
}

void reddit_driver::process_thread(task *tt) {

	reddit_task *t = (reddit_task*)tt;

	if (!check_error(t)) {

		retry(t);
		return;	
	}

	//Parse the html into a list of post objects.
	std::vector<reddit_post> thread = parser->parse_thread(t->get_subreddit().c_str(), t->get_data());

	//Add the posts to the database and delete the existing ones from the vector.
	reddit_db::insert_posts(table_name, thread);

	const std::string &referer = t->get_url();
	const std::string &filepath = t->get_filepath();

	//For each new post, grab the image posted to by the img_url field.
	for (const auto &new_post : thread)
		grab_post_img(new_post, referer, filepath);

	delete tt;
}

void reddit_driver::grab_post_img(
	const reddit_post &post, 
	const std::string &referer,
	const std::string &filepath) 
{
	//Not all posts have images.
	if (post.img_url.empty())
		return;

	reddit_task *t = new reddit_task(domain_id, post.img_url, referer, task::FILE, 
		std::bind(&reddit_driver::process_image, this, std::placeholders::_1), post.subreddit);

	t->set_filepath(filepath);
	t->set_priority(4);	
	kyukon::add_task(t);
}

void reddit_driver::quit() {
	kyukon::stop();
}

#endif
bool reddit_driver::create_path(const std::string &path)
{
	struct stat stat_buf;
	int res = stat(path.c_str(), &stat_buf);

	/* No errors from stat. */
	if (!res) {
		
		/* What we want exists and is a directory. */
		if (S_ISDIR(stat_buf.st_mode)) {
			goto good;
		}

		std::cout << "Error file exists but is not a directory." 
			<< std::endl; 
		goto bad;
	}

	/* Path does not exist, good. */
	if (errno == ENOENT) {
		if (!mkdir(path.c_str(), 0777)) {
			goto good;
		} else {
			std::cout << "Error creating directory: " << 
			errno << std::endl;
			goto bad;
		}
	}

	std::cout << "Stat error " << errno << std::endl;
	goto bad;

	good:
		return true;
	bad:
		return false;
}

std::string reddit_driver::create_path()
{
	std::string path(table_name);
	path += "/" + subreddits[subreddit] + "/";  

	if (create_path(path.c_str()))
		return path;
	else
		return "";
}
