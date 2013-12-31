#include "base_driver.hpp"
#include <string>
#include "kyukon/task.hpp"
#include "kyukon/kyukon.hpp"
#include <iostream>
#include <sys/stat.h>


base_driver::base_driver() {

	max_retries = 5;
	magic_cookie = magic_open(MAGIC_MIME); 
	magic_load(magic_cookie, NULL);
}

bool base_driver::check_error(task *t) {

	for (std::string res = t->get_curl_result(); !res.empty();) {
		
		std::cout << "Curl error: " << res << std::endl;
		return false;
	}

	if (!t->get_data_size())
		return false;

	return true;
}

bool base_driver::check_file_error(task *t) {

	//check filesize
	if (!check_filesize(t))
		return false;

	//check file type
	if (file_is_txt(t))
		return false;

	return true;
}

void base_driver::retry(task *t) {

	if (t->inc_retries() > max_retries) {

		std::cout << "Giving up on " << t->get_url() << std::endl;
		delete t;
	} else {
		
		kyukon::add_task(t);
	}
}

bool base_driver::check_filesize(task *t) {

	struct stat filestatus;
	stat(t->get_data().c_str(), &filestatus);
	double real_file_size = filestatus.st_size;

	if (real_file_size != t->get_data_size()) {

		std::cout << "Size mismatch: real size = " << real_file_size <<
		" but downloaded size = " << t->get_data_size() << std::endl;;
		return false;
	}

	return true;
}

bool base_driver::file_is_txt(task *t) {
	return check_filetype(t);
}

bool base_driver::file_is_bin(task *t) {
	return !check_filetype(t);
}

bool base_driver::check_filetype(task *t) {

	std::string mime = magic_file(magic_cookie, t->get_data().c_str());
	return (mime.find("text") != std::string::npos); 
}
