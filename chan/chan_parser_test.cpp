#include "chan_parser.hpp"

int main(int argc, char **argv) {

	chan_parser p;
	p.parse_posts("input_2.html");

	return 1;
	std::vector<std::string> res = parse_thread_ids();
	res = convert_id_to_url(res);

	for (const auto &a : res) {
		if (!a.empty())
			std::cout << a << std::endl;
	}
}
