#define BOOST_TEST_MODULE ext_ut
#include "../../chan/unit_test/but.hpp"
#include "../ext.hpp"

/* TODO It might be worth diving deeper into Chibi exception handling. */
BOOST_AUTO_TEST_CASE(load_nonexistant_script)
{
    sexp_gc_var1(ret);
    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    ret = ext::load_file(ctx, "does_not_exist.ss");
    BOOST_CHECK(sexp_exceptionp(ret));

    sexp_gc_release1(ctx);
}
