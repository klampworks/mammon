#define PUGIXML_HEADER_ONLY
#include "../pugi_lib/pugixml.cpp"
#include "reddit_parser.hpp"
#include <boost/regex.hpp>

#include <iostream>
#include <vector>

std::vector<std::string> reddit_parser::parse_posts(const std::string &value)
{
	std::vector<std::string> return_v;

	std::string::const_iterator start, end;
	start = value.begin();
	end = value.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	
	boost::regex expression("blank \" href=\"([^\"]+)\"");

	while(regex_search(start, end, what, expression, flags)) {
		
		return_v.push_back(what[1]);
		start = what[0].second;
	}

	return return_v;
}

std::vector<std::string> reddit_parser::parse_images(const std::string &value)
{
	auto ret = parse_posts(value);
	ret.erase(ret.remove_if(ret.begin(), ret.end(), 
		[](const std::string &a)
		{
			return a[4] != 's';
		}), 
		ret.end());

	return ret;
}
