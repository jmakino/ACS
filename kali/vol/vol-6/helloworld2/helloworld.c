#include "ruby.h"

static VALUE helloworld(VALUE self)
{
    printf("hello, world\n");
    return Qnil;
}

static VALUE helloworld2(VALUE self, VALUE x1, VALUE x2)
{
    int i;
    double d;
    i = FIX2INT(x1);
    d = NUM2DBL(x2);
    printf("hello, world 2 %d %g\n",i,d);
    return Qnil;
}

void Init_helloworld()
{
    rb_define_global_function("helloworld", helloworld, 0);
    rb_define_global_function("helloworld2", helloworld2, 2);
}
