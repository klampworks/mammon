#define BOOST_TEST_MODULE ext_ut
#include "../../chan/unit_test/but.hpp"
#include "../ext.hpp"
#include <fstream>

BOOST_AUTO_TEST_CASE(load_nonexistant_script)
{
    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    bool ret = ext::load_file(ctx, "does_not_exist.ss");
    BOOST_CHECK(!ret);
}

BOOST_AUTO_TEST_CASE(load_existing_script)
{
    const char *fn = "blank.ss";
    std::ofstream ofs;
    ofs.open("blank.ss");
    ofs << std::endl;
    ofs.close();

    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    bool ret = ext::load_file(ctx, fn);
    BOOST_CHECK(ret);
}

BOOST_AUTO_TEST_CASE(fun_is_not_defined)
{
    const char *fn = "blank.ss";
    std::ofstream ofs;
    ofs.open("blank.ss");
    ofs << std::endl;
    ofs.close();

    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    ext::load_file(ctx, fn);
    bool ret = ext::fun_is_defined(ctx, "no-such-fun");
    BOOST_CHECK(!ret);
}
