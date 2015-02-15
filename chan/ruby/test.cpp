#include <iostream>
#include <ruby.h>

std::string dorb(const char *name)
{
    int status;

    //rb_protect((VALUE(*)(VALUE))rb_require, rb_str_new2("/usr/bin/nokogiri"), &status);
    
    rb_load_protect(rb_str_new2(name), 0, &status);
    if (status) {
        VALUE rbError = rb_funcall(rb_gv_get("$!"), rb_intern("message"), 0);
        std::cerr << StringValuePtr(rbError) << std::endl;
    };
    
    VALUE result = rb_funcall(Qnil, rb_intern("parse"), 1, 
        rb_str_new2("./test.html"));

    return std::string(StringValuePtr(result));
}

int main()
{
    ruby_init();
    ruby_init_loadpath();

    std::cout << "Hello, world from C++" << std::endl;
    std::cout << "Ruby says <" << dorb("./test2.rb") << ">" << std::endl;

    ruby_finalize();
}
