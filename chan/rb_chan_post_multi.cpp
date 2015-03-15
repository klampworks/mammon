#include "rb_chan_post_multi.hpp"
#include "chan_post.hpp"
#include <functional>
#include <ruby.h>

namespace rb_chan_post_multi {

    std::string rbstr_to_str(VALUE rbstr)
    {
        return StringValuePtr(rbstr);
    }

    std::string field_to_str(VALUE self, const char *field)
    {
        std::cout << "Field " << field << std::endl;
        auto f = rb_iv_get(self, field);
        return rbstr_to_str(f);
    }

    chan_post unwrap(VALUE rb_cpm) 
    {
std::cout <<"a\n";
        chan_post cpm(
            field_to_str(rb_cpm, "@board"),
            field_to_str(rb_cpm, "@thread_id"),
            field_to_str(rb_cpm, "@post_id"),
            unwrap_multi<std::string>(rb_iv_get(rb_cpm, "@file_ids"), rbstr_to_str),
            unwrap_multi<std::string>(rb_iv_get(rb_cpm, "@file_urls"), rbstr_to_str),
            field_to_str(rb_cpm, "@content"));
        return cpm;
    }

    VALUE rb_chan_post_class;
    bool initted = false;

    static VALUE init(VALUE self, VALUE board_p, VALUE thread_id_p, 
        VALUE post_id_p, VALUE file_ids_p, VALUE file_urls_p, VALUE content_p)
    {
        VALUE board, thread_id, post_id, file_ids, file_urls, content;
        board = board_p;
        thread_id = thread_id_p;
        post_id = post_id_p;
        file_ids = file_ids_p;
        file_urls = file_urls_p;
        content = content_p;

        // TODO Check if argments can be plugged straight into rb_iv_set.
        rb_iv_set(self, "@board", board);
        rb_iv_set(self, "@thread_id", thread_id);
        rb_iv_set(self, "@post_id", post_id);
        rb_iv_set(self, "@file_ids", file_ids);
        rb_iv_set(self, "@file_urls", file_urls);
        rb_iv_set(self, "@content", content);
           
        return self;
    }

    static VALUE get_post_id(VALUE self)
    {
        return rb_iv_get(self, "@post_id");
    }

    static VALUE set_thread_id(VALUE self, VALUE thread_id)
    {
        rb_iv_set(self, "@thread_id", thread_id);
        return self;
    }

    std::vector<std::vector<chan_post>> unwrap_threads(VALUE threads)
    {
        return unwrap_multi<std::vector<chan_post>>(threads, 
            std::bind(unwrap_multi<chan_post>, std::placeholders::_1, unwrap));
    }

    void define()
    {
        if (!initted) {
            ruby_init();
            ruby_init_loadpath();
            initted = true;

            rb_chan_post_class = rb_define_class("Chan_post_multi", rb_cObject);

            rb_define_method(rb_chan_post_class, "initialize", 
                (VALUE(*)(...))init, 6);
            rb_define_method(rb_chan_post_class, "get_post_id", 
                (VALUE(*)(...))get_post_id, 0);
            rb_define_method(rb_chan_post_class, "set_thread_id", 
                (VALUE(*)(...))set_thread_id, 1);
        }
    }
}
