#include <string>
#include <fstream>
#include <iostream>

std::string read_file(const char *filename)
{
    std::ifstream ifs;
    ifs.open(filename);

    std::string out, tmp;

    while(std::getline(ifs, tmp))
        out += tmp;

    ifs.close();
    return out;
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        std::cout << "Enter a filename." << std::endl;
        return 1;
    }

    auto s = read_file(argv[1]);
    std::cout << s << std::endl;
}
