#ifndef BASE_DRIVER_HPP
#define BASE_DRIVER_HPP

#include <magic.h>
class task;

struct base_driver {

	base_driver();
	bool check_error(task *t);
	bool check_file_error(task *t);
	void retry(task *t);
	unsigned max_retries;

	bool check_filesize(task *t);
	bool file_is_txt(task *t);
	bool file_is_bin(task *t);
	bool check_filetype(task *t);
	magic_t magic_cookie;
};
#endif
