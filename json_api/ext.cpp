#include "ext.hpp"
#include <chibi/eval.h>

namespace ext {

    sexp load_file(sexp ctx, const char *filename)
    {
        sexp_gc_var1(filename_ss);
        sexp_gc_preserve1(ctx, filename_ss);
        filename_ss = sexp_c_string(ctx, filename, -1);
        sexp ret = sexp_load(ctx, filename_ss, NULL);

        sexp_gc_release1(ctx);
        return ret;
    }
}
