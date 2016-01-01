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

BOOST_FIXTURE_TEST_CASE(get_ext, fs_fix) 
{
    const std::string f1 = "hello.txt";
    cmd("touch " + test_dir + "/" + f1);

    BOOST_CHECK(fs::get_ext(f1) == ".txt");
}

BOOST_FIXTURE_TEST_CASE(get_ext_no_ext, fs_fix)
{
    const std::string f1 = "hello";
    cmd("touch " + test_dir + "/" + f1);

    BOOST_CHECK(fs::get_ext(f1) == "");
}

BOOST_FIXTURE_TEST_CASE(get_ext_dot_no_ext, fs_fix)
{
    const std::string f1 = "hello.";
    cmd("touch " + test_dir + "/" + f1);

    BOOST_CHECK(fs::get_ext(f1) == ".");
}

BOOST_FIXTURE_TEST_CASE(split_ext, fs_fix)
{
    const std::string f1 = "hello.txt";
    cmd("touch " + test_dir + "/" + f1);

    const auto res = fs::split_ext(f1);
    BOOST_CHECK(res.first == "hello");
    BOOST_CHECK(res.second == "txt");
}

BOOST_FIXTURE_TEST_CASE(split_ext_no_ext, fs_fix)
{
    const std::string f1 = "hello";
    cmd("touch " + test_dir + "/" + f1);

    const auto res = fs::split_ext(f1);
    BOOST_CHECK(res.first == "hello");
    BOOST_CHECK(res.second == "");
}

BOOST_FIXTURE_TEST_CASE(split_ext_dot_no_ext, fs_fix)
{
    const std::string f1 = "hello.";
    cmd("touch " + test_dir + "/" + f1);

    const auto res = fs::split_ext(f1);
    BOOST_CHECK(res.first == "hello");
    BOOST_CHECK(res.second == "");
}
