#include "ext.hpp"
#include <chibi/eval.h>

namespace ext {

    bool load_file(sexp ctx, const char *filename)
    {
        sexp_gc_var2(filename_ss, ret);
        sexp_gc_preserve2(ctx, filename_ss, ret);
        filename_ss = sexp_c_string(ctx, filename, -1);
        ret = sexp_load(ctx, filename_ss, NULL);

        bool all_good = !sexp_exceptionp(ret);

        sexp_gc_release2(ctx);
        return all_good;
    }

    bool fun_is_defined(sexp ctx, const char *function_name)
    {
        return false;
    }
}
