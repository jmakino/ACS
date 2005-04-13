              // Time-stamp: <2004-09-15 10:03:58 piet>
             //================================================================
            //                                                                |
           //           /__----__                         ........            |
          //       .           \                     ....:        :.          |
         //       :                 _\|/_         ..:                         |
        //       :                   /|\         :                     _\|/_  |
       //  ___   ___                  _____                      ___    /|\   |
      //  /     |   \    /\ \     / |   |   \  / |        /\    |   \         |
     //  |   __ |___/   /  \ \   /  |   |    \/  |       /  \   |___/         |
    //   |    | |  \   /____\ \ /   |   |    /   |      /____\  |   \     \/  |
   //     \___| |   \ /      \ V    |   |   /    |____ /      \ |___/     |   |
  //                                                                      /   |
 //              :                       _/|     :..                    |/    |
//                :..               ____/           :....          ..         |
/*   o   //          :.    _\|/_     /                   :........:           |
 *  O  `//\                 /|\                                               |
 *  |     /\                                                                  |
 *=============================================================================
 *
 *  nbody_sh1WD.C:  an N-body integrator with a shared but variable time step
 *                (the same for all particles but its size changing in time),
 *                using the Hermite integration scheme.
 *                        
 *                ref.: Hut, P., Makino, J. & McMillan, S., 1995,
 *                      Astrophysical Journal Letters 443, L93-L96.
 *                
 *  note: in this first version, all functions are included in one file,
 *        without any use of a special library or header files.
 *_____________________________________________________________________________
 *
 * Walter's modifications (in May 2004) in order to:
 *                - use Walter's tupel.h  -> simplify the code
 *                - use a simple class bodies to hold body data -> simpler code
 *                - improve performance (affects only get_acc_jerk_pot_coll(),
 *                  which has been renamed to "get_acc_jerk_coll()")
 *                - compute epot in a separate routines (because in the old
 *                  version, epot was computed from the predicted, not the 
 *                  4th-order accurate positions).
 *                the resulting speed-up is about a factor 2 per step
 *            NOTE: due to the slightly different way the actual computation
 *                  of acceleration and jerk is done, truncation errors have
 *                  a different effect than in the original version. As a
 *                  consequence, the evolution of chaotic systems will differ.
 *            WARNING: the modified code cannot cope with massless particles,
 *                  since they experience no force (but still acceleration)
 *                  which is what is computed (because it's symmetric).
 *
 *_____________________________________________________________________________
 *
 *  usage: nbody_sh1 [-h (for help)] [-d step_size_control_parameter]
 *                   [-e diagnostics_interval] [-o output_interval]
 *                   [-t total_duration] [-i (start output at t = 0)]
 *                   [-x (extra debugging diagnostics)]
 * 
 *         "step_size_control_parameter" is a coefficient determining the
 *            the size of the shared but variable time step for all particles
 *
 *         "diagnostics_interval" is the time between output of diagnostics,
 *            in the form of kinetic, potential, and total energy; with the
 *            -x option, a dump of the internal particle data is made as well
 * 
 *         "output_interval" is the time between successive snapshot outputs
 *
 *         "total_duration" is the integration time, until the program stops
 *
 *         Input/output are read/written from/to the standard i/o streams.
 *         Since all options have sensible defaults, the simplest way to run
 *         the code is by only specifying the i/o files for the N-body
 *         snapshots:
 *
 *            nbody_sh1 < data.in > data.out
 *
 *         The diagnostics information will then appear on the screen.
 *         To capture the diagnostics information in a file, capture the
 *         standard error stream as follows:
 *
 *            (nbody_sh1 < data.in > data.out) >& data.err
 *
 *  Note: if any of the times specified in the -e, -o, or -t options are not an
 *        an integer multiple of "step", output will occur slightly later than
 *        predicted, after a full time step has been taken.  And even if they
 *        are integer multiples, round-off error may induce one extra step.
 *_____________________________________________________________________________
 *
 *  External data format:
 *
 *     The program expects input of a single snapshot of an N-body system,
 *     in the following format: the number of particles in the snapshot n;
 *     the time t; mass mi, position ri and velocity vi for each particle i,
 *     with position and velocity given through their three Cartesian
 *     coordinates, divided over separate lines as follows:
 *
 *                      n
 *                      t
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z
 *                      ...
 *                      mn rn_x rn_y rn_z vn_x vn_y vn_z
 *
 *     Output of each snapshot is written according to the same format.
 *
 *  Internal data format:
 *
 *     The data for an N-body system is stored internally as a 1-dimensional
 *     array for the masses, and 2-dimensional arrays for the positions,
 *     velocities, accelerations and jerks of all particles.
 *_____________________________________________________________________________
 *
 *    version 1:  Jan 2002   Piet Hut, Jun Makino
 *    version 2:  May 2004   Walter Dehnen
 *_____________________________________________________________________________
 */

#include  <iostream>
#include  <cmath>                          // to include sqrt(), etc.
#include  <cstdlib>                        // for atoi() and atof()
#include  <unistd.h>                       // for getopt()
#include  "tupel.h"                        // Walter's tupel<>
using namespace std;

typedef double  real;                      // "real" as a general name for the
                                           // standard floating-point data type

const int NDIM = 3;                        // number of spatial dimensions

typedef WD::tupel<NDIM, real> vect;        // "vect" as general name for a 
                                           // vector of NDIM reals
                                           // (not "vector" to avoid conflicts
                                           //  with std::vector)

// class bodies to hold the body data
//    warning: extremely simple and not fool proof!
class bodies {
    void  allocate();                      // allocate body data & set pointers
    int   N, Ndata;                        // # bodies, # bytes of data
    char *DATA;                            // DATA: pointer to chunk of data 
    real *MASS;                            //    allocated from the heap
    vect *POS,*VEL,*ACC,*JERK;
public:
    // public member methods:
    // const and non-const access to body data
    real&mass(int i) { return MASS[i]; }
    vect&pos (int i) { return POS[i]; }
    vect&vel (int i) { return VEL[i]; }
    vect&acc (int i) { return ACC[i]; }
    vect&jerk(int i) { return JERK[i]; }
    real const&mass(int i) const { return MASS[i]; }
    vect const&pos (int i) const { return POS[i]; }
    vect const&vel (int i) const { return VEL[i]; }
    vect const&acc (int i) const { return ACC[i]; }
    vect const&jerk(int i) const { return JERK[i]; }
    int const&N_bodies() const { return N; }
    // constructors
    bodies() : N(0), Ndata(0), DATA (0) {}
    bodies(bodies const&B) : N (B.N), Ndata(B.Ndata) {
        allocate();
	memcpy(DATA, B.DATA,  Ndata);
    }
    // destruction
    ~bodies() {
        if(DATA) delete[] DATA;
    }
    // copy all/some body data
    void copy         (bodies const&B);
    void copy_pos_vel (bodies const&B);
    void copy_acc_jerk(bodies const&B);
    // I/O of data from stdin / to stdout
    void get (real&t);                       // get snapshot data
    void put (real t);                       // put snapshot 
}
Bodies,                                      // global: bodies
BodiesOld;                                   // global: bodies with old x,v,a,j

// forward declarations of local methods
void correct_step(real dt);
void evolve(real&t, real dt_param, real dt_dia, real dt_out,
	    real dt_tot, bool init_out, bool x_flag);
void evolve_step(real&t, real dt, real&coll_time);
void get_acc_jerk_coll(real&coll_time);
void get_pot_kin(real&epot, real&ekin);
void predict_step(real dt);
bool read_options(int argc, char**argv, real&dt_param, real&dt_dia,
		  real&dt_out, real&dt_tot, bool&i_flag, bool&x_flag);
void write_diagnostics(real t, int nsteps, real&einit,
		       bool init_flag, bool x_flag);

/*-----------------------------------------------------------------------------
 *  main  --  reads option values, reads a snapshot, and launches the
 *            integrator
 *-----------------------------------------------------------------------------
 */

int main(int argc, char **argv)
{
    real  dt_param = 0.03;     // control parameter to determine time step size
    real  dt_dia = 1;          // time interval between diagnostics output
    real  dt_out = 1;          // time interval between output of snapshots
    real  dt_tot = 10;         // duration of the integration
    bool  init_out = false;    // if true: snapshot output with start at t = 0
                               //          with an echo of the input snapshot
    bool  x_flag = false;      // if true: extra debugging diagnostics output

    if (! read_options(argc, argv, dt_param, dt_dia, dt_out, dt_tot, init_out,
                       x_flag))
        return 1;                // halt criterion detected by read_options()

    real t;                      // time
    Bodies.get(t);               // read: t,N, particle data
 
    evolve(t, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag);

}

/*-----------------------------------------------------------------------------
 *  read_options  --  reads the command line options, and implements them.
 *
 *  note: when the help option -h is invoked, the return value is set to false,
 *        to prevent further execution of the main program; similarly, if an
 *        unknown option is used, the return value is set to false.
 *-----------------------------------------------------------------------------
 */

bool read_options(int argc, char**argv, real&dt_param, real&dt_dia,
		  real&dt_out, real&dt_tot, bool&i_flag, bool&x_flag)
{
    int c;
    while ((c = getopt(argc, argv, "hd:e:o:t:ix")) != -1)
        switch(c){
            case 'd': dt_param = atof(optarg);
                      break;
            case 'e': dt_dia = atof(optarg);
                      break;
            case 'i': i_flag = true;
                      break;
            case 'o': dt_out = atof(optarg);
                      break;
            case 't': dt_tot = atof(optarg);
                      break;
            case 'x': x_flag = true;
                      break;
            case 'h':
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-d step_size_control_parameter]\n"
                           << "         [-e diagnostics_interval]"
                           << " [-o output_interval]\n"
                           << "         [-t total_duration]"
                           << " [-i (start output at t = 0)]\n"
                           << "         [-x (extra debugging diagnostics)]"
                           << endl;
                      return false;      // execution stops after help or error
            }

    return true;                         // ready to continue program execution
}

/*-----------------------------------------------------------------------------
 * bodies::allocate() by Walter, 2004
 *    allocated body data
 *    note: we assume bodies::Ndata to be known.
 *          we allocate all body data into one single array so that we can use
 *          memcpy() to make a copy of the old body data
 *-----------------------------------------------------------------------------
 */
void bodies::allocate() {
    if(Ndata) { 
        DATA = new char[Ndata];
	char*c = DATA;
	MASS = static_cast<real*>(static_cast<void*>(DATA));
	POS  = static_cast<vect*>(static_cast<void*>(MASS+N));
	VEL  = POS + N;
	ACC  = VEL + N;
	JERK = ACC + N;
    } else
      DATA = 0;
}

/*-----------------------------------------------------------------------------
 * bodies::copy() by Walter, 2004
 *    make this a copy of B
 *-----------------------------------------------------------------------------
 */
void bodies::copy(bodies const&B) {
    if(&B == this) return;
    if(N != B.N) {
        if(DATA) delete[] DATA;
	N     = B.N;
	Ndata = B.Ndata;
	allocate();
    }
    memcpy(DATA, B.DATA,  Ndata);
}
/*-----------------------------------------------------------------------------
 * bodies::get() by Walter, 2004
 *  superseeds get_snapshot()
 *-----------------------------------------------------------------------------
 */
void bodies::get(real&t) {
    int n;
    cin >> n >> t;
    if (n != N) {
        if(DATA) delete[] DATA;
	N     = n;
        Ndata = N*(4*sizeof(vect)+sizeof(real));
	allocate();
    }
    for (int i = 0; i != N ; ++i) {
        cin >> mass(i) >> pos(i) >> vel(i);
	if(mass(i) == 0) {
	    cerr<<" mass of body "<<i+1<<" == 0\n"
		<<" the code cannot deal with this currently, sorry\n";
	    exit(1);
	}
    }
}
/*-----------------------------------------------------------------------------
 * bodies::put() by Walter, 2004
 *  superseeds put_snapshot()
 *-----------------------------------------------------------------------------
 */
void bodies::put(real t) {
    cout.precision(16);                         // full double precision
    cout << N << '\n'                           // N, total particle number
	 << t << '\n';                          // current time
    for (int i = 0; i != N; ++i)
        cout << mass(i) << '\n'                 // mass of particle i
	     << pos (i) << '\n'                 // position of particle i
	     << vel (i) << '\n';                // velocity of particle i
    cout.flush();
}
    
/*-----------------------------------------------------------------------------
 *  write_diagnostics  --  writes diagnostics on the error stream cerr:
 *                         current time; number of integration steps so far;
 *                         kinetic, potential, and total energy; absolute and
 *                         relative energy errors since the start of the run.
 *                         If x_flag (x for eXtra data) is true, all internal
 *                         data are dumped for each particle (mass, position,
 *                         velocity, acceleration, and jerk).
 *-----------------------------------------------------------------------------
 */

void write_diagnostics(real t, int nsteps, real&einit, 
		       bool init_flag, bool x_flag)
{
    real epot, ekin;
    get_pot_kin(epot, ekin);
    real etot = ekin + epot;             // total energy of the n-body system

    if (init_flag)                       // at first pass, pass the initial
        einit = etot;                    // energy back to the calling function

    cerr << "at time t = " << t << " , after " << nsteps
         << " steps :\n  E_kin = " << ekin
         << " , E_pot = " << epot
         << " , E_tot = " << etot << '\n'
         << "                "
         << "absolute energy error: E_tot - E_init = "
         << etot - einit << '\n'
         << "                "
         << "relative energy error: (E_tot - E_init) / E_init = "
         << (etot - einit) / einit << endl;

    if (x_flag){
        cerr << "  for debugging purposes, here is the internal data "
             << "representation:\n";
        for (int i = 0; i != Bodies.N_bodies() ; ++i)
            cerr << "    internal data for particle " << i+1 << " :\n      "
		 << Bodies.mass(i) << ' '
		 << Bodies.pos (i) << ' '
		 << Bodies.vel (i) << ' '
		 << Bodies.acc (i) << ' '
		 << Bodies.jerk(i) << endl;
    }
}
    
/*-----------------------------------------------------------------------------
 *  evolve  --  integrates an N-body system, for a total duration dt_tot.
 *              Snapshots are sent to the standard output stream once every
 *              time interval dt_out.  Diagnostics are sent to the standard
 *              error stream once every time interval dt_dia.
 *
 *  note: the integration time step, shared by all particles at any given time,
 *        is variable.  Before each integration step we use coll_time (short
 *        for collision time, an estimate of the time scale for any significant
 *        change in configuration to happen), multiplying it by dt_param (the
 *        accuracy parameter governing the size of dt in units of coll_time),
 *        to obtain the new time step size.
 *
 *  Before moving any particles, we start with an initial diagnostics output
 *  and snapshot output if desired.  In order to write the diagnostics, we
 *  first have to calculate the potential energy, with get_acc_jerk_pot_coll().
 *  That function also calculates accelerations, jerks, and an estimate for the
 *  collision time scale, all of which are needed before we can enter the main
 *  integration loop below.
 *       In the main loop, we take as many integration time steps as needed to
 *  reach the next output time, do the output required, and continue taking
 *  integration steps and invoking output this way until the final time is
 *  reached, which triggers a `break' to jump out of the infinite loop set up
 *  with `while(true)'.
 *-----------------------------------------------------------------------------
 */

void evolve(real&t, real dt_param, real dt_dia, real dt_out,
	    real dt_tot, bool init_out, bool x_flag)
{
    cerr << "Starting a Hermite integration for a " << Bodies.N_bodies()
         << "-body system,\n  from time t = " << t 
         << " with time step control parameter dt_param = " << dt_param
         << "  until time " << t + dt_tot 
         << " ,\n  with diagnostics output interval dt_dia = "
         << dt_dia << ",\n  and snapshot output interval dt_out = "
         << dt_out << "." << endl;

    real coll_time;                   // collision (close encounter) time scale

    get_acc_jerk_coll(coll_time);

    int nsteps = 0;               // number of integration time steps completed
    real einit;                   // initial total energy of the system

    write_diagnostics(t, nsteps, einit, true, x_flag);
    if (init_out)                                    // flag for initial output
        Bodies.put(t);

    real t_dia = t + dt_dia;           // next time for diagnostics output
    real t_out = t + dt_out;           // next time for snapshot output
    real t_end = t + dt_tot;           // final time, to finish the integration

    do {
        while (t < t_dia && t < t_out && t < t_end){
            real dt = dt_param * coll_time;
            evolve_step(t, dt, coll_time);
            nsteps++;
        }
        if (t >= t_dia){
            write_diagnostics(t, nsteps, einit, false, x_flag);
            t_dia += dt_dia;
        }
        if (t >= t_out){
	    Bodies.put(t);
            t_out += dt_out;
        }
    } while (t < t_end);
}

/*-----------------------------------------------------------------------------
 *  evolve_step  --  takes one integration step for an N-body system, using the
 *                   Hermite algorithm.
 *-----------------------------------------------------------------------------
 */

void evolve_step(real&t, real dt, real&coll_time)
{
    BodiesOld.copy(Bodies);
    predict_step(dt);
    get_acc_jerk_coll(coll_time);
    correct_step(dt);
    t += dt;
}

/*-----------------------------------------------------------------------------
 *  predict_step  --  takes the first approximation of one Hermite integration
 *                    step, advancing the positions and velocities through a
 *                    Taylor series development up to the order of the jerks.
 *-----------------------------------------------------------------------------
 */

void predict_step(real dt)
{
    const real dtqh = dt*dt/2;
    const real dtcs = dt*dt*dt/6;
    for (int i = 0; i != Bodies.N_bodies() ; ++i) {
        Bodies.pos(i).add_times(Bodies.vel (i),dt);
        Bodies.pos(i).add_times(Bodies.acc (i),dtqh);
        Bodies.pos(i).add_times(Bodies.jerk(i),dtcs);
	Bodies.vel(i).add_times(Bodies.acc (i),dt);
	Bodies.vel(i).add_times(Bodies.jerk(i),dtqh);
    }
}

/*-----------------------------------------------------------------------------
 *  correct_step  --  takes one iteration to improve the new values of position
 *                    and velocities, effectively by using a higher-order
 *                    Taylor series constructed from the terms up to jerk at
 *                    the beginning and the end of the time step.
 *-----------------------------------------------------------------------------
 */

void correct_step(real dt)
{
    const real dth  = dt/2;
    const real dtqt = dt*dt/12;
    for (int i = 0; i != Bodies.N_bodies() ; ++i) {
        Bodies.vel(i) = BodiesOld.vel(i);
        Bodies.vel(i).add_times(BodiesOld.acc (i) + Bodies.acc (i),dth);
        Bodies.vel(i).add_times(BodiesOld.jerk(i) - Bodies.jerk(i),dtqt);
        Bodies.pos(i) = BodiesOld.pos(i);
        Bodies.pos(i).add_times(BodiesOld.vel (i) + Bodies.vel (i),dth);
        Bodies.pos(i).add_times(BodiesOld.acc (i) - Bodies.acc (i),dtqt);
    }
}

/*-----------------------------------------------------------------------------
 *  get_acc_jerk_coll  --  calculates accelerations and jerks, and as side
 *                         effect also calculates the time scale coll_time for
 *                         significant changes in local configurations to occur.
 *                                                  __                     __
 *                                                 |          -->  -->       |
 *               M                           M     |           r  . v        |
 *   -->          j    -->       -->          j    | -->        ji   ji -->  |
 *    a   ==  --------  r    ;    j   ==  -------- |  v   - 3 ---------  r   |
 *     ji     |-->  |3   ji        ji     |-->  |3 |   ji      |-->  |2   ji |
 *            | r   |                     | r   |  |           | r   |       |
 *            |  ji |                     |  ji |  |__         |  ji |     __|
 *                             
 *  note with the new code (WD, May 2004):
 *        the potential energy is better not computed here, since here we use
 *        the predicted position and velocity and not the actual 4th-order ones.
 *        Moreover, since epot is needed only when diagnostic output is made,
 *        computing it separately does NOT double the computer time, contrary
 *        to the statement below in the original note.
 *                             
 *  note with the original code:
 *        it would be cleaner to calculate potential energy and collision time
 *        in a separate function.  However, the current function is by far the
 *        most time consuming part of the whole program, with a double loop
 *        over all particles that is executed every time step.  Splitting off
 *        some of the work to another function would significantly increase
 *        the total computer time (by an amount close to a factor two).
 *                             
 *  We determine the values of all four quantities of interest by walking
 *  through the system in a double {i,j} loop.  The first three, acceleration,
 *  jerk, and potential energy, are calculated by adding successive terms;
 *  the last, the estimate for the collision time, is found by determining the 
 *  minimum value over all particle pairs and over the two choices of collision
 *  time, position/velocity and sqrt(position/acceleration), where position and
 *  velocity indicate their relative values between the two particles, while
 *  acceleration indicates their pairwise acceleration.  At the start, the
 *  first three quantities are set to zero, to prepare for accumulation, while
 *  the last one is set to a very large number, to prepare for minimization.
 *       The integration loops only over half of the pairs, with j > i, since
 *  the contributions to the acceleration and jerk of particle j on particle i
 *  is the same as those of particle i on particle j, apart from a minus sign
 *  and a different mass factor.
 *-----------------------------------------------------------------------------
 *
 * Modifications by Walter to improve performance:
 *
 *  - using of vect
 *  - (re-) using of register variables.
 *  - avoiding divisions
 *  - computing forces rather than accelerations:
 *    the loop to compute accelerations looks like this (with obvious notation):
 *
 *    for(i=0; i!=n; ++i) 
 *        a[i] = 0.                                 // reset accelerations to 0
 *    for(i=0; i!=n; ++i) {
 *        mi = m[i];                                // get mass and pos of ith
 *        ri = r[i];                                // body into register
 *        ai = 0.;                                  // to hold force due to j>i
 *        for(j=i+1; j!=n; ++j) {                   // loop pairs j>i
 *           rji  = r[j] - ri;                      // distance vector
 *           rji *= mi*m[j] / |rji|^3               // mutual force
 *           ai  += rji;                            // add: force_i due to j>i
 *           a[j]-= rji;                            // add: force_j due to i<j
 *        }
 *        a[i] = (a[i] * ai)/mi;                    // acceleration
 *    }
 *
 *    as a result, the inner most loop (over j>i) contains
 *    
 *    operator:    +/-       *          /          sqrt
 *    old code:     35      36          9          1
 *    this code:    29      28          1          1
 *-----------------------------------------------------------------------------
 */

void get_acc_jerk_coll(real&coll_time)
{
    for (int i = 0; i < Bodies.N_bodies() ; i++) {
        Bodies.acc (i) = 0.;
        Bodies.jerk(i) = 0.;
    }
    real inv_coll_time_q = 0.;                 // 1/collision time to 4th power
    real inv_coll_est_q;                       // estimate of ----- 

    for (int i = 0; i != Bodies.N_bodies() ; ++i){
        const real mi(Bodies.mass(i));                    // mi                
        const vect ri(Bodies.pos(i)), vi(Bodies.vel(i));  // ri, vi            
        register vect ai = 0., ji = 0.;                   // ai, ji due to j>i 
        for (int j = i+1; j != Bodies.N_bodies() ; ++j){
  	    // pre-compute some auxiliary quantites
  	    register vect rji(Bodies.pos(j)); rji-= ri; // rji = distance vector
	    register vect vji(Bodies.vel(j)); vji-= vi; // vji = d rji / d t
	    register real
	      r2    = norm(rji),                   // rji^2
	      v2    = norm(vji),                   // vji^2
	      pr2   = 1./r2,                       // 1 / rji^2
	      rv_r2 =(rji*vji) * pr2;              // ( rij . vij ) / rji^2
	    // first collision time estimate, 
	    // based on unaccelerated linear motion:
	    inv_coll_est_q = WD::meta::square(v2*pr2);
	    if (inv_coll_est_q > inv_coll_time_q)
	        inv_coll_time_q = inv_coll_est_q;
	    // second collision time estimate,
	    // based on free fall:
	    inv_coll_est_q = WD::meta::square((mi+Bodies.mass(j))*pr2)*pr2;
	    if (inv_coll_est_q > inv_coll_time_q)
	        inv_coll_time_q = inv_coll_est_q;
	    // add the {j (i)} contribution to the {i (j)} values of
	    // force and dforce/dt
	    pr2 *= Bodies.mass(j)*mi*sqrt(pr2);  // mi*mj / |rji|^3
	    rji *= pr2;                          // mutual force = mi*mj*da
	    vji *= pr2;
	    vji.sub_times(rji,3*rv_r2);          // mutual dforce/dt = mi*mj*dj
	    ai             += rji;               // force     at i due to j>i
	    Bodies.acc (j) -= rji;               // force     at j due to i<j
	    ji             += vji;               // dforce/dt at i due to j>i
	    Bodies.jerk(j) -= vji;               // dforce/dt at j due to i<j
        }        
	// add forces due to j>i and divide by mass to get accelerations
	register real tmp = 1./mi;
	Bodies.acc (i)   += ai;
	Bodies.acc (i)   *= tmp;
	// add dforces/dt due to j>i and divide by mass to get jerks
	Bodies.jerk(i)   += ji;
	Bodies.jerk(i)   *= tmp;
    }                                               // from q for quartic back
    coll_time = 1./sqrt(sqrt(inv_coll_time_q));     // to linear collision time
}                                             
/*-----------------------------------------------------------------------------
 *  get_pot_kin  --  computes the potential and kinetic energy from the actual
 *                   body positions and velocities.
 *-----------------------------------------------------------------------------
 */
void get_pot_kin(real&epot, real&ekin)
{
    ekin = 0;
    epot = 0.;
    for(int i = 0; i != Bodies.N_bodies() ; ++i){
        const real mi(Bodies.mass(i));                    // mi                
        const vect ri(Bodies.pos(i));                     // ri                
        ekin += mi * norm(Bodies.vel(i));
        for (int j = i+1; j != Bodies.N_bodies() ; ++j)
	    epot -= Bodies.mass(j)*mi/dist(ri,Bodies.pos(j));
    }                                               // from q for quartic back
    ekin *= 0.5;
}                                             

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  nbody_sh1WD.C                                       /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
