#define BOOST_TEST_MODULE fourchan_proc_ut
#include "../../chan/unit_test/but.hpp"
#include "../fourchan_proc.hpp"

BOOST_AUTO_TEST_CASE(file_url) 
{
    const auto expected = "http://i.4cdn.org/x/hello.pdf";
    const auto got = fourchan_proc::mk_file_url("x", "hello.pdf");

    BOOST_CHECK_MESSAGE(got == expected, 
        "Expected <" << expected << "> but got <" << "got <" << got << ">");
}
