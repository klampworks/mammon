#ifndef EIGHTCHAN_DRIVER_HPP
#define EIGHTCHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct eightchan_driver : public chan_driver {

	eightchan_driver(std::vector<std::string>&&);
	~eightchan_driver();
};
#endif
