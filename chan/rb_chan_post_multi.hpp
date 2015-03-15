#pragma once

#include <ruby.h>
#include <functional>
#include <vector>

class chan_post;

namespace rb_chan_post_multi {

    chan_post unwrap(VALUE rb_cpm);
    std::vector<std::vector<chan_post>> unwrap_threads(VALUE threads);

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
