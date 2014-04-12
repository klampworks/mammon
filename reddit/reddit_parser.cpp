#include "reddit_parser.hpp"
#include <boost/regex.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

/* Parse posted links on a page. */
std::vector<std::string> reddit_parser::parse_posts(const std::string &value)
{
	static const boost::regex expression("blank \" href=\"([^\"]+)\"");
	return parse_list(value, expression);
}

/* Parse image links only. */
std::vector<std::string> reddit_parser::parse_images(const std::string &value)
{
	static const boost::regex expression(
		"blank \" href=\"(http[^\"]+\\.(jpe?g|png|gif))\"");
	return parse_list(value, expression);
}
