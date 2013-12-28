#include "chan_driver.hpp"
#include "../kyukon/kyukon.hpp"

int main() {

	std::ifstream ifs;
	ifs.open("proxies");

	assert(ids.good());

	std::vector<std::pair<std::string, bool>> proxies;
	std::string tmp;

	while(std::getline(ifs, tmp)) {
		
		assert(!tmp.empty());
		proxies.push_back(std::pair<std::string, bool>(tmp, true));
	}

	assert(!proxies.empty());

	kyukon::init(proxies);

	chan_driver d;
}
