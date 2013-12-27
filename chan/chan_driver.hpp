#ifndef CHAN_DRIVER_HPP
#define CHAN_DRIVER_HPP

struct chan_driver {

	chan_parser parser;
	const char *table_name = "desuchan";

	void process_list_page(const std::string &xml);
};
#endif
