#ifndef FOURCHAN_DRIVER_HPP
#define FOURCHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct fourchan_driver : public chan_driver {

	fourchan_driver(std::vector<std::string>&&);
	~fourchan_driver();
};
#endif
