              // Time-stamp: <2003-01-13 18:06:48 piet>
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
 *  nbody_sh1.C:  an N-body integrator with a shared but variable time step
 *                (the same for all particles but changing in time), using
 *                the Hermite integration scheme.
 *                        
 *                ref.: Hut, P., Makino, J. & McMillan, S., 1995,
 *                      Astrophysical Journal Letters 443, L93-L96.
 *                
 *  note: in this first version, all functions are included in one file,
 *        without any use of a special library or header files.
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
 *    version 2:  Jan 2003   Piet Hut ( crude attempt at XLM I/O, C++ classes )
 *_____________________________________________________________________________
 */

#include  <iostream>
#include  <vector>
#include  <algorithm>
#include  <cmath>                          // to include sqrt(), etc.
#include  <cstdlib>                        // for atoi() and atof()
#include  <unistd.h>                       // for getopt()
using namespace std;

typedef double  real;                      // "real" as a general name for the
                                           // standard floating-point data type

const int NDIM = 3;                        // number of spatial dimensions
const int BUFFER_SIZE = 256;               // maximum line length for input

/*-----------------------------------------------------------------------------
 *  space_vector  --  a simple class for NDIM-dimensional vectors
 *-----------------------------------------------------------------------------
 */
class  space_vector
{
public:
//	Default: initialize to zero.

        space_vector(real c = 0)
	    {component[0] = component[1] = component[2] = c;}

	space_vector(real x, real y, real z)
	    {component[0] = x; component[1] = y; component[2] = z;}

//  []: the return type is declared as a reference (&), so that it can be used
//  on the left-hand side of an asignment, as well as on the right-hand side,
//  i.e.  v[1] = 3.14  and  x = v[2]  are both allowed and work as expected.

        real & operator [] (int i)       {return component[i];}

	inline void print() {cout << component[0] << " " << component[1] << " "
				  << component[2] << "\n";}

//	Unary -

        space_vector operator - ()
	    {return space_vector(-component[0], -component[1], -component[2]);}

//	Dot product.

        real operator * (const space_vector& b)
	    {return component[0]*b.component[0]
		  + component[1]*b.component[1]
		  + component[2]*b.component[2];}

//	Outer product.

        space_vector operator ^ (const space_vector &b)
	    {return space_vector(
	     component[1]*b.component[2] - component[2]*b.component[1],
	     component[2]*b.component[0] - component[0]*b.component[2],
	     component[0]*b.component[1] - component[1]*b.component[0]);}

//	Vector +, -

        space_vector operator + (const space_vector &b)
	    {return space_vector(component[0]+b.component[0],
			   component[1]+b.component[1],
			   component[2]+b.component[2]);}
        space_vector operator - (const space_vector &b)
	    {return space_vector(component[0]-b.component[0],
		 	   component[1]-b.component[1],
			   component[2]-b.component[2]);}

        friend space_vector operator + (real, const space_vector & );
        friend space_vector operator + (const space_vector &, real);

//	Scalar *, /

        friend space_vector operator * (real, const space_vector & );
        friend space_vector operator * (const space_vector &, real);
        friend space_vector operator / (const space_vector &, real);

//	Vector +=, -=, *=, /=

        space_vector& operator += (const space_vector& b)
	    {component[0] += b.component[0];       
	     component[1] += b.component[1];
	     component[2] += b.component[2];
	     return *this;}

	space_vector& operator -= (const space_vector& b)
	    {component[0] -= b.component[0];
	     component[1] -= b.component[1];
	     component[2] -= b.component[2];
	     return *this;}

	space_vector& operator *= (const real b)
	    {component[0] *= b;
	     component[1] *= b;
	     component[2] *= b;
	     return *this;}

	space_vector& operator /= (const real b)
	    {component[0] /= b;
	     component[1] /= b;
	     component[2] /= b;
	     return *this;}

//      Input / Output

        friend ostream & operator << (ostream & , const space_vector & );

	friend istream & operator >> (istream & , space_vector & );

    private:
        real component[NDIM];
};

inline  ostream & operator << (ostream & s, const space_vector & v)
	    {return s << v.component[0] << "  " << v.component[1]
		      << "  " << v.component[2];}

inline  istream & operator >> (istream & s, space_vector & v)
	    {s >> v.component[0] >> v.component[1] >> v.component[2];
	     return s;}

inline  real square(space_vector v) {return v*v;}
inline  real abs(space_vector v)    {return sqrt(v*v);}

// Another measure of vector magnitude; less work than abs():

inline  real abs1(space_vector v)   {return abs(v[0]) + abs(v[1]) + abs(v[2]);}

inline  space_vector operator + (real b, const space_vector & v)
	    {return space_vector(b+v.component[0],
			   b+v.component[1],
			   b+v.component[2]);}

inline  space_vector operator + (const space_vector & v, real b)
	    {return space_vector(b+v.component[0],
			   b+v.component[1],
			   b+v.component[2]);}

inline  space_vector operator * (real b, const space_vector & v)
	    {return space_vector(b*v.component[0],
			   b*v.component[1],
			   b*v.component[2]);}

inline  space_vector operator * (const space_vector & v, real b)
	    {return space_vector(b*v.component[0],
			   b*v.component[1],
			   b*v.component[2]);}

inline  space_vector operator / (const space_vector & v, real b)
	    {return space_vector(v.component[0]/b,
			   v.component[1]/b,
			   v.component[2]/b);}

/*-----------------------------------------------------------------------------
 *  body  --  a simple particle class
 *-----------------------------------------------------------------------------
 */
class  body
{
public:
//    void  set_time(const real new_time)              {_time = new_time;}
//    void  clear_time()                               {_time = 0.0;}
//    void  inc_time(const real d_time)                {_time += d_time;}
//    void  scale_time(const real scale_factor)        {_time *= scale_factor;}
//    inline real  get_time()                          {return _time;}

    void  set_mass(const real new_mass)              {_mass = new_mass;}
    void  clear_mass()                               {_mass = 0.0;}
    void  inc_mass(const real d_mass)                {_mass += d_mass;}
    void  scale_mass(const real scale_factor)        {_mass *= scale_factor;}
    inline real  get_mass()                          {return _mass;}

    void  set_pos(const space_vector& new_pos)       {_pos = new_pos;}
    void  set_vel(const space_vector& new_vel)       {_vel = new_vel;}
    void  set_acc(const space_vector& new_acc)       {_acc = new_acc;}
    void  set_jrk(const space_vector& new_jrk)       {_jrk = new_jrk;}

    void  clear_pos()                                {_pos = 0.0;}
    void  clear_vel()                                {_vel = 0.0;}
    void  clear_acc()                                {_acc = 0.0;}
    void  clear_jrk()                                {_jrk = 0.0;}

    inline void  inc_pos(const space_vector& d_pos)  {_pos += d_pos;}
    inline void  inc_vel(const space_vector& d_vel)  {_vel += d_vel;}
    inline void  inc_acc(const space_vector& d_acc)  {_acc += d_acc;}
    inline void  inc_jrk(const space_vector& d_jrk)  {_jrk += d_jrk;}

    inline void  scale_pos(const real scale_factor)  {_pos *= scale_factor;}
    inline void  scale_vel(const real scale_factor)  {_vel *= scale_factor;}
    inline void  scale_acc(const real scale_factor)  {_acc *= scale_factor;}
    inline void  scale_jrk(const real scale_factor)  {_jrk *= scale_factor;}

    inline space_vector  get_pos()                   {return _pos;}
    inline space_vector  get_vel()                   {return _vel;}
    inline space_vector  get_acc()                   {return _acc;}
    inline space_vector  get_jrk()                   {return _jrk;}

    inline space_vector  get_old_pos()               {return _old_pos;}
    inline space_vector  get_old_vel()               {return _old_vel;}
    inline space_vector  get_old_acc()               {return _old_acc;}
    inline space_vector  get_old_jrk()               {return _old_jrk;}

    void  update()                          {_old_pos = _pos; _old_vel = _vel;
                                             _old_acc = _acc; _old_jrk = _jrk;}

    friend ostream & operator << (ostream & , const body & );

    friend istream & operator >> (istream & , body & );

private:
//    real  _time;                        // time
    real  _mass;                        // mass
    space_vector  _pos;                 // position
    space_vector  _vel;                 // velocity
    space_vector  _acc;                 // acceleration
    space_vector  _jrk;                 // jerk    (3rd time derivative of pos)
//  space_vector  _snp;                 // snap    (4th time derivative of pos)
//  space_vector  _crc;                 // crackle (5th time derivative of pos)
//  space_vector  _pop;                 // pop     (6th time derivative of pos)
    space_vector  _old_pos;             // previous position
    space_vector  _old_vel;             // previous velocity
    space_vector  _old_acc;             // previous acceleration
    space_vector  _old_jrk;             // previous jerk
};

inline  ostream & operator << (ostream & s, const body & b)
//            {return s << b._time << "\n" << b._mass << "\n" <<
            {return s << b._mass << "\n" <<
	    b._pos << "\n" << b._vel << "\n";}

inline  istream & operator >> (istream & s, body & b)
//            {s >> b._time >> b._mass >> b._pos >> b._vel;
            {s >> b._mass >> b._pos >> b._vel;
	     return s;}

/*-----------------------------------------------------------------------------
 *  function declarations
 *-----------------------------------------------------------------------------
 */

typedef vector < body > nbody;             // internal representation
                                           // of an N-body snapshot
void begin_get_snapshot();
void begin_get_snapstream();
void begin_put_snapstream();
void correct_step(nbody &snap, real dt);
void end_get_snapshot();
void end_put_snapstream();
void evolve(nbody &snap, real &t, real dt_param, real dt_dia, real dt_out,
            real dt_tot, bool init_out, bool x_flag, bool y_flag);
void evolve_step(nbody &snap, real &t, real dt, real & epot, real & coll_time);
void get_acc_jerk_pot_coll(nbody &snap, real & epot, real & coll_time);
void get_snapshot(nbody &snap, real &t);
void predict_step(nbody &snap, real dt);
void put_snapshot(const nbody &snap, const real &t);
bool read_options(int argc, char *argv[], real & dt_param, real & dt_dia,
                  real & dt_out, real & dt_tot, bool & i_flag, bool & x_flag,
		  bool & y_flag);
void write_diagnostics(const nbody &snap, const real &t, real epot,
                       int nsteps, real & einit, bool init_flag,
                       bool x_flag);

/*-----------------------------------------------------------------------------
 *  main  --  reads option values, reads a snapshot, and launches the
 *            integrator
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    real  dt_param = 0.03;     // control parameter to determine time step size
    real  dt_dia = 1;          // time interval between diagnostics output
    real  dt_out = 1;          // time interval between output of snapshots
    real  dt_tot = 10;         // duration of the integration
    bool  init_out = false;    // if true: snapshot output with start at t = 0
                               //          with an echo of the input snapshot
    bool  x_flag = false;      // if true: extra debugging diagnostics output
    bool  y_flag = false;      // if true: no debugging diagnostics output

    if (! read_options(argc, argv, dt_param, dt_dia, dt_out, dt_tot, init_out,
                       x_flag, y_flag))
        return 1;                // halt criterion detected by read_options()

    begin_get_snapstream();
    begin_get_snapshot();

    real t;                      // time
    nbody snap;                  // an Nbody snapshot

    get_snapshot(snap, t);

    end_get_snapshot();

    begin_put_snapstream();

    evolve(snap, t, dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag,
	   y_flag);

    end_put_snapstream();
}

/*-----------------------------------------------------------------------------
 *  read_options  --  reads the command line options, and implements them.
 *
 *  note: when the help option -h is invoked, the return value is set to false,
 *        to prevent further execution of the main program; similarly, if an
 *        unknown option is used, the return value is set to false.
 *-----------------------------------------------------------------------------
 */

bool read_options(int argc, char *argv[], real & dt_param, real & dt_dia,
                  real & dt_out, real & dt_tot, bool & i_flag, bool & x_flag,
		  bool & y_flag)
{
    int c;
    while ((c = getopt(argc, argv, "hd:e:o:t:ixy")) != -1)
        switch(c){
            case 'h': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-d step_size_control_parameter]\n"
                           << "         [-e diagnostics_interval]"
                           << " [-o output_interval]\n"
                           << "         [-t total_duration]"
                           << " [-i (start output at t = 0)]\n"
                           << "         [-x (extra debugging diagnostics)]"
                           << " [-y (no debugging diagnostics)]"
                           << endl;
                      return false;         // execution should stop after help
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
            case 'y': y_flag = true;
                      break;
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-d step_size_control_parameter]\n"
                           << "         [-e diagnostics_interval]"
                           << " [-o output_interval]\n"
                           << "         [-t total_duration]"
                           << " [-i (start output at t = 0)]\n"
                           << "         [-x (extra debugging diagnostics)]"
                           << " [-y (no debugging diagnostics)]"
                           << endl;
                      return false;        // execution should stop after error
            }

    return true;                         // ready to continue program execution
}

/*-----------------------------------------------------------------------------
 *  begin_get_snapstream  --  reads the first part of an XLM document that
 *                            will contain a stream of snapshots
 *-----------------------------------------------------------------------------
 */

void begin_get_snapstream()
{
    char buffer[BUFFER_SIZE];

    cin.getline(buffer, BUFFER_SIZE);
    cin.getline(buffer, BUFFER_SIZE);

    // of course, we'll have to check, but for now this works if all is okay
}
    
/*-----------------------------------------------------------------------------
 *  begin_put_snapstream  --  writes the first part of an XLM element that
 *                            contains a snapshot
 *-----------------------------------------------------------------------------
 */

void begin_put_snapstream()
{
    cout << "<?xml version=\"1.0\"?>" << endl;
    cout << "<nbody-snapstream version=\"0.1\">" << endl;
}
    
/*-----------------------------------------------------------------------------
 *  end_put_snapstream  --  writes the last part of an XLM element that
 *                          contains a snapshot
 *-----------------------------------------------------------------------------
 */

void end_put_snapstream()
{
    cout << "</nbody-snapstream>" << endl;
}
    
/*-----------------------------------------------------------------------------
 *  begin_get_snapshot  --  reads the first part of an XLM element that
 *                          contains a snapshot
 *-----------------------------------------------------------------------------
 */

void begin_get_snapshot()
{
    char buffer[BUFFER_SIZE];

    cin.getline(buffer, BUFFER_SIZE);

    // of course, we'll have to check, but for now this works if all is okay
}
    
/*-----------------------------------------------------------------------------
 *  end_get_snapshot  --  reads the last part of an XLM element that
 *                        contains a snapshot
 *-----------------------------------------------------------------------------
 */

void end_get_snapshot()
{
    char buffer[BUFFER_SIZE];

    cin.getline(buffer, BUFFER_SIZE);

    // of course, we'll have to check, but for now this works if all is okay
}
    
/*-----------------------------------------------------------------------------
 *  get_snapshot  --  reads a single snapshot from the input stream cin.
 *
 *  note: the expected format is: the number of particles in the snapshot n;
 *        the time t; the mass mi, position ri and velocity vi for each
 *        particle i, with position and velocity given through their three
 *        Cartesian coordinates, divided over separate lines as follows:
 *
 *                      n
 *                      t
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z
 *                      ...
 *                      mn rn_x rn_y rn_z vn_x vn_y vn_z
 *                      
 *-----------------------------------------------------------------------------
 */

void get_snapshot(nbody &snap, real &t)
{
    int n;                       // N, number of particles in the N-body system

    cin >> n;
    cin >> t;

    istream_iterator < body > in(cin);      // [from nbody_leap2a.C ; check!!]
    for (int i = 0; i < n ; i++)            // [from nbody_leap2a.C ; check!!]
        snap.push_back(*in++);              // [from nbody_leap2a.C ; check!!]
}
    
/*-----------------------------------------------------------------------------
 *  put_snapshot  --  writes a single snapshot on the output stream cout.
 *
 *  note: unlike get_snapshot(), put_snapshot handles particle number and time
 *-----------------------------------------------------------------------------
 */

void put_snapshot(const nbody &snap, const real &t)
{
    cout.precision(16);                       // full double precision

    cout << "<nbody-snapshot>" << endl;

    int n = snap.size();                      // N, number of particles
    cout << n << endl;                        // N, total particle number
    cout << t << endl;                        // current time
    for (int i = 0; i < n ; i++)
        cout << snap[i];

    cout << "</nbody-snapshot>" << endl;
}
    
/*-----------------------------------------------------------------------------
 *  write_diagnostics  --  writes diagnostics on the error stream cerr:
 *                         current time; number of integration steps so far;
 *                         kinetic, potential, and total energy; absolute and
 *                         relative energy errors since the start of the run.
 *                         If x_flag (x for eXtra data) is true, all internal
 *                         data are dumped for each particle (mass, position,
 *                         velocity, acceleration, and jerk).
 *
 *  note: the kinetic energy is calculated here, while the potential energy is
 *        calculated in the function get_acc_jerk_pot_coll().
 *-----------------------------------------------------------------------------
 */

//
// why is it wrong to include "const"?  The compiler complains . . . .
//
// void write_diagnostics(const nbody &snap, const real &t, real epot,
void write_diagnostics(nbody &snap, const real &t, real epot,
                       int nsteps, real & einit, bool init_flag,
                       bool x_flag)
{
    real ekin = 0;                       // kinetic energy of the n-body system

//
// for the oversensitive compiler, we will force snap.size() to be signed int:
//
//    for (int i = 0; i < snap.size() ; i++)
    for (int i = 0; i < (signed) snap.size() ; i++)
        ekin += 0.5 * snap[i].get_mass() *
	        ( snap[i].get_vel() * snap[i].get_vel() );

    real etot = ekin + epot;             // total energy of the n-body system

    if (init_flag)                       // at first pass, pass the initial
        einit = etot;                    // energy back to the calling function

    cerr << "at time t = " << t << " , after " << nsteps
         << " steps :\n  E_kin = " << ekin
         << " , E_pot = " << epot
         << " , E_tot = " << etot << endl;
    cerr << "                "
         << "absolute energy error: E_tot - E_init = "
         << etot - einit << endl;
    cerr << "                "
         << "relative energy error: (E_tot - E_init) / E_init = "
         << (etot - einit) / einit << endl;

    if (x_flag){
        cerr << "  for debugging purposes, here is the internal data "
             << "representation:\n";
        for (int i = 0; i < snap.size() ; i++){
            cerr << "    internal data for particle " << i+1 << " : " << endl;
            cerr << "      ";
            cerr << snap[i].get_mass();
            cerr << ' ' << snap[i].get_pos();
            cerr << ' ' << snap[i].get_vel();
            cerr << ' ' << snap[i].get_jrk();
            cerr << endl;
        }
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

void evolve(nbody &snap, real &t, real dt_param, real dt_dia, real dt_out,
            real dt_tot, bool init_out, bool x_flag, bool y_flag)
{
    if (! y_flag)
	cerr << "Starting a Hermite integration for a " << snap.size()
	     << "-body system,\n  from time t = " << t 
	     << " with time step control parameter dt_param = " << dt_param
	     << "  until time " << t + dt_tot 
	     << " ,\n  with diagnostics output interval dt_dia = "
	     << dt_dia << ",\n  and snapshot output interval dt_out = "
	     << dt_out << "." << endl;

    real epot;                        // potential energy of the n-body system
    real coll_time;                   // collision (close encounter) time scale

    get_acc_jerk_pot_coll(snap, epot, coll_time);

    int nsteps = 0;               // number of integration time steps completed
    real einit;                   // initial total energy of the system

    if (! y_flag)
        write_diagnostics(snap, t, epot, nsteps, einit,
                      true, x_flag);
    if (init_out)                                    // flag for initial output
        put_snapshot(snap, t);

    real t_out = t + dt_out;           // next time for snapshot output
    real t_end = t + dt_tot;           // final time, to finish the integration
    real t_dia;
    const real VERY_LARGE_NUMBER = 1e300;
    if (y_flag)
        t_dia = VERY_LARGE_NUMBER;
    else
        t_dia = t + dt_dia;       // next time for diagnostics output

    while (true){
        while (t < t_dia && t < t_out && t < t_end){
            real dt = dt_param * coll_time;
            evolve_step(snap, t, dt, epot, coll_time);
            nsteps++;
        }
        if (t >= t_dia){
            write_diagnostics(snap, t, epot, nsteps,
                              einit, false, x_flag);
            t_dia += dt_dia;
        }
        if (t >= t_out){
            put_snapshot(snap, t);
            t_out += dt_out;
        }
        if (t >= t_end)
            break;
    }
}

/*-----------------------------------------------------------------------------
 *  evolve_step  --  takes one integration step for an N-body system, using the
 *                   Hermite algorithm.
 *-----------------------------------------------------------------------------
 */

void evolve_step(nbody &snap, real &t, real dt, real & epot, real & coll_time)
{
    for (int i = 0; i < snap.size() ; i++)
        snap[i].update();

    predict_step(snap, dt);
    get_acc_jerk_pot_coll(snap, epot, coll_time);
    correct_step(snap, dt);
    t += dt;
}

/*-----------------------------------------------------------------------------
 *  predict_step  --  takes the first approximation of one Hermite integration
 *                    step, advancing the positions and velocities through a
 *                    Taylor series development up to the order of the jerks.
 *-----------------------------------------------------------------------------
 */

void predict_step(nbody &snap, real dt)
{
    for (int i = 0; i < snap.size() ; i++){
	snap[i].inc_pos( snap[i].get_vel() * dt ); 
	snap[i].inc_pos( snap[i].get_acc() * dt*dt/2 ); 
	snap[i].inc_pos( snap[i].get_jrk() * dt*dt*dt/6 ); 
	snap[i].inc_vel( snap[i].get_acc() * dt ); 
	snap[i].inc_vel( snap[i].get_jrk() * dt*dt/2 ); 
        }
}

/*-----------------------------------------------------------------------------
 *  correct_step  --  takes one iteration to improve the new values of position
 *                    and velocities, effectively by using a higher-order
 *                    Taylor series constructed from the terms up to jerk at
 *                    the beginning and the end of the time step.
 *-----------------------------------------------------------------------------
 */

void correct_step(nbody &snap, real dt)
{
    for (int i = 0; i < snap.size() ; i++){
	snap[i].set_vel( snap[i].get_old_vel() ); 
	snap[i].inc_vel((snap[i].get_old_acc()+snap[i].get_acc())*dt/2); 
	snap[i].inc_vel((snap[i].get_old_jrk()-snap[i].get_jrk())*dt*dt/12);
	snap[i].set_pos( snap[i].get_old_pos() ); 
	snap[i].inc_pos((snap[i].get_old_vel()+snap[i].get_vel())*dt/2); 
	snap[i].inc_pos((snap[i].get_old_acc()-snap[i].get_acc())*dt*dt/12);
        }
}

/*-----------------------------------------------------------------------------
 *  get_acc_jerk_pot_coll  --  calculates accelerations and jerks, and as side
 *                             effects also calculates potential energy and
 *                             the time scale coll_time for significant changes
 *                             in local configurations to occur.
 *                                                  __                     __
 *                                                 |          -->  -->       |
 *               M                           M     |           r  . v        |
 *   -->          j    -->       -->          j    | -->        ji   ji -->  |
 *    a   ==  --------  r    ;    j   ==  -------- |  v   - 3 ---------  r   |
 *     ji     |-->  |3   ji        ji     |-->  |3 |   ji      |-->  |2   ji |
 *            | r   |                     | r   |  |           | r   |       |
 *            |  ji |                     |  ji |  |__         |  ji |     __|
 *                             
 *  note: it would be cleaner to calculate potential energy and collision time
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
 */

void get_acc_jerk_pot_coll(nbody &snap, real & epot, real & coll_time)
{
    for (int i = 0; i < snap.size() ; i++){
        snap[i].clear_acc();
        snap[i].clear_jrk();
    }

    epot = 0;
    const real VERY_LARGE_NUMBER = 1e300;
    real coll_time_q = VERY_LARGE_NUMBER;      // collision time to 4th power
    real coll_est_q;                           // collision time scale estimate
                                               // to 4th power (quartic)
    for (int i = 0; i < snap.size() ; i++){
        for (int j = i+1; j < snap.size() ; j++){  // rji is the vector from
            space_vector rji;                      // particle i to particle j
            space_vector vji;                      // vji = d rji / d t
            rji = snap[j].get_pos() - snap[i].get_pos();
            vji = snap[j].get_vel() - snap[i].get_vel();
            real r2 = square(rji);                 // | rji |^2
            real v2 = square(vji);                 // | vji |^2
            real rv_r2 = (rji*vji)/r2;             // ( rij . vij ) / | rji |^2
            real r = sqrt(r2);                     // | rji |
            real r3 = r * r2;                      // | rji |^3

// add the {i,j} contribution to the total potential energy for the system:

            epot -= snap[i].get_mass() * snap[j].get_mass() / r;

// add the {j (i)} contribution to the {i (j)} values of acceleration and jerk:

            space_vector da;                          // main terms in pairwise
            space_vector dj;                          // acceleration and jerk
            da = rji / r3;                            // see equations
            dj = (vji - 3 * rv_r2 * rji) / r3;        // in the header

	    snap[i].inc_acc( snap[j].get_mass() * da );       // using symmetry
	    snap[j].inc_acc( - snap[i].get_mass() * da );     // find pairwise
	    snap[i].inc_acc( snap[j].get_mass() * dj );       // acceleration
	    snap[j].inc_acc( - snap[i].get_mass() * dj );     // and jerk

cout << "snap[i].get_acc() = " << snap[i].get_acc() << endl;
// first collision time estimate, based on unaccelerated linear motion:

            coll_est_q = (r2*r2) / (v2*v2);
            if (coll_time_q > coll_est_q)
                coll_time_q = coll_est_q;

// second collision time estimate, based on free fall:

            real da2 = square(da);
            double mij = snap[i].get_mass()+      // da2 becomes the square of
		         snap[j].get_mass();      // the pair-wise acceleration
            da2 *= mij * mij;                     // between particles i and j

            coll_est_q = r2/da2;
            if (coll_time_q > coll_est_q)
                coll_time_q = coll_est_q;
        }                                     
    }                                               // from q for quartic back
    coll_time = sqrt(sqrt(coll_time_q));            // to linear collision time
}                                             

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  nbody_sh1.C                                         /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
