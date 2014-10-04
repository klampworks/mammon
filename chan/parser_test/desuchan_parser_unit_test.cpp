#include "../desuchan_parser.hpp"
#include "../unit_test/parser_common.hpp"

int main(int argc, char **argv)
{
	assert(argc == 2);
	desuchan_parser p;

	std::string xml = dump_file(argv[1]);
	auto ret = p.parse_threads("test", xml);
	std::cout << "Parsed " << ret.size() << " posts.\n" << std::endl;

	for (const auto &r : ret) {
		r.begin()->print();
	}
}
