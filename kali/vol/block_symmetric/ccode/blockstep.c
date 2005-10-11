/*
 * blockstep.c
 */

#include <stdio.h>
#include <math.h>
double sqrt();

#define NDIM 2

typedef struct vect{
    double x;
    double y;
}VECT,*PVECT;

void printv(VECT x, char* s)
{
    printf("%s = %e %e\n", s, x.x, x.y);
}


VECT calc_acc(VECT r)
{
    VECT a;
    double r2 = r.x*r.x+ r.y*r.y;
    double r3inv = 1.0/(r2*sqrt(r2));
    a.x= -r.x*r3inv;
    a.y= -r.y*r3inv;
    return a;
}


VECT inc_with_scale(VECT x, VECT dx, double s)
{
    VECT res;
    res.x = x.x+dx.x*s;
    res.y = x.y+dx.y*s;
    return res;

}
void leapfrog(VECT r,
	      VECT v,
	      PVECT newr,
	      PVECT newv,
	      double dt)
{
    VECT a0, a1;
    a0 = calc_acc(r);
    *newv = inc_with_scale(v, a0, dt*0.5);
    *newr = inc_with_scale(r, *newv, dt);
    a1 = calc_acc(*newr);
    *newv = inc_with_scale(*newv, a1, dt*0.5);
}

double energy(VECT x, VECT v)
{
    return -1.0/sqrt(x.x*x.x+x.y*x.y)+0.5*(v.x*v.x+v.y*v.y);
}

void printenergy(VECT x, VECT v, double time)
{
    static int first=1;
    static double e0;
    double e;
    if (first){
	first=0;
	e0=energy(x,v);
    }
    e=energy(x,v);
    printf("T= %15.6e E= %21.13e DE= %21.13e\n", time, e, e-e0);
}

double vsq(VECT v)
{
    return v.x*v.x+v.y*v.y;
}

double timescale(VECT x, VECT v)
{
    double r2 = vsq(x);
    double est1 = sqrt(r2/vsq(v));
    double est2 = sqrt(sqrt(r2))/r2;
    return (est1 < est2)? est1:est2;
}

double force2(double x)
{
    double y=1;
    while (y > x) y*= 0.5;
    return y;
}

main()
{
    double dt, dtcoef;
    double tmax;
    double dtout;
    double time, tout;
    VECT x, v;
    VECT newx, newv;
    scanf("%le%le%le", &dtcoef, &tmax, &dtout);
    scanf("%le%le%le%le", &(x.x), &(x.y), &(v.x), &(v.y));
    printf("dtcoef, tmax, dtout = %e %e %e\n", dtcoef, tmax, dtout);
    printf("x, v = %e %e %e %e\n", (x.x), (x.y), (v.x), (v.y));
    time=0;
    tout = dtout;
    printenergy(x, v, time);
    while (time < tmax){
	double newdt, dt0;
	int i;
	dt = timescale(x, v)*dtcoef;
	dt0=dt;
#ifdef SIMPLE_SYMMETRIC	
	for (i=0;i<5; i++){
	    leapfrog(x,v, &newx, &newv, dt);
	    newdt=timescale(newx, newv)*dtcoef;
	    dt = 0.5*(dt0+newdt);
	}
#endif
#ifdef CORRECTED_BLOCKSTEP	
        dt=force2(dt0);
	if (fmod(time, dt*2)== 0.0) dt*=2;
	for (i=0;i<5; i++){
	    leapfrog(x,v, &newx, &newv, dt);
	    newdt=timescale(newx, newv)*dtcoef;
	    if (dt > 0.5*(dt0+newdt)) dt *= 0.5;
	}
#endif
#ifdef SIMPLE_BLOCKSTEP
#define NSYM 5	
	dt = force2(dt);
	for (i=0;i<NSYM; i++){
	    leapfrog(x,v, &newx, &newv, dt);
	    newdt=timescale(newx, newv)*dtcoef;
	    if (i < NSYM-1){
		dt = force2(0.5*(dt0+newdt));
	    }
	}
#endif
#ifdef MIN_BLOCKSTEP
#define NSYM 5
	{
	    double steps[NSYM];
	    dt = force2(dt);
	    for (i=0;i<NSYM; i++){
		leapfrog(x,v, &newx, &newv, dt);
		newdt=timescale(newx, newv)*dtcoef;
		if (i < NSYM-1){
		    steps[i] = force2(0.5*(dt0+newdt));
		    if (i < NSYM-2) {
			dt = steps[i];
		    }else{
			dt = (steps[i]>steps[i-1])? steps[i-1]:steps[i];
		    }
		}
	    }
	}
#endif
	
	time += dt;
	x=newx;
	v=newv;
	if (time >= tout){
	    printv(x, "x");
	    printv(v, "v");
	    printenergy(x, v, time);
	    tout += dtout;
	}
    }
}

    
    
