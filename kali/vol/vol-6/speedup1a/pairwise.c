#include "ruby.h"

typedef double  real;
#define NDIMMAX  3

static void pair_acc(real posi[],
		     real posj[],
		     real massi,
		     real massj,
		     real eps,
		     real acci[],
		     real accj[],
		     int ndim)
{
    
    int k;
    real rji[NDIMMAX];
    real r2 = 0;   
    real mir3inv,mjr3inv, r3inv;
    for (k = 0; k < ndim ; k++){
	rji[k] = posj[k] - posi[k];
    }
    r2 = eps*eps;
    for (k = 0; k < ndim ; k++){
	r2 += rji[k] * rji[k];
    }
    r3inv = 1.0/(r2*sqrt(r2));
    mjr3inv = massj*r3inv;
    mir3inv = massi*r3inv;
    for (k = 0; k < ndim ; k++){
	acci[k] += rji[k]*mjr3inv;
	accj[k] -= rji[k]*mir3inv;
    }
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
				   VALUE ruby_acci,
				   VALUE ruby_accj,
				   VALUE ruby_posi,
				   VALUE ruby_posj,
				   VALUE ruby_massi,
				   VALUE ruby_massj,
				   VALUE ruby_eps,
				   VALUE ruby_ndim)
     

{

    real posi[3], posj[3];
    real acci[3], accj[3];
    real massi=NUM2DBL(ruby_massi);
    real massj=NUM2DBL(ruby_massj);
    real eps =  NUM2DBL(ruby_eps);
    int ndim = FIX2INT(ruby_ndim);
    copy_from_ruby_array(posi, ruby_posi,ndim);
    copy_from_ruby_array(posj, ruby_posj,ndim);
    copy_from_ruby_array(acci, ruby_acci,ndim);
    copy_from_ruby_array(accj, ruby_accj,ndim);
    pair_acc(posi, posj,massi,massj, eps,acci,accj,ndim);
    copy_to_ruby_array(acci, ruby_acci, ndim);
    copy_to_ruby_array(accj, ruby_accj, ndim);
    return Qnil;
}

	

void Init_pairwise()
{
    rb_define_global_function("pairwise_acceleration",
			      pairwise_acceleration, 8);
}
