#include "ext.hpp"
#include <chibi/eval.h>
#include <string>

#include <iostream>
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

    int call_fun_str2(sexp ctx, const char *fun_name,
        const char *arg1, const char *arg2)
    {
        sexp_gc_var5(arg1_sym, arg1_val, arg2_sym, arg2_val, ret);
        sexp_gc_preserve5(ctx, arg1_sym, arg1_val, arg2_sym, arg2_val, ret);

        arg1_val = sexp_c_string(ctx, arg1, -1);
        arg1_sym = sexp_intern(ctx, "arg1", -1);
        sexp_env_define(ctx, sexp_context_env(ctx), arg1_sym, arg1_val);

        arg2_val = sexp_c_string(ctx, arg2, -1);
        arg2_sym = sexp_intern(ctx, "arg2", -1);
        sexp_env_define(ctx, sexp_context_env(ctx), arg2_sym, arg2_val);

        const std::string code(
            std::string("(") + fun_name + std::string(" arg1 arg2)"));
        ret = sexp_eval_string(ctx, code.c_str(), -1, NULL);

        int r;
        if (sexp_integerp(ret))
            r = sexp_unbox_fixnum(ret);
        else
            r = -1;

        sexp_gc_release5(ctx);

        return r;
    }

    int config_get_int(sexp ctx, const char *sym, int def)
    {
        sexp_gc_var1(ret);
        sexp_gc_preserve1(ctx, ret);
        ret = sexp_eval_string(ctx, sym, -1, NULL);

        int r;
        if (sexp_integerp(ret))
            r = sexp_unbox_fixnum(ret);
        else
            r = def;

        sexp_gc_release1(ctx);
        return r;
    }

    std::string config_get_str(sexp ctx, const char *sym, const char *def)
    {
        sexp_gc_var1(ret);
        sexp_gc_preserve1(ctx, ret);
        ret = sexp_eval_string(ctx, sym, -1, NULL);

        std::string r;
        if (sexp_stringp(ret))
            r = sexp_string_data(ret);
        else
            r = def;

        sexp_gc_release1(ctx);
        return r;
    }

    std::vector<std::string> config_get_list(sexp ctx, const char *sym)
    {
        sexp_gc_var2(ret, kar);
        sexp_gc_preserve2(ctx, ret, kar);
        ret = sexp_eval_string(ctx, sym, -1, NULL);

        std::vector<std::string> r;

        while (sexp_pairp(ret)) {

            kar = sexp_car(ret);
            if (sexp_stringp(kar))
                r.push_back(sexp_string_data(kar));
            ret = sexp_cdr(ret);
        }

        sexp_gc_release2(ctx);
        return r;
    }
}
