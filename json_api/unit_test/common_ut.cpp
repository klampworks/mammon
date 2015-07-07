#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <iostream>

namespace common_ut {
    template <typename T>
    T read_file(   
        const char *filename, 
        std::function<void(T&, const std::string&)> fn)
    {
        std::ifstream ifs;
        ifs.open(filename);

        if (!ifs) {
            std::cout << "Could not open file " << filename << std::endl;
            abort();
        }

        std::string tmp;
        T out;

        while(std::getline(ifs, tmp))
            fn(out, tmp);

        ifs.close();
        return out;
    }

    std::string read_file_string(const char *filename)
    {
        return read_file<std::string>(filename, 
            [](std::string &col, const std::string &line) {
                return col += line; });
    }

    std::vector<std::string> read_file_vector(const char *filename)
    {
        return read_file<std::vector<std::string>>(filename,
            [](std::vector<std::string> &col, const std::string &line) {
                col.push_back(line); });
    }
}
