#include "ruby.h"

typedef double  real;
#define NDIMMAX  3

static void pair_acc(real posi[],
		     real posj[],
		     real massj,
		     real eps,
		     real acc[],
		     int ndim)
{
    
    int k;
    real rji[NDIMMAX];
    real r2 = 0;   
    real mr3inv;
    for (k = 0; k < ndim ; k++){
	rji[k] = posj[k] - posi[k];

    }
    r2 = eps*eps;
    for (k = 0; k < ndim ; k++){
	r2 += rji[k] * rji[k];
    }
    mr3inv = massj/(r2*sqrt(r2));
    for (k = 0; k < ndim ; k++){
	acc[k] += rji[k]*mr3inv;
    }
}


static VALUE say_hello(VALUE self)
{
    printf("hello, world\n");
    return Qnil;
}
static VALUE hello2(VALUE self, VALUE x1, VALUE x2)
{
    int i;
    double d;
    i = FIX2INT(x1);
    d = NUM2DBL(x2);
    printf("hello, world 2 %d %g\n",i,d);
    return Qnil;
}

static void copy_from_ruby_array(double * v, VALUE p, int ndim)
{
    int k;
    for(k=0;k<ndim;k++){
	v[k] = NUM2DBL(rb_ary_entry(p,k));
    }
}
static void copy_to_ruby_array(double * v, VALUE p, int ndim)
{
    int k;
    for(k=0;k<ndim;k++){
	rb_ary_store(p,k,rb_float_new(v[k]));
    }
}
    

static VALUE pairwise_acceleration(VALUE self,
				   VALUE ruby_posi,
				   VALUE ruby_posj,
				   VALUE ruby_massj,
				   VALUE ruby_eps,
				   VALUE ruby_acc,
				   VALUE ruby_ndim)
     

{

    real posi[3], posj[3];
    real acc[3];
    real massj=NUM2DBL(ruby_massj);
    real eps =  NUM2DBL(ruby_eps);
    int ndim = FIX2INT(ruby_ndim);
    copy_from_ruby_array(posi, ruby_posi,ndim);
    copy_from_ruby_array(posj, ruby_posj,ndim);
    copy_from_ruby_array(acc, ruby_acc,ndim);

    pair_acc(posi, posj,massj, eps,acc,ndim);
    
    copy_to_ruby_array(acc, ruby_acc, ndim);
    return Qnil;
}

	

void Init_pairwise()
{
    rb_define_global_function("say_hello", say_hello, 0);
    rb_define_global_function("hello2", hello2, 2);
    rb_define_global_function("pairwise_acceleration",
			      pairwise_acceleration, 6);
}
