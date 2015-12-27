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
        cmd("mkdir -p " + test_dir);
    }

    ~fs_fix()
    {
        cmd("rm -r " + test_dir);
    }
};

BOOST_FIXTURE_TEST_CASE(file_does_not_exist, fs_fix) 
{
    const std::string test_file = "hello";
    BOOST_CHECK(!fs::file_exists(test_dir + test_file));
}

BOOST_FIXTURE_TEST_CASE(file_does_exist, fs_fix) 
{
    const std::string test_file = "hello";
    BOOST_CHECK(!fs::file_exists(test_dir + test_file));

    cmd("touch " + test_dir + "/" + test_file);
    BOOST_CHECK(fs::file_exists(test_dir + test_file));
}

BOOST_FIXTURE_TEST_CASE(move_file, fs_fix) 
{
    const std::string f1 = "hello";
    const std::string f2 = "world";

    BOOST_CHECK(!fs::file_exists(test_dir + f1));
    BOOST_CHECK(!fs::file_exists(test_dir + f2));

    cmd("touch " + test_dir + "/" + f1);
    BOOST_CHECK(fs::file_exists(test_dir + f1));
    BOOST_CHECK(!fs::file_exists(test_dir + f2));

    BOOST_CHECK(fs::move(test_dir + f1, test_dir + f2));
    BOOST_CHECK(!fs::file_exists(test_dir + f1));
    BOOST_CHECK(fs::file_exists(test_dir + f2));
}

