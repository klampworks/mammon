#ifndef SECCHAN_DRIVER_HPP
#define SECCHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct secachan_driver : public chan_driver {

	secachan_driver(std::vector<std::string>&&);
	~secachan_driver();
	void grab_post_img(const chan_post&, const std::string&);
};
#endif
