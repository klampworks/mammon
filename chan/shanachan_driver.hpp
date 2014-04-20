#ifndef SHANACHAN_DRIVER_HPP
#define SHANACHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct shanachan_driver : public chan_driver {

	shanachan_driver(std::vector<std::string>&&);
	~shanachan_driver();
	void grab_post_img(const chan_post&, const std::string&, const std::string &);
};
#endif
