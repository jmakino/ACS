/* File : pairwise.c */

typedef double  real;
const int NDIM = 3;                        // number of spatial dimensions

typedef struct{
  int epot;
  } Epot_struct, *Epot_structptr;

Epot_struct test_pairwise_acc_jerk_pot_coll(real massi, real massj,
 			                       Epot_struct epot_test);

void pairwise_acc_jerk_pot_coll(real massi, real massj,
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
    
    // add the {i,j} contribution to the total potential energy for the system:
    
    *epot -= massi * massj / r;
    
    // add the {j (i)} contribution to the {i (j)} values of acceleration and jerk:
    
    for (k = 0; k < NDIM ; k++){
	da[k] = rji[k] / r3;                           // see equations
	dj[k] = (vji[k] - 3 * rv_r2 * rji[k]) / r3;    // in the header
    }
    for (k = 0; k < NDIM ; k++){
	acci[k] += massj * da[k];                 // using symmetry
	accj[k] -= massi * da[k];                 // find pairwise
	jerki[k] += massj * dj[k];                // acceleration
	jerkj[k] -= massi * dj[k];                // and jerk
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
