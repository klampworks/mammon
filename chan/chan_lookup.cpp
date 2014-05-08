#include "chan_db.hpp"
#include "../base_db.hpp"

#include <iostream>
#include <unistd.h>
#include <cstring>

void validate_table(const char *table)
{
	static const char *tables[] = {
		"desuchan",
		"wakachan",
		"shanachan",
		"secchan",
		"fourchan",
		"redditchan"
	};

	for (size_t i = 0; i < sizeof(tables) / sizeof tables[0]; ++i) {
		if (!strcmp(table, tables[i]))
			return;
	}

	std::cout << "Error invalid table name " << table << std::endl;
	exit(1);
}

int main(int argc, char **argv)
{
	chan_db::init();

	extern char *optarg;

	char *table, *value;

	int c;
	while((c = getopt(argc, argv, "b:v:")) != -1) {
		switch (c) {
		case 'b':
			table = optarg;	
			break;
		case 'v':
			value = optarg;
			break;
		}
	}

	validate_table(table);	
	std::string stmt("select * from " + 
		std::string(table) + " where img == ?");

	auto res = base_db::lookup_single_value(stmt, value);

	std::cout << res << std::endl;
}
