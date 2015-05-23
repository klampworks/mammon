#pragma once

#include <iostream>
#include <fstream>
#include <string>

bool file_exists(const std::string &fn)
{
	std::fstream f;

	f.open(fn.c_str());
	bool res = f.good();
	f.close();
	return res;
}

void rm(const std::string &fn)
{
	if (file_exists(fn))
		remove(fn.c_str());
}

