/*
 * nbody.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double sqrt();

#define NDIM 3
#define PMAX 10240
#define NMAX 1024
typedef struct parmstruct{
    double eps2;
    double dtmax;
    double dtcoef;
    double tend;
    double tblock;
    int niter;
} PARMS, *PPARMS;

static struct parmstruct pblock;

typedef struct vect{
    double x;
    double y;
    double z;
}VECT,*PVECT;

typedef struct particle{
    double t;
    double dtreal;
    double phi;
    double maxvr2;
    VECT x;
    VECT v;
    VECT a;
    VECT jerk;
}PARTICLE, *PPARTICLE;

typedef struct pline{
    double mass;
    int index;
    int np;
    int np0;
    int plast;
    PARTICLE p[PMAX];
    PARTICLE p0[PMAX];
}PLINE, *PPLINE;

void printv(VECT x, char* s)
{
    printf("%s = %e %e %e\n", s, x.x, x.y, x.z);
}

void zerov(PVECT x)
{
    x->x = x->y = x->z = 0;
}
VECT add_with_scale(VECT x, VECT dx, double s)
{
    VECT res;
    res.x = x.x+dx.x*s;
    res.y = x.y+dx.y*s;
    res.z = x.z+dx.z*s;
    return res;
}

void inc_with_scale(PVECT x, VECT dx, double s)
{
    VECT res;
    x->x +=dx.x*s;
    x->y +=dx.y*s;
    x->z +=dx.z*s;
}

VECT mulvect(VECT x, double s)
{
    VECT res;
    res.x = x.x*s;
    res.y = x.y*s;
    res.z = x.z*s;
    return res;

}
VECT addvect(VECT x, VECT y)
{
    VECT res;
    res.x = x.x+y.x;
    res.y = x.y+y.y;
    res.z = x.z+y.z;
    return res;
}

VECT subvect(VECT x, VECT y)
{
    VECT res;
    res.x = x.x-y.x;
    res.y = x.y-y.y;
    res.z = x.z-y.z;
    return res;
}

double sqv(VECT x)
{
    return x.x*x.x+x.y*x.y+x.z*x.z;
}

double innerp(VECT x, VECT y)
{
    return x.x*y.x+x.y*y.y+x.z*y.z;
}

void dump_vector(FILE * outsnapfile, char* s, PVECT v)
{
    fprintf(outsnapfile,"%s=%.12g %.12g %.12g\n", s,  v->x, v->y, v->z);
}

void accum_gravity(VECT r,
		   VECT v,
		   double mass,
		   double eps2,
		   PVECT acc,
		   PVECT jerk,
		   double * phi,
		   double *maxvr2)
{
    VECT a;
    double r2 = sqv(r)+ eps2;
    double r2inv = 1.0/r2;
    double a3 = -3.0 * innerp(r,v) * r2inv;
    double mrinv = mass * sqrt(r2inv);
    double mr3inv;
    *phi -= mrinv;
    mr3inv = mrinv * r2inv;
    inc_with_scale(acc, r, mr3inv);
    inc_with_scale(jerk, addvect(v, mulvect(r, a3)), mr3inv);
    double vr2 = sqv(v)/sqv(r);
    if (vr2 > *maxvr2) *maxvr2 = vr2;
}




void dump_particle(FILE * outsnapfile, PPARTICLE p, int id)
{
    int i;
    fprintf(outsnapfile,"    id=%d, t, dtreal= %g %g\n",
	    id, p->t, p->dtreal);
    dump_vector(outsnapfile, "    pos ", &(p->x));
    dump_vector(outsnapfile, "    vel ", &(p->v));
    dump_vector(outsnapfile, "    acc ", &(p->a));
    dump_vector(outsnapfile, "    jerk", &(p->jerk));
    fprintf(outsnapfile,"    phi=%g\n", p->phi);
}
void dump_ppline(FILE * outsnapfile, PPLINE pl)
{
    int i;
    fprintf(outsnapfile,"ID=%d mass=%g\n", pl->index, pl->mass);
    fprintf(outsnapfile,"List of particles:\n");

    for(i=0;i<=pl->np;i++) dump_particle(outsnapfile, pl->p+i,i);
    fprintf(outsnapfile,"List of old particles:\n");
    for(i=0;i<=pl->np0;i++) dump_particle(outsnapfile, pl->p0+i,i);
}

void dump_system(FILE * outsnapfile, PPLINE  pl, int  n)
{
    int ndim,i;
    fprintf(outsnapfile, "N=%d\n", n);
    for(i=0;i<n;i++)
    for(i=0;i<n;i++){
	dump_ppline(outsnapfile, pl+i);
    }
}

PPLINE select_tpmin(PPLINE pl, int n)
{
    double tmin = 1e100;
    PPLINE p;
    PPLINE pmin;
    int i;
    for(i =0, p = pl; i< n; i++,p++){
	if (p->p[p->np].t < tmin){
	    tmin = p->p[p->np].t;
	    pmin = p;
	    //	    fprintf(stderr,"select_tpin, i, tmin = %d %g\n", i,  tmin);
	}
    }
    return pmin;
}

void error_and_exit(char * s)
{
    fprintf(stderr,"%s",s);
    exit(-1);
}
double interporated_dt(PPLINE p, double time)
{
    int i = p->plast;
    PPARTICLE p0;
    PPARTICLE p1;
    if (i < 0) i = 0;
    if (i >= p->np0-1) i = p->np0-2;
    while (p->p0[i].t > time) {
	i--;
	if (i < 0) error_and_exit("interporation failed");
    }
    while (p->p0[i+2].t < time){
	i++;
	if (i > p->np0-2) error_and_exit("interporation failed");
    }
    p0 = &(p->p0[i]);
    p1 = &(p->p0[i+1]);
    return p0->dtreal+(time-p0->t)*(p1->dtreal-p0->dtreal)/(p1->t - p0->t);
}
double predicted_dt(PPLINE p, double time)
{
    PPARTICLE pn = &(p->p[p->np]);
    PPARTICLE pp = &(p->p[p->np-1]);
    return pn->dtreal+(time-pn->t)*(pn->dtreal-pp->dtreal)/(pn->t - pp->t);
}

double forceddt(double dt, double time)
{
    int ex;
    double dttmp;
    frexp(dt,&ex);
    //    fprintf(stderr, "forceddt, %g->%g\n", dt,ldexp(0.5,ex));
    dttmp= ldexp(0.5,ex);
    while (fmod(time, dttmp) != 0.0) dttmp*= 0.5;
    return dttmp;
}
double dt(VECT v,
	  double phi,
	  VECT a,
	  VECT jerk,
	  double maxvr2)
{
    //    double rate1 = sqrt(sqv(v)/sqv(jerk));
    //    double rate2 = (sqv(v)/sqv(a));
    //    double rate3 = (sqv(a)/sqv(jerk));
    //    double dt = 1.0/sqrt(rate1+rate2+rate3);
    double dt = 1e10;
    if (dt > 1.0/sqrt(maxvr2)) dt = 1.0/sqrt(maxvr2);
    dt *= pblock.dtcoef;
    if (dt > pblock.dtmax) dt = pblock.dtmax;
    return dt;
}

double dtadjust(double dt, double time, PPLINE p)
{
    int i;
    if (p->np0 < 2) return dt;
    if (p->p0[p->np0-1].t< dt+time) return dt;
    for(i=0;i<p->np0;i++){
	if (fabs(p->p0[i].t - (dt+time)) < dt*0.1){
	    if (p->p0[i].dtreal < dt)return dtadjust(dt*0.5, time, p);
	    return dt;
	}else if (p->p0[i].t > dt+time) {
	    return dt;
	}
    }
}


void correct(PVECT xnew,
	     PVECT vnew,
	     VECT xold,
	     VECT vold,
	     VECT anew,
	     VECT aold,
	     double dt)
{
    *vnew = add_with_scale(vold, addvect(anew, aold), dt*0.5);
    if (pblock.niter > 1){
	*xnew = add_with_scale(xold, addvect(*vnew, vold), dt*0.5);
    }
}

void integration_epilogue(PPLINE p)
{
    /* things to do:
       apply corrector
       update the estimate for the current timestep
       predict next timestep
       determine next time
       other bookkeeping?
    */
    PPARTICLE pn = &(p->p[p->np]);
    PPARTICLE pp = &(p->p[p->np-1]);
    double dtprev;
    double dttmp;
    correct(&(pn->x), &(pn->v), pp->x, pp->v, pn->a, pp->a, pn->t - pp->t);
    pn-> dtreal = dt(pn->v, pn->phi, pn->a, pn->jerk, pn->maxvr2);
    dttmp = forceddt(pn->dtreal, pn->t);
    dttmp=dtadjust(dttmp, pn->t, p);
    (pn+1)-> t = pn->t + dttmp;
    p->np++;
}


void direct_predict_pos_and_vel(PVECT r, PVECT v, PPLINE p, double tnext)
{
    PPARTICLE pp = &(p->p[p->np-1]);
    double t0 = pp->t;
    VECT x = pp->x;
    VECT v0 = pp->v;
    VECT a = pp->a;
    double dt = tnext - t0;
    if (dt < 0){
	fprintf(stderr," t=%g,  tnext=%g, dt=%g\n", t0, tnext, dt);
	error_and_exit("Negative dt at direct prediction\n");
    }
    *r= add_with_scale(add_with_scale(x, v0, dt), a, 0.5*dt*dt);
    *v= add_with_scale(v0, a, dt);
}

VECT linear_interpolation(VECT x0, VECT x1, double f)
{
    return addvect(mulvect(x0, 1.0-f), mulvect(x1, f));
}

void interpolate_old_data(PVECT r, PVECT v, PPLINE p, double tnext)
{
    int i;
    PPARTICLE pp;
    PPARTICLE ppn;
    pp= &(p->p0[0]);
    i=1;
    while ((pp+i)-> t < tnext) i++;
    pp = pp+(i-1);
    ppn = pp+1;
    double dt0= ppn-> t - pp-> t;
    double dt = tnext -  pp-> t;
    *r= linear_interpolation(pp->x, ppn->x, dt/dt0);
    *v= linear_interpolation(pp->v, ppn->v, dt/dt0);
    //    printf("Interpolator f=%g\n", dt/dt0);
    //    dump_vector(stdout," pp->x",&(pp->x));
    //    dump_vector(stdout," ppn->x",&(ppn->x));
    //    dump_vector(stdout," *r",r);
    
}

void find_common_time(int* inew, int* iold, PPARTICLE pnew, PPARTICLE pold)
{
    int i;
    //
    // removing the following if block
    // somehow caused gcc 3.3.2 failed to make correct code with -O4
    //
    if (0){
	for(i=0;i<= *inew; i++)printf("New t[%d]=%g\n", i, (pnew+i)->t);
	for(i=0;i<= *iold; i++)printf("Old t[%d]=%g\n", i, (pold+i)->t);
    }
    while (((pnew+*inew)->t) != ((pold+*iold)->t)){
	//	printf("new, old ids =  %d %d\n", *inew, *iold);
	while (((pnew+*inew)->t) > ((pold+*iold)->t)){
	    (*inew)--;
	    if (*inew < 0)error_and_exit("find common time failed");
	}
	while (((pnew+*inew)->t) < ((pold+*iold)->t)) {
	    (*iold)--;
	    if (*iold < 0)error_and_exit("find common time failed");
	}
    }
}

void apply_collection_to_interpolation(PVECT r,
				       PVECT v,
				       PPLINE p,
				       double tnext)
{
    int new, old;
    new = p->np-1;
    old = p->np0-1;
    find_common_time(&new, &old, &(p->p[0]),&(p->p0[0]));
    *r = addvect(*r, subvect(p->p[new].x, p->p0[old].x));
    *v = addvect(*v, subvect(p->p[new].v, p->p0[old].v));
    //    dump_vector(stdout," adjusted r",r);
}
void interpolate_predict_pos_and_vel(PVECT r, PVECT v, PPLINE p, double tnext)
{
    interpolate_old_data(r,v,p,tnext);
    apply_collection_to_interpolation(r,v,p,tnext);
}
void predict_pos_and_vel(PVECT r, PVECT v, PPLINE p, double tnext)
{
    if (pblock.niter == 1){
	direct_predict_pos_and_vel(r, v, p, tnext);
	return;
    }
    if(p->np0 <1){
	direct_predict_pos_and_vel(r, v, p, tnext);
    }else if (p->p0[p->np0-1].t < tnext){
	direct_predict_pos_and_vel(r, v, p, tnext);
    }else{
	interpolate_predict_pos_and_vel(r, v, p, tnext);
    }
}

VECT integrate_position(VECT x,
			VECT v,
			VECT a,
			double dt)
{
    return add_with_scale(x, addvect(v, mulvect(a, dt*0.5)), dt);
}

void calculate_force(PPLINE pl,
		     int n,
		     VECT x,
		     VECT v,
		     int index,
		     double tnext,
		     VECT * acc,
		     VECT * jerk,
		     double * phi,
		     double *maxvr2)
{
    PPLINE p;
    int i;
    zerov(acc);
    zerov(jerk);
    *phi=0;
    *maxvr2=0;
    for(p=pl, i=0; i< n; i++,p++){
	if (index  != p->index){
	    VECT xj;
	    VECT vj;
	    predict_pos_and_vel(&xj, &vj, p, tnext);
	    accum_gravity(subvect(xj,x), subvect(vj,v), p->mass,
			  pblock.eps2, acc,jerk, phi,maxvr2);
	    //	    VECT xx = subvect(xj,x);
	    //	    dump_vector(stderr," r0",&x);
	    //	    dump_vector(stderr," r1",&xj);
	    //	    dump_vector(stderr," r",&xx);
	    //	    dump_vector(stderr," a",acc);
	}
    }
}

double potential_energy(int  n, PPLINE  pl, int mode)
/* mode=0 t=tstart
   mode=1 t=tend (at np-1)
   else: undefined
*/
{
    double psum = 0;
    int i;
    for(i=0;i<n;i++){
	PPARTICLE p;
	if (mode == 0){
	    p = (pl+i)->p;
	}else if  (mode == 1){
	    p = (pl+i)->p+((pl+i)->np-1);
	}
	psum += (pl+i)->mass*p->phi;
    }
    return psum*0.5;
}

double kinetic_energy(int  n, PPLINE  pl, int mode)
/* mode=0 t=tstart
   mode=1 t=tend (at np-1)
   else: undefined
*/
{
    double ksum = 0;
    int i;
    for(i=0;i<n;i++){
	PPARTICLE p;
	if (mode == 0){
	    p = (pl+i)->p;
	}else if  (mode == 1){
	    p = (pl+i)->p+((pl+i)->np-1);
	}
	ksum += (pl+i)->mass*sqv(p->v);
    }
    return ksum*0.5;
}

VECT cmpos(int n, PPLINE pl)
{
    VECT sum;
    double msum;
    int i;
    zerov(&sum);
    msum = 0;
    for(i=0;i<n;i++){
	sum = addvect(sum, mulvect((pl+i)->p[0].x,(pl+i)->mass) );
	msum += (pl+i)->mass;
    }
    return mulvect(sum, 1.0/msum);
}
VECT cmvel(int n, PPLINE pl)
{
    VECT sum;
    double msum;
    int i;
    zerov(&sum);
    msum = 0;
    for(i=0;i<n;i++){
	sum = addvect(sum, mulvect((pl+i)->p[0].v,(pl+i)->mass));
	msum += (pl+i)->mass;
    }
    return mulvect(sum, 1.0/msum);
}

void print_log(int n, PPLINE pl, int set_e0)
{
    static double e0;
    double e;
    double time = pl->p[pl->np-1].t;
    double ke=kinetic_energy(n,  pl, 1);
    double pe=potential_energy(n,  pl, 1);
    e = ke+pe;
    fprintf(stdout,"Time=% g energies= %g %g %.16g",
	    time, ke, pe, e);
    if (set_e0){
	e0=e;
	fprintf(stdout,"\n");
    }else{
	fprintf(stdout," de= %g\n",(e-e0)/e0);
    }
    VECT cmp =cmpos(n,pl);
    VECT cmv =cmvel(n,pl);
    fprintf(stdout,"CM = %g %g %g %g %g %g\n",
	    cmp.x, cmp.y, cmp.z, 
	    cmv.x, cmv.y, cmv.z);
}

void integrate_particle(PPLINE p, PPLINE pl, int n)
{
    int np = p->np;
    PPARTICLE pc;
    PPARTICLE pn;
    pc = &(p->p[np-1]);
    pn = &(p->p[np]);
    //    dump_ppline(stdout,p);
    //    printf("t, tnext = %g,%g\n", pc->t, pn->t);
    predict_pos_and_vel(&(pn->x), &(pn->v), p, pn->t);
    //    dump_vector(stdout,"pred pos", &(pn->x));

    calculate_force(pl, n, pn->x,pn->v, p->index, pn->t,
		    &(pn->a), &(pn->jerk), &(pn->phi),&(pn->maxvr2));
    //    dump_vector(stdout,"new a", &(pn->a));
    integration_epilogue(p);
}

void reset_particle_at_tstart(PPLINE p,  double tstart)
{
    p->np=1;
}

void reset_particles_at_tstart(PPLINE pl, int n, double tstart)
{
    int i;
    for(i=0;i<n;i++) reset_particle_at_tstart(pl+i, tstart);
}

void backup_particle_history(PPLINE p)
{
    int np=p->np;
    int i;
    for(i=0;i<=np; i++)p->p0[i]=p->p[i];
    p->np0=np;
    p->plast=0;
}

void backup_system_history(PPLINE pl, int n)
{
    int i;
    for(i=0;i<n;i++) backup_particle_history(pl+i);
}

void cut_particle_history(PPLINE p)
{
    int np=p->np;
    int i;
    //    printf("Index=%d, np=%d\n", p->index, p->np);
    p->plast=0;
    p->p[0] = p->p[np-1];
    p->p[1] = p->p[np];
    p->np=1;
}

void cut_system_history(PPLINE pl, int n)
{
    int i;
    for(i=0;i<n;i++) cut_particle_history(pl+i);
}
void go_back_in_history(PPLINE pl, int n)
{
    int i;
    for(i=0;i<n;i++) {
	(pl+i)->np=1;
    }
	
}
void cut_system_old_history(PPLINE pl, int n)
{
    int i;
    for(i=0;i<n;i++) {
	(pl+i)->np0=0;
    }
}


void trial_integrate_system(PPLINE pl, int n, double tstart, double tend)
{
    PPLINE p;
    int i;
    for(i=0;i<n;i++){
	p=pl+i;
	double dt =p->p[1].t -p->p[0].t;
	dt = dtadjust(dt, p->p[0].t, p);
	p->p[1].t =p->p[0].t+dt;
    }
    p = select_tpmin(pl, n);
    double tsys = tstart;
    //    fprintf(stderr, "selected p = %d\n", p->index);
    //    reset_particles_at_tstart(pl,n,tstart);
    //    fprintf(stderr, "End reset_particles\n");
    while (tsys <= tend){
	//    fprintf(stderr, "Enter integrate\n");
	integrate_particle(p, pl, n);
	p =  select_tpmin(pl, n);
	//	fprintf(stderr, "selected p = %d\n", p->index);
	tsys = p->p[p->np].t;
	
    }
}

void integrate_system(PPLINE pl, int n, double tend)
{
    double tstart = pl->p[0].t;
    double tsys = tstart;
    while (tsys < tend){
	int i;
	tsys = tstart + pblock.tblock;
	for(i=0; i<pblock.niter; i++){
	    //    fprintf(stdout, "niter = %d iiter=%d\n", pblock.niter, i);
	    //	    fprintf(stdout, "integrate_system, itertation #%d tstart tend = %e %e\n", i, tstart, tsys);
	    go_back_in_history(pl, n);
	    //	    dump_system(stdout, pl, n);
	    trial_integrate_system(pl, n, tstart, tsys);
	    //	    printf("Before backup\n");
	    //	    dump_system(stdout, pl, n);
	    backup_system_history(pl, n);
	    //	    printf("After backup and cut\n");
	    //dump_system(stdout, pl, n);
	    //	    fprintf(stdout, "end loop: niter = %d iiter=%d\n", pblock.niter, i);
	}
	//	fprintf(stdout,"Exit from loop %d\n",i);
	tstart = tsys;
	//	dump_system(stdout, pl, n);
	cut_system_history(pl, n);
	cut_system_old_history(pl, n);
	print_log(n,pl,0);
	//	dump_system(stdout, pl, n);
    }
}

void initialize_particle_phase0(PPLINE p, double tstart)
{
    // assumption: index, mass, position and velocity are set. Nothing else
    p->np=1;
    p->np0=0;
    p->plast=-1;
}

void set_physical_data_to_particle(PPLINE p,
				   double mass,
				   VECT pos,
				   VECT vel,
				   int index)
{
    p->mass=mass;
    p->index=index;
    p->p[0].x = pos;
    p->p[0].v = vel;
}

void set_initial_force_and_timestep(PPLINE pl, int n, PPLINE p, double tstart)
{
    VECT acc, jerk;
    double phi;
    double maxvr2;
    PPARTICLE pp = &(p->p[0]);
    VECT r=pp->x;
    VECT v=pp->v;
    int index = p->index;
    calculate_force(pl, n,  r,v,index, tstart, &acc, &jerk, &phi, &maxvr2);
    pp->a=acc;
    pp->jerk=jerk;
    pp->phi=phi;
    pp->maxvr2 = maxvr2;
    pp->dtreal=dt(pp->v, pp->phi, pp->a, pp->jerk, pp->maxvr2);
    (pp+1)->t=pp->t + forceddt(pp->dtreal,pp->t);
}

void readin_stoa_snapshot(FILE * insnapfile,
			  PPLINE * pl,
			  int * n,
			  double* tstart)
{
    int ndim,i;
    fprintf(stderr,"Enter readin\n");
    fscanf(insnapfile, "%d", n);
    fscanf(insnapfile, "%d", &ndim);
    if (ndim != 3){
	error_and_exit("Input snapshot file format error: ndim <> 3");
    }
    fscanf(insnapfile,"%lf", tstart);
    fprintf(stdout,"n= %d, tstart=%le\n", *n, *tstart);
    //    fprintf(stdout,"mem to allocate = %d\n", *n*sizeof(PLINE));
    *pl = calloc(*n, sizeof(PLINE));
    for(i=0;i<*n;i++)(*pl+i)->index = i;
    for(i=0;i<*n;i++)fscanf(insnapfile,"%lf", &((*pl+i)->mass));
    for(i=0;i<*n;i++){
	double* p = &((*pl)[i].p[0].x.x);
	fscanf(insnapfile,"%lf%lf%lf", p,p+1,p+2);
    }
    for(i=0;i<*n;i++){
	double* p = &((*pl)[i].p[0].v.x);
	fscanf(insnapfile,"%lf%lf%lf", p,p+1,p+2);
    }
}

void write_stoa_snapshot(FILE * outsnapfile,
			 PPLINE  pl,
			 int  n)

{
    int ndim,i;
    fprintf(outsnapfile, "START STOA\n");
    fprintf(outsnapfile, "%d\n", n);
    fprintf(outsnapfile, "%d\n", 3);
    fprintf(outsnapfile,"%21.18e\n", pl[0].p[0].t);
    for(i=0;i<n;i++)fprintf(outsnapfile,"%21.18e\n", ((pl+i)->mass));
    for(i=0;i<n;i++){
	double* p = &(pl[i].p[0].x.x);
	int k;
	for(k=0;k<3;k++)fprintf(outsnapfile," %21.18e", *(p+k));
	fprintf(outsnapfile,"\n");
    }
    for(i=0;i<n;i++){
	double* p = &(pl[i].p[0].v.x);
	int k;
	for(k=0;k<3;k++)fprintf(outsnapfile," %21.18e", *(p+k));
	fprintf(outsnapfile,"\n");
    }
}


void initialize_system(PPLINE pl, int n, double tstart)
{
    int i;
    for(i=0;i<n;i++) initialize_particle_phase0(pl+i, tstart);
    for(i=0;i<n;i++) {
	set_initial_force_and_timestep(pl, n,  pl+i, tstart);
    }
}


#define MAXCHARS 512

void readin_system(int * n, PPLINE * pl, double * tstart)
{
    char strbuf[MAXCHARS];
    char fname[MAXCHARS];
    FILE * insnapfile;
    fprintf(stderr,"Enter input snapshot file name:");
    fgets(strbuf, MAXCHARS, stdin);
    sscanf(strbuf,"%s",fname);
    fprintf(stdout,"input snapshot file = \"%s\"\n", fname);
    insnapfile=fopen(fname,"r");
    if (insnapfile == NULL){
	error_and_exit("failed to open input snapshot file\n");
    }
    readin_stoa_snapshot(insnapfile, pl, n, tstart);
}

void readin_parameters(PPARMS ppb)
{
    double eps;
    fprintf(stderr,"Enter eps, dtmax, dtcoef, tend, tblock, niteration:");
    fscanf(stdin,"%lf%lf%lf%lf%lf%d",&eps,&(ppb->dtmax),
	   &(ppb->dtcoef),&(ppb->tend),&(ppb->tblock),
	   &(ppb->niter));
    ppb->eps2 = eps*eps;
    fprintf(stdout,"eps = %g, eps2=%g\n", eps, ppb->eps2);
    fprintf(stdout,"dtmax  = %g\n", ppb->dtmax);
    fprintf(stdout,"dtcoef = %g\n", ppb->dtcoef);
    fprintf(stdout,"tend   = %g\n", ppb->tend);
    fprintf(stdout,"tblock = %g\n", ppb->tblock);
    fprintf(stdout,"niter  = %d\n", ppb->niter);
}
int main()
{
    PPLINE pl;
    int n;
    double tstart;
    readin_system(&n, &pl, &tstart);
    readin_parameters(&pblock);
    initialize_system(pl, n, tstart);
    //    dump_system(stdout, pl, n);
    print_log(n,pl,1);
    integrate_system(pl, n, pblock.tend);
    //    dump_system(stdout, pl, n);
    print_log(n,pl,0);
    write_stoa_snapshot(stdout,  pl, n);
    return 0;
}

