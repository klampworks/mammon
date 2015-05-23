#ifndef SHANACHAN_DRIVER_HPP
#define SHANACHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct shanachan_driver : public chan_driver {

	shanachan_driver(std::vector<std::string>&&);
	~shanachan_driver();
    std::string mk_file_url(std::string url);
};
#endif
