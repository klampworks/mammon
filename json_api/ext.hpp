#pragma once
#include <chibi/eval.h>
#include <string>
#include <vector>

namespace ext {

    bool load_file(sexp ctx, const char *filename);
    bool fun_is_defined(sexp ctx, const char *function_name);
    int call_fun_str2(sexp ctx, const char *fun_name,
        const char *arg1, const char *arg2);
    int config_get_int(sexp ctx, const char *sym, int def);
    std::string config_get_str(sexp ctx, const char *sym, const char *def);
    std::vector<std::string> config_get_list(sexp ctx, const char *sym);
}
