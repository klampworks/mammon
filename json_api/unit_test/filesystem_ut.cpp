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

BOOST_FIXTURE_TEST_CASE(file_does_not_exist, fs_fix) 
{
    const std::string test_file = "hello";
    BOOST_CHECK(!fs::file_exists(test_dir + test_file));
}

