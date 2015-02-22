#pragma once

#include "chan_post_multi.hpp"
#include <ruby.h>
#include <functional>

namespace rb_chan_post_multi {

    chan_post_multi unwrap(VALUE rb_cpm);
    std::vector<std::vector<chan_post_multi>> unwrap_threads(VALUE threads);

    template <class R>
    std::vector<R> unwrap_multi(VALUE rb_cpms, std::function<R(VALUE)> fn)
    {
        std::vector<R> ret;
        if (TYPE(rb_cpms) == T_ARRAY) {
            VALUE v = rb_ary_pop(rb_cpms);

            while (TYPE(v) != T_NIL) {
                ret.push_back(fn(v));
                v = rb_ary_pop(rb_cpms);
            }
        }
        return ret;
    }
    void define();
}
