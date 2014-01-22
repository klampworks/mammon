#ifndef WAKACHAN_DRIVER_HPP
#define WAKACHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct wakachan_driver : public chan_driver {

	wakachan_driver(std::vector<std::string>&&);
	~wakachan_driver();
	void grab_post_img(const chan_post&, const std::string&);
};
#endif
