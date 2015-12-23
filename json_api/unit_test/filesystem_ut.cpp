#define BOOST_TEST_MODULE filesystem_ut
#include "../../chan/unit_test/but.hpp"
#include "../../filesystem.hpp"
#include <cstdlib>

void cmd(const std::string &c)
{
    system(c.c_str());
}

struct fs_fix {

    std::string test_dir;
    fs_fix()
    {
        test_dir = "test/";
        system(std::string("mkdir -p " + test_dir).c_str());
    }

    ~fs_fix()
    {
        system(std::string("rm -r " + test_dir).c_str());
    }
};

