#include "chan_post.hpp"
#include "eightchan_parser.hpp"
#include "rb_chan_post_multi.hpp"
#include <ruby.h>
#include <vector>

eightchan_parser::eightchan_parser()
{
    rb_chan_post_multi::define();

    int status;
    rb_load_protect(rb_str_new2("./eightchan_parser.rb"), 0, &status);
    if (status) {
        VALUE rbError = rb_funcall(rb_gv_get("$!"), rb_intern("message"), 0);
        std::cerr << StringValuePtr(rbError) << std::endl;
    };
}

std::vector<std::vector<chan_post>> eightchan_parser::parse_threads(
    const char *board, const std::string &xml)
{
    VALUE result = rb_funcall(Qnil, rb_intern("parse"), 1, 
        rb_str_new2(xml.c_str()));

    return rb_chan_post_multi::unwrap_threads(result);
}

bool eightchan_parser::final_page(const std::string &xml)
{
    rb_chan_post_multi::define();

    VALUE result = rb_funcall(Qnil, rb_intern("final_page?"), 1, 
        rb_str_new2(xml.c_str()));

    return TYPE(result) == T_TRUE;
}
