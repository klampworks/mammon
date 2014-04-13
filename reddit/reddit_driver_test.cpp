#include "reddit_driver.hpp"
#include <vector>
#include <string>

int main()
{
	std::vector<std::string> subreddits;
	reddit_driver d(std::move(subreddits));

}
