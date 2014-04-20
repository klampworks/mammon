#ifndef SECCHAN_DRIVER_HPP
#define SECCHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct secchan_driver : public chan_driver {

	secchan_driver(std::vector<std::string>&&);
	~secchan_driver();
	void grab_post_img(const chan_post&, const std::string&, const std::string&);
};
#endif
