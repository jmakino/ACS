#include "ruby.h"


typedef double  real;
#define NDIM  3                        // number of spatial dimensions

static void pairwise_acc_jerk(real massi, real massj,
			      real posi[], real posj[],
			      real veli[], real velj[],
			      real acci[], real accj[],
			      real jerki[], real jerkj[])
{

    int k;
    real rji[NDIM];                        // particle i to particle j
    real vji[NDIM];                        // vji[] = d rji[] / d t
    real r2 = 0;                           // | rji |^2
    real v2 = 0;                           // | vji |^2
    real rv_r2 = 0;                        // ( rij . vij ) / | rji |^2
    real r = sqrt(r2);                     // | rji |
    real r3 = r * r2;                      // | rji |^3
    real da[NDIM];                         // main terms in pairwise
    real dj[NDIM];                         // acceleration and jerk
    real da2 = 0;
    for (k = 0; k < NDIM ; k++){
	rji[k] = posj[k] - posi[k];
	vji[k] = velj[k] - veli[k];
    }
    for (k = 0; k < NDIM ; k++){
	r2 += rji[k] * rji[k];
	v2 += vji[k] * vji[k];
	rv_r2 += rji[k] * vji[k];
    }
    rv_r2 /= r2;
    r = sqrt(r2);
    
    
    // add the {j (i)} contribution to the {i (j)} values of acceleration and jerk:
    r3 = r2*r;
    for (k = 0; k < NDIM ; k++){
	da[k] = rji[k] / r3;                           // see equations
	dj[k] = (vji[k] - 3 * rv_r2 * rji[k]) / r3;    // in the header
    }
    for (k = 0; k < NDIM ; k++){
	acci[k] = massj * da[k];                 // using symmetry
	accj[k] = - massi * da[k];                 // find pairwise
	jerki[k] = massj * dj[k];                // acceleration
	jerkj[k] = -massi * dj[k];                // and jerk
    }
    
}

static void get_pos_vel_and_mass(VALUE p,
				 double * pos,
				 double * vel,
				 double * mass)
{
    int k;
    VALUE local, element;
    local = rb_iv_get(p, "@pos");
    for(k=0;k<3;k++){
	pos[k] = NUM2DBL(rb_ary_entry(local,k));
    }
    local = rb_iv_get(p, "@vel");
    for(k=0;k<3;k++){
	vel[k] = NUM2DBL(rb_ary_entry(local,k));
    }
    *mass = NUM2DBL(rb_iv_get(p, "@mass"));
}

static void inc_acc(VALUE p,   double * da)
{
    int k;
    VALUE local, element;
    local = rb_iv_get(p, "@acctmp");
    for(k=0;k<3;k++){
	rb_ary_store(local,k,rb_float_new(NUM2DBL(rb_ary_entry(local,k))+da[k]));
    }
}
static void inc_jerk(VALUE p,   double * da)
{
    int k;
    VALUE local, element;
    local = rb_iv_get(p, "@jerktmp");
    for(k=0;k<3;k++){
	rb_ary_store(local,k,rb_float_new(NUM2DBL(rb_ary_entry(local,k))+da[k]));
    }
}


static void copy_from_ruby_array(double * v, VALUE p)
{
    int k;
    for(k=0;k<NDIM;k++){
	v[k] = NUM2DBL(rb_ary_entry(p,k));
    }
}
static void copy_to_ruby_array(double * v, VALUE p)
{
    int k;
    for(k=0;k<NDIM;k++){
	rb_ary_store(p,k,rb_float_new(v[k]));
    }
}
    

static VALUE pairwise_force(VALUE self, VALUE pi, VALUE pj, VALUE c)
{

    real massi, massj;
    real posi[3], posj[3], veli[3], velj[3];
    real acci[3], accj[3], jerki[3], jerkj[3];

    get_pos_vel_and_mass(pi, posi, veli, &massi);

    get_pos_vel_and_mass(pj, posj, velj, &massj);

    pairwise_acc_jerk(massi, massj, posi, posj, veli, velj,
		      acci, accj, jerki, jerkj);
    
    inc_acc(pi, acci);
    return Qnil;
}
static VALUE pairwise_jerk(VALUE self, VALUE pi, VALUE pj, VALUE c)
{

    real massi, massj;
    real posi[3], posj[3], veli[3], velj[3];
    real acci[3], accj[3], jerki[3], jerkj[3];

    get_pos_vel_and_mass(pi, posi, veli, &massi);

    get_pos_vel_and_mass(pj, posj, velj, &massj);

    pairwise_acc_jerk(massi, massj, posi, posj, veli, velj,
		      acci, accj, jerki, jerkj);
    
    inc_jerk(pi, jerki);
    return Qnil;
}

	

void Init_pairwise()
{
    rb_define_global_function("pairwise_force", pairwise_force, 3);
    rb_define_global_function("pairwise_jerk", pairwise_jerk, 3);

}

