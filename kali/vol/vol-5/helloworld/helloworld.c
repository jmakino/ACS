#include "ruby.h"

static VALUE helloworld(VALUE self)
{
    printf("hello, world\n");
    return Qnil;
}
void Init_helloworld()
{
    rb_define_global_function("helloworld", helloworld, 0);
}
