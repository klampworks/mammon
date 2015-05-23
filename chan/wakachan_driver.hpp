#ifndef WAKACHAN_DRIVER_HPP
#define WAKACHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct wakachan_driver : public chan_driver {

	wakachan_driver(std::vector<std::string>&&);
	~wakachan_driver();
    std::string mk_file_url(std::string url);
};
#endif
