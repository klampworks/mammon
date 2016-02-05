#include "ext.hpp"
#include <chibi/eval.h>
#include <string>

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
        sexp_gc_var1(fun);
        sexp_gc_preserve1(ctx, fun);
        fun = sexp_eval_string(ctx, function_name, -1, NULL);

        bool fun_exists = sexp_procedurep(fun);

        sexp_gc_release1(ctx);
        return fun_exists;
    }

    int call_fun_str(sexp ctx, const char *fun_name, const char *arg)
    {
        sexp_gc_var3(arg_sym, arg_val, ret);
        sexp_gc_preserve3(ctx, arg_sym, arg_val, ret);

        arg_val = sexp_c_string(ctx, arg, -1);
        arg_sym = sexp_intern(ctx, "arg", -1);
        sexp_env_define(ctx, sexp_context_env(ctx), arg_sym, arg_val);

        const std::string code(
            std::string("(") + fun_name + std::string(" arg)"));
        ret = sexp_eval_string(ctx, code.c_str(), -1, NULL);

        int r;
        if (sexp_integerp(ret))
            r = sexp_unbox_fixnum(ret);
        else
            r = -1;

        sexp_gc_release3(ctx);

        return r;
    }

    int config_get_int(sexp ctx, const char *sym, int def)
    {
        return def;
    }
}
