#include <iostream>
#include <ruby.h>

static VALUE t_init(VALUE self)
{
    VALUE a, b;
    a = rb_str_new2("pair a ");
    b = rb_str_new2("pair b ");
    rb_iv_set(self, "@a", a);
    rb_iv_set(self, "@b", b);
    return self;
}


static VALUE a(VALUE self)
{
    VALUE a = rb_iv_get(self, "@a");
    return a;
}

static VALUE b(VALUE self)
{
    VALUE b = rb_iv_get(self, "@b");
    return b;
}

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

    int type = TYPE(result);
    std::cout << "result " << type << std::endl;

    VALUE t = rb_ary_pop(result);
    std::cout << "t = " << TYPE(t) << std::endl;
    ///while (TYPE(t) == T_STRING) {
    for(;;) {
        VALUE t = rb_ary_pop(result);
        std::cout << "t = " << TYPE(t) << std::endl;
        ///std::cout << std::string(StringValuePtr(t)) << std::endl;
        ///break;
    }
    //return std::string(StringValuePtr(result));
    return "";
}

int main()
{
    ruby_init();
    ruby_init_loadpath();

    VALUE test_pair = rb_define_class("TestPair", rb_cObject);
    rb_define_method(test_pair, "initialize", (VALUE(*)(...))t_init, 0);
    rb_define_method(test_pair, "a", (VALUE(*)(...))a, 0);
    rb_define_method(test_pair, "b", (VALUE(*)(...))b, 0);

    std::cout << "Hello, world from C++" << std::endl;
    std::cout << "Ruby says <" << dorb("./test2.rb") << ">" << std::endl;

    ruby_finalize();
}
