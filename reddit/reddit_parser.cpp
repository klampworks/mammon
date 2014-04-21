#include "reddit_parser.hpp"
#include <boost/regex.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

/* Parse posted links on a page. */
std::vector<std::string> reddit_parser::parse_urls(const std::string &value)
{
	static const boost::regex expression("blank \" href=\"(http[^\"]+)\"");
	return parse_list(value, expression);
}

/* Parse image links only. */
std::vector<std::string> reddit_parser::parse_images(const std::string &value)
{
	static const boost::regex expression(
		"blank \" href=\"(http[^\"]+\\.(jpe?g|png|gif))\"");
	return parse_list(value, expression);
}

/* Parse next url. */
std::string reddit_parser::parse_next(const std::string &value)
{
	/* TODO Add a parse single string function to base_parser. */

	/* TODO figure out why &amp;=&amp; strings magically appear in the 
	 * middle of urls until it is so long that Reddit servers reject the 
	 * request. */
	static const boost::regex expression(
		"href=\"[^\"]+(count[^\"]+)\" rel=\"nofollow next\"");

	auto tmp = parse_list(value, expression);

	if (tmp.empty())
		return "";
	else
		return "?" + tmp[0];
}
