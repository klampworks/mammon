#pragma once
#include <chibi/eval.h>

namespace ext {

    bool load_file(sexp ctx, const char *filename);
    bool fun_is_defined(sexp ctx, const char *function_name);
    int call_fun_str(sexp ctx, const char *fun_name, const char *arg);
    int config_get_int(sexp ctx, const char *sym, int def);
}
