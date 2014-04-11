#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "reddit_parser.hpp"
#include <boost/regex.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::string> reddit_parser::parse_posts(const std::string &value)
{
	static const boost::regex expression("blank \" href=\"([^\"]+)\"");
	return parse_list(value, expression);
}

std::vector<std::string> reddit_parser::parse_images(const std::string &value)
{
	auto ret = parse_posts(value);
	ret.erase(std::remove_if(ret.begin(), ret.end(), 
		[](const std::string &a)
		{
			return a[4] != 's';
		}), 
		ret.end());

	return ret;
}
