#include "../eightchan_parser.hpp"
#include "../chan_post.hpp"
#include <vector>
#include <fstream>
#include <cassert>

std::string dump_file(const char *filename) {

	std::ifstream ifs;
	ifs.open(filename);

	assert(ifs.good());

	std::string xml, tmp;

	while(std::getline(ifs, tmp))
		xml += tmp;

	ifs.close();
	return std::move(xml);
}

int main(int argc, char **argv)
{
    eightchan_parser p;
    auto html = dump_file(argv[1]);

    auto t = p.parse_threads("test", html);
    for (const auto &tt: t) {
        for (const auto &ttt : tt) {
            ttt.print();
        }
        std::cout << "########################################" << std::endl;
    }
    
    std::cout << p.final_page(html) << std::endl;
}
