#include "ruby.h"


typedef double  real;
#define NDIM  3                        // number of spatial dimensions


typedef struct{
    real mass;
    real pos[NDIM];
    real vel[NDIM];
    real acc[NDIM];
    real jerk[NDIM];
    real old_pos[NDIM];
    real old_vel[NDIM];
    real old_acc[NDIM];
    real old_jerk[NDIM];
}Cbody, *Cbody_ptr;

static VALUE RCbody;

static real epot;
static real coll_time_q;
static real dt;

static void pairwise_acc_jerk_pot_coll(real massi, real massj,
				real posi[], real posj[],
				real veli[], real velj[],
				real acci[], real accj[],
				real jerki[], real jerkj[],
				real * epot, real * coll_time_q){
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
    real coll_est_q;                           // collision time scale estimate
                                               // to 4th power (quartic)
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
    
    // add the {i,j} contribution to the total potential energy for the system:
    
    *epot -= massi * massj / r;
    
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
    
    // first collision time estimate, based on unaccelerated linear motion:
    
    coll_est_q = (r2*r2) / (v2*v2);
    if (*coll_time_q > coll_est_q)
	*coll_time_q = coll_est_q;
    
    // second collision time estimate, based on free fall:
    
    // da2 becomes the 
    for (k = 0; k < NDIM ; k++)                // square of the 
	da2 += da[k] * da[k];                      // pair-wise accel-
    double mij = massi + massj;                // eration between
    da2 *= mij * mij;                              // particles i and j
    
    coll_est_q = r2/da2;
    if (*coll_time_q > coll_est_q)
	*coll_time_q = coll_est_q;
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

static VALUE hello1(VALUE self, VALUE x1)
{
    int i;
    double d;
    VALUE local, element;
    printf("hello1 called\n");
    if (TYPE(x1) == T_FIXNUM)     printf("x1 is int\n");
    if (TYPE(x1) == T_FLOAT)     printf("x1 is float\n");
    if (TYPE(x1) == T_CLASS)     printf("x1 is class\n");
    if (TYPE(x1) == T_STRUCT)     printf("x1 is struct\n");
    if (TYPE(x1) == T_OBJECT) {
	printf("x1 is object\n");
	local = rb_iv_get(x1, "@mass");
	d = NUM2DBL(local);
	d = d*2;
	local = rb_float_new(d);
	rb_iv_set(x1, "@mass",local);
	printf("mass = %g\n", d);
	local = rb_iv_get(x1, "@pos");
	element = rb_ary_entry(local,1);
	d = NUM2DBL(element);
	printf("x1 = %g\n", d);
	d = d+12345;
	rb_ary_store(local,1, rb_float_new(d));
    }
    return Qnil;
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


static void inc_acc_jerk(VALUE p,
			 double * da,
			 double * dj)
{
    int k;
    VALUE local, element;
    local = rb_iv_get(p, "@acc");
    for(k=0;k<3;k++){
	rb_ary_store(local,k,rb_float_new(NUM2DBL(rb_ary_entry(local,k))+da[k]));
    }
    local = rb_iv_get(p, "@jerk");
    for(k=0;k<3;k++){
	rb_ary_store(local,k,rb_float_new(NUM2DBL(rb_ary_entry(local,k))+dj[k]));
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
    

static VALUE pairwize_force(VALUE self, VALUE pi, VALUE pj, VALUE c)
{

    real massi, massj;
    real posi[3], posj[3], veli[3], velj[3];
    real acci[3], accj[3], jerki[3], jerkj[3];
    real epot,  coll_time_q;


    get_pos_vel_and_mass(pi, posi, veli, &massi);

    get_pos_vel_and_mass(pj, posj, velj, &massj);

    epot = NUM2DBL(rb_cv_get(c, "@@epot"));
    coll_time_q = NUM2DBL(rb_cv_get(c, "@@coll_time_q"));
    pairwise_acc_jerk_pot_coll(massi, massj, posi, posj, veli, velj,
				acci, accj, jerki, jerkj,
			       &epot, &coll_time_q);

    inc_acc_jerk(pi, acci, jerki);
    inc_acc_jerk(pj, accj, jerkj);
    
    rb_cv_set(c, "@@epot",rb_float_new(epot));
    rb_cv_set(c, "@@coll_time_q",rb_float_new(coll_time_q));

    return Qnil;
}

static void Cbody_free(Cbody_ptr p)
{
    free(p);
}

static VALUE Cbody_new(VALUE self)
{
    VALUE obj, vpath, vmode;
    Cbody_ptr bp;

    obj = Data_Make_Struct(RCbody, Cbody, NULL, Cbody_free, bp);
    return obj;
}

void Cbody_set_mass(Cbody_ptr b, real m)
{
    b->mass = m;
    printf("mass set = %g\n",b->mass);
}


real Cbody_get_mass(Cbody_ptr b)
{
    return b->mass;
}

static VALUE RCbody_set_mass(VALUE self, VALUE x)
{
    real d;
    Cbody_ptr p;
    d = NUM2DBL(x);
    Data_Get_Struct(self, Cbody, p);
    Cbody_set_mass(p,d);
    return Qnil;
}

static VALUE RCbody_get_mass(VALUE self)
{
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    return rb_float_new(Cbody_get_mass(p));
}

static VALUE RCbody_set_pos_vel_and_mass(VALUE self, VALUE rp)
{
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    get_pos_vel_and_mass(rp,p->pos, p->vel, &(p->mass));
    return Qnil;
}

static VALUE RCbody_get_acc_jerk(VALUE self, VALUE rp)
     
{
    int k;
    VALUE local, element;
    real * acci;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    copy_to_ruby_array(p->acc,rb_iv_get(rp, "@acc"));
    copy_to_ruby_array(p->jerk,rb_iv_get(rp, "@jerk"));
}

static VALUE RCbody_set_acc_jerk(VALUE self, VALUE rp)
     
{
    int k;
    VALUE local, element;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    copy_from_ruby_array(p->acc,rb_iv_get(rp, "@acc"));
    copy_from_ruby_array(p->jerk,rb_iv_get(rp, "@jerk"));

}

static VALUE RCbody_clear_acc_and_jerk(VALUE self)

{
    int k;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    for(k=0;k<3;k++){
	p->acc[k]=0.0;
	p->jerk[k]=0.0;
    }
}


static VALUE Cbody_set_epot_and_collq(VALUE self,VALUE c)
{
    epot = NUM2DBL(rb_cv_get(c, "@@epot"));
    coll_time_q = NUM2DBL(rb_cv_get(c, "@@coll_time_q"));
    return Qnil;
}

static VALUE Cbody_get_epot_and_collq(VALUE self,VALUE c)
{    
    rb_cv_set(c, "@@epot",rb_float_new(epot));
    rb_cv_set(c, "@@coll_time_q",rb_float_new(coll_time_q));
    return Qnil;
}


static void Cbody_pairwize_force(Cbody_ptr p1, Cbody_ptr p2)
{

    int k;
    real acci[3], accj[3], jerki[3], jerkj[3];
    pairwise_acc_jerk_pot_coll(p1->mass, p2->mass,
			       p1->pos, p2->pos,
			       p1->vel, p2->vel,
			       acci, accj, jerki, jerkj,
			       &epot, &coll_time_q);
    for(k=0;k<NDIM;k++){
	p1->acc[k] += acci[k];
	p1->jerk[k] += jerki[k];
	p2->acc[k] += accj[k];
	p2->jerk[k] += jerkj[k];
    }
}


static VALUE RCbody_pairwize_force(VALUE rp1, VALUE rp2)
{
    real * acci;
    Cbody_ptr p1;
    Cbody_ptr p2;
    Data_Get_Struct(rp1, Cbody, p1);
    Data_Get_Struct(rp2, Cbody, p2);
    Cbody_pairwize_force(p1, p2);
    return Qnil;
}

static VALUE RCbody_copy_to_C(VALUE self, VALUE rp)
{
    int k;
    VALUE local, element;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    copy_from_ruby_array(p->pos,rb_iv_get(rp, "@pos"));
    copy_from_ruby_array(p->vel,rb_iv_get(rp, "@vel"));
    copy_from_ruby_array(p->acc,rb_iv_get(rp, "@acc"));
    copy_from_ruby_array(p->jerk,rb_iv_get(rp, "@jerk"));
    p->mass = NUM2DBL(rb_iv_get(rp, "@mass"));
    for (k=0;k<NDIM;k++){
	p->old_pos[k] = p->pos[k];
	p->old_vel[k] = p->vel[k];
	p->old_acc[k] = p->acc[k];
	p->old_jerk[k]= p->jerk[k];
    }
}
    
static VALUE RCbody_copy_from_C(VALUE self, VALUE rp)
{
    int k;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    copy_to_ruby_array(p->pos,rb_iv_get(rp, "@pos"));
    copy_to_ruby_array(p->vel,rb_iv_get(rp, "@vel"));
    copy_to_ruby_array(p->acc,rb_iv_get(rp, "@acc"));
    copy_to_ruby_array(p->jerk,rb_iv_get(rp, "@jerk"));
}
    
static VALUE set_dt(VALUE self, VALUE rdt)
{
    dt = NUM2DBL(rdt);
    return Qnil;
}

static VALUE predict(VALUE self)
{
    int k;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    for(k=0;k<NDIM;k++){
	p->pos[k] = p->pos[k] + p->vel[k]*dt +
	    p->acc[k]*dt*dt/2 + p->jerk[k]*dt*dt*dt/6;
	p->vel[k] = p->vel[k] + p->acc[k]*dt +
	    p->jerk[k]*dt*dt/2;
    }
    return Qnil;
}
static VALUE correct(VALUE self)
{
    int k;
    Cbody_ptr p;
    Data_Get_Struct(self, Cbody, p);
    for(k=0;k<NDIM;k++){
	p->vel[k] = p->old_vel[k] +
	    (p->old_acc[k] + p->acc[k])*dt/2 +
	    (p->old_jerk[k] - p->jerk[k])*dt*dt/12;
	p->pos[k] = p->old_pos[k] + 
	    (p->old_vel[k] + p->vel[k])*dt/2 +
	    (p->old_acc[k] - p->acc[k])*dt*dt/12;
    }
    return Qnil;
}

	

void Init_pairwize()
{
    rb_define_global_function("say_hello", say_hello, 0);
    rb_define_global_function("hello1", hello1, 1);
    rb_define_global_function("hello2", hello2, 2);
    rb_define_global_function("pairwize_force", pairwize_force, 3);
    RCbody = rb_define_class("Cbody", rb_cObject);
    rb_define_singleton_method(RCbody, "new", Cbody_new, 0);
    rb_define_method(RCbody, "set_mass", RCbody_set_mass, 1);
    rb_define_method(RCbody, "get_mass", RCbody_get_mass, 0);
    rb_define_method(RCbody, "set_pos_vel_and_mass",
		     RCbody_set_pos_vel_and_mass, 1);
    rb_define_method(RCbody, "set_acc_jerk",
		     RCbody_set_acc_jerk, 1);
    rb_define_method(RCbody, "get_acc_jerk",
		     RCbody_get_acc_jerk, 1);
    rb_define_method(RCbody, "set_epot_and_collq",
		     Cbody_set_epot_and_collq, 1);
    rb_define_method(RCbody, "get_epot_and_collq",
		     Cbody_get_epot_and_collq, 1);
    rb_define_method(RCbody, "pairwize_force",
		     RCbody_pairwize_force, 1);
    rb_define_method(RCbody, "copy_to_c",
		     RCbody_copy_to_C, 1);
    rb_define_method(RCbody, "copy_from_c",
		     RCbody_copy_from_C, 1);
    rb_define_method(RCbody, "clear_acc_and_jerk",
		     RCbody_clear_acc_and_jerk, 0);
    rb_define_method(RCbody, "set_dt", set_dt, 1);
    rb_define_method(RCbody, "predict", predict, 0);
    rb_define_method(RCbody, "correct", correct, 0);

}

