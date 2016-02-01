#pragma once
#include <chibi/eval.h>

namespace ext {

    bool load_file(sexp ctx, const char *filename);
    bool fun_is_defined(sexp ctx, const char *function_name);
}
