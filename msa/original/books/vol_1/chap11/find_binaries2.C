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
 *  find_binaries2.C:  a tool to determine which pairs of particles are mutually
 *                    bound, i.e. form a (temporary) binary.
 *                    The program accepts a stream of N-body snapshots, and
 *                    outputs a list of binaries for each snapshot.
 *                
 *  note: in this first version, all functions are included in one file,
 *        without any use of a special library or header files.
 *_____________________________________________________________________________
 *
 *  usage: find_binaries2 [-h (for help)]
 * 
 *         Input/output are read/written from/to the standard i/o streams.
 *         Since there are no options, the code can simply be run by
 *         specifying the input file for the N-body snapshots:
 *
 *            find_binaries2 < data.in
 *
 *         This will produce data on the screen.  In order to capture the data
 *         in an output file "binarylist.out", use:
 *
 *            find_binaries2 < data.in > binarylist.out
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
 *     Output for each snapshot consists of a list of binaries, each one
 *     defined by the identity {id1,id2} of its members and its semimajor
 *     axis abin and eccentricity ebin, as follows:
 *
 *                      time = t
 *                      star1 = id1  star2 = id2  a = abin  e = ebin
 *
 *     Each line "time = t" is followed by zero, one, or more binary
 *     listings, depending on how many binaries there are found in the
 *     corresponding snapshot.
 *
 *  Internal data format:
 *
 *     The data for an N-body system is stored internally as 1-dimensional
 *     arrays for the masses and densities, and 2-dimensional arrays for the
 *     positions and velocities of all particles.
 *_____________________________________________________________________________
 *
 *    version 1:  Sep 2003   Piet Hut, Jun Makino
 *_____________________________________________________________________________
 */

#include  <iostream>
#include  <cmath>                          // to include sqrt(), etc.
#include  <cstdlib>                        // for atoi() and atof()
#include  <unistd.h>                       // for getopt()
using namespace std;

typedef double  real;                      // "real" as a general name for the
                                           // standard floating-point data type

const int NDIM = 3;                        // number of spatial dimensions

bool read_options(int argc, char *argv[], real & a_max);
int get_snapshot(real * * mass, real (* * pos)[NDIM], real (* * vel)[NDIM],
		 int & n, real & t);
void delete_snapshot(const real mass[], const real pos[][NDIM],
		     const real vel[][NDIM]);
bool find_binaries(real mass[], real pos[][NDIM], real vel[][NDIM], int n,
		   real t, real a_max);

/*-----------------------------------------------------------------------------
 *  main  --  reads option values, and starts a loop; in each round of the loop
 *            a new shapshot is read, the binaries are found and reported.
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    const real VERY_LARGE_NUMBER = 1e300;
    real a_max = VERY_LARGE_NUMBER;      // maximum value for semi-major axis a
                                 // of binaries, to be reported by this program
    if (! read_options(argc, argv, a_max))
        return 1;                // halt criterion detected by read_options()

    real * mass;                 // masses for all particles
    real (* pos)[NDIM];          // positions for all particles
    real (* vel)[NDIM];          // velocities for all particles

    int n;                       // N, number of particles in the N-body system
    real t;                      // time

    while(get_snapshot(&mass, &pos, &vel, n, t)){
        if (! find_binaries(mass, pos, vel, n, t, a_max))
	    return 1;
	delete_snapshot(mass, pos, vel);
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 *  read_options  --  reads the command line options, and implements them.
 *
 *  note: when the help option -h is invoked, the return value is set to false,
 *        to prevent further execution of the main program; similarly, if an
 *        unknown option is used, the return value is set to false.
 *-----------------------------------------------------------------------------
 */

bool read_options(int argc, char *argv[], real & a_max)
{
    int c;
    while ((c = getopt(argc, argv, "ha:")) != -1)
        switch(c){
            case 'a': a_max = atof(optarg);
                      break;
            case 'h':
            case '?': cerr << "usage: " << argv[0]
                           << " [-a maximum-value-for-semi-major-axis]"
                           << " [-h (for help)]"
                           << endl;
                      return false;      // execution stops after help or error
            }

    return true;                         // ready to continue program execution
}

/*-----------------------------------------------------------------------------
 *  get_snapshot  --  reads a single snapshot from the input stream cin
 *
 *  note: memory allocation for masses, positions and velocities is done here
 *        after reading in the number of particles (n).
 *        If the end of file is reached, get_snapshot() returns 0;
 *        after successful completion, get_snapshot() returns 1.
 *-----------------------------------------------------------------------------
 */

int get_snapshot(real * * mass, real (* * pos)[NDIM], real (* * vel)[NDIM],
		 int & n, real & t)
{
    cin >> n;
    if (cin.fail())
        return 0;
    cin >> t;

    *mass = new real[n];                  // masses for all particles
    *pos = new real[n][NDIM];             // positions for all particles
    *vel = new real[n][NDIM];             // velocities for all particles

    for (int i = 0; i < n ; i++){
        cin >> (*mass)[i];                       // mass of particle i
        for (int k = 0; k < NDIM; k++)
            cin >> (*pos)[i][k];                 // position of particle i
        for (int k = 0; k < NDIM; k++)
            cin >> (*vel)[i][k];                 // velocity of particle i
    }
    return 1;
}
    
/*-----------------------------------------------------------------------------
 *  delete_snapshot  --  frees up the memory that was allocated to the masses,
 *                       positions, and velocities for the particles in a
 *                       snapshot.
 *-----------------------------------------------------------------------------
 */

void delete_snapshot(const real mass[], const real pos[][NDIM],
		     const real vel[][NDIM])
{
    delete[] mass;
    delete[] pos;
    delete[] vel;
}

/*-----------------------------------------------------------------------------
 *  find_binaries  --  for each particle pair, check whether their relative
 *                     energy is negative.  If so, determine the semi-major
 *                     axis a and eccentricity e, and print those, but only
 *                     if a < a_max, the maximum value for a.  The values
 *                     for a and e are determined from the energy and angular
 *                     momentum of the particle pair, expressed in units of
 *                     their reduced mass.
 *-----------------------------------------------------------------------------
 */

bool find_binaries(real mass[], real pos[][NDIM], real vel[][NDIM], int n,
		   real t, real a_max)
{
    cout << "time = " << t << endl;
    for (int i = 0; i < n-1 ; i++){
	for (int j = i+1; j < n ; j++){
	    real delr[NDIM];
	    real delv[NDIM];
            real delr_sq = 0;
            real delv_sq = 0;
            for (int k = 0; k < NDIM ; k++){
		delr[k] = pos[j][k]-pos[i][k];
		delv[k] = vel[j][k]-vel[i][k];
                delr_sq += delr[k] * delr[k];
                delv_sq += delv[k] * delv[k];
	    }
	    real m_tot = mass[i] + mass[j];
	    real Etilde;        // energy per unit of reduced mass
	    Etilde = - m_tot / sqrt(delr_sq) + 0.5 * delv_sq;
	    if (Etilde < 0.0){
		real Ltilde_sq = 0.0;    // square of the value of the angular 
	                                 // momentum per unit of reduced mass
		if (NDIM == 2){
		    real r_cross_v_component = delr[0]*delv[1]-delr[1]*delv[0];
		    Ltilde_sq = r_cross_v_component * r_cross_v_component;
		}
		else if (NDIM == 3){
		    real r_cross_v_component = delr[0]*delv[1]-delr[1]*delv[0];
		    Ltilde_sq = r_cross_v_component * r_cross_v_component;
		    r_cross_v_component = delr[1]*delv[2]-delr[2]*delv[1];
		    Ltilde_sq += r_cross_v_component * r_cross_v_component;
		    r_cross_v_component = delr[2]*delv[0]-delr[0]*delv[2];
		    Ltilde_sq += r_cross_v_component * r_cross_v_component;
		}
		else {
		    cerr << "find_binaries: NDIM = " << NDIM
			 << "not supported " << endl;
		    return false;
		}
		real a = - 0.5 * m_tot / Etilde;
		real e_sq = 1 - Ltilde_sq/(m_tot * a);
		real e;
		if (e_sq > 0.0)
		    e = sqrt(e_sq);
		else               // in case of roundoff errors that may
		    e = 0.0;       // lead to e_sq being slightly negative
		
		if (a < a_max)
		    cout << "star1 = " << i << "  star2 = " << j
			 << "  a = " << a << "  e = " << e << endl;
	    }
	}
    }
    return true;
}

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  find_binaries2.C                                    /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
