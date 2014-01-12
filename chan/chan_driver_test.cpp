#include "desuchan_driver.hpp"
#include "wakachan_driver.hpp"
#include "../kyukon/kyukon.hpp"
#include <fstream>

int main() {

	std::ifstream ifs;
	ifs.open("proxies");

	assert(ifs.good());

	std::vector<std::pair<std::string, bool>> proxies;
	std::string tmp;

	while(std::getline(ifs, tmp)) {
		
		assert(!tmp.empty());
		proxies.push_back(std::pair<std::string, bool>(tmp, true));
	}

	assert(!proxies.empty());

	kyukon::init(proxies);
	ifs.close();

	ifs.open("desuchan_boards");
	std::vector<std::string> boards;

	while(std::getline(ifs, tmp))
		boards.push_back(tmp);

	//desuchan_driver d(std::move(boards));
	wakachan_driver d(std::move(boards));

	while(1);
}
