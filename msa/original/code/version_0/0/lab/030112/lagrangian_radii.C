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
 *  lagrangian_radii.C:  a tool to determine a few Lagrangian radii for each
 *                       snapshot in a stream of N-body snapshots.
 *
 *  note: in this first version, all functions are included in one file,
 *        without any use of a special library or header files.
 *_____________________________________________________________________________
 *
 *  usage:  lagrangian_radii [-h (for help)] [-t]
 * 
 *          By default, only quartiles are returned: the Lagrangian radii
 *          for the radius of the inner 25% of the mass, the half-mass radius,
 *          and the radius of the outer quartile, enclosing 75% of the mass.
 *
 *          With the "-t" option added, the ten-percentiles are given instead:
 *          the values of the radii enclosing 10%, 20%, . . ., 90% of the mass.
 *
 *          Input/output are read/written from/to the standard i/o streams.
 *          Since all options have sensible defaults, the simplest way to run
 *          the code is by only specifying the i/o files for the N-body
 *          snapshots:
 *
 *             density < data.in > data.out
 *_____________________________________________________________________________

 *
 *  External data format:
 *
 *     The program expects input of a single snapshot of an N-body system,
 *     in the following format: the number of particles in the snapshot n;
 *     the time t; mass mi, position ri and velocity vi, and density rhoi
 *     for each particle i, with position and velocity given through their
 *     three Cartesian coordinates, divided over separate lines as follows:
 *
 *                      n
 *                      t
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z rho1
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z rho2
 *                      ...
 *                      mn rn_x rn_y rn_z vn_x vn_y vn_z rhon
 *
 *     Output of quartiles happens as follows, with r_q1(3) the radius of the
 *     inner(outer) quartile and r_h the half-mass radius:
 *
 *         t  r_q1  r_h  r_q3
 *
 *     Output of ten-percentiles is similarly given by the time, followed by a
 *     list of the radii r_t1, . . . , r_t9  for the inner through the outer
 *     ten-percentile radii:
 *
 *         t  r_t1 r_t2 r_t3 . . . r_t9
 *
 *  Internal data format:
 *
 *     The data for an N-body system is stored internally as 1-dimensional
 *     arrays for the masses and densities, and 2-dimensional arrays for the
 *     positions and velocities of all particles.
 *_____________________________________________________________________________
 *
 *    version 1:  Nov 2002   Piet Hut, Jun Makino
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

bool read_options(int argc, char *argv[], bool & t);
int get_snapshot(real * *mass, real (* (*pos))[NDIM], real (* (*vel))[NDIM],
		 real * *dens, int & n, real & t);
void delete_snapshot(const real mass[], const real pos[][NDIM],
		     const real vel[][NDIM], const real dens[]);
void lagrangian_radii(real mass[], real pos[][NDIM], int n, real t, bool tenp);

/*-----------------------------------------------------------------------------
 *  main  --  reads option values, and starts a loop.  In each round of the
 *            loop a new shapshot is read, and all particle positions are
 *            shifted to a new coordinate system centered on the particle with
 *            the highest density value (the velocities are not changed), and
 *            the shifted snapshot is written out.
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    bool tenp;         // if true, use ten-percentiles; otherise, use quartiles

    if (! read_options(argc, argv, tenp))
        return 1;                // halt criterion detected by read_options()

    real * mass;                 // masses for all particles
    real (* pos)[NDIM];          // positions for all particles
    real (* vel)[NDIM];          // velocities for all particles
    real * dens;                 // density values for all particles

    int n;                       // N, number of particles in the N-body system
    real t;                      // time

    while(get_snapshot(&mass, &pos, &vel, &dens, n, t)){
        lagrangian_radii(mass, pos, n, t, tenp);
	delete_snapshot(mass, pos, vel, dens);
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

bool read_options(int argc, char *argv[], bool & tenp)
{
    int c;
    tenp = false;             // default

    while ((c = getopt(argc, argv, "ht")) != -1)
        switch(c){
            case 'h': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-t (for ten-percentiles)]"
                           << endl;
                      return false;         // execution should stop after help
            case 't': tenp = true;
                      break;
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-t (for ten-percentiles)]"
                           << endl;
                      return false;        // execution should stop after error
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

int get_snapshot(real * *mass, real (* (*pos))[NDIM], real (* (*vel))[NDIM],
		 real * *dens, int & n, real & t)
{
    cin >> n;
    if (cin.fail())
        return 0;
    cin >> t;

    *mass = new real[n];                  // masses for all particles
    *pos = new real[n][NDIM];             // positions for all particles
    *vel = new real[n][NDIM];             // velocities for all particles
    *dens = new real[n];                  // density values for all particles

    for (int i = 0; i < n ; i++){
        cin >> (*mass)[i];                       // mass of particle i
        for (int k = 0; k < NDIM; k++)
            cin >> (*pos)[i][k];                 // position of particle i
        for (int k = 0; k < NDIM; k++)
            cin >> (*vel)[i][k];                 // velocity of particle i
        cin >> (*dens)[i];                       // density for particle i
    }
    return 1;
}
    
/*-----------------------------------------------------------------------------
 *  delete_snapshot  --  frees up the memory that was allocated to the masses,
 *                       positions, velocities and density values for the
 *                       particles in a snapshot.
 *-----------------------------------------------------------------------------
 */

void delete_snapshot(const real mass[], const real pos[][NDIM],
		     const real vel[][NDIM], const real dens[])
{
    delete[] mass;
    delete[] pos;
    delete[] vel;

    delete[] dens;
}

/*-----------------------------------------------------------------------------
 *  rm_pair  --  a structure that binds two real numbers, the mass m and the
 *               distance to the center r for a single particle.
 *               "rm_pair_ptr" is an abbreviation for a pointer to "rm_pair"
 *-----------------------------------------------------------------------------
 */

typedef  struct
{
    real  radius_sq;
    real  mass;
} rm_pair, *rm_pair_ptr;

/*-----------------------------------------------------------------------------
 *  compare_radii  --  compare the distances to the center for two particles
 *-----------------------------------------------------------------------------
 */

int compare_radii(const void * pi, const void * pj)
{
    if (((rm_pair_ptr) pi)->radius_sq > ((rm_pair_ptr) pj)->radius_sq)
        return +1;
    else if (((rm_pair_ptr)pi)->radius_sq < ((rm_pair_ptr)pj)->radius_sq)
        return -1;
    else
        return 0;
}

/*-----------------------------------------------------------------------------
 *  lagrangian_radii  --  for each N-body snapshot, print on one line the time,
 *                        followed by the quartiles or ten-percentiles for the
 *                        Lagrangian radii (depending on whether the option
 *                        "tenp" for ten-percentiles is true or false).
 *-----------------------------------------------------------------------------
 */

void lagrangian_radii(real mass[], real pos[][NDIM], int n, real t, bool tenp)
{
    rm_pair_ptr rm_table = new rm_pair[n];

    for (int i = 0; i < n ; i++){
	real r_sq = 0;
	for (int k = 0; k < NDIM ; k++)
	    r_sq += (pos[i][k] * pos[i][k]);
	rm_table[i].radius_sq = r_sq;
	rm_table[i].mass = mass[i];
    }
		     
    qsort((void *)rm_table, (size_t)n, sizeof(rm_pair), compare_radii);

    // we now replace the masses in the table by cumulative masses: each radius
    // will be associated with the mass inclosed by that radius.

    for (int i = 1; i < n ; i++)
	rm_table[i].mass += rm_table[i-1].mass;
    real total_mass = rm_table[n-1].mass;

    const real LARGER_THAN_ONE = 2;
    real quartile_mass_fractions[] = {0.25 , 0.5 , 0.75, LARGER_THAN_ONE};
    real tenpercentile_mass_fractions[] =
           {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, LARGER_THAN_ONE};

    real * mass_fraction;
    if (tenp)                                         // ten-percentiles
	mass_fraction = tenpercentile_mass_fractions;
    else                                              // quartiles
	mass_fraction = quartile_mass_fractions;

    cout << t;
    for (int i = 0; i < n ; i++){
	while (rm_table[i].mass >= total_mass * (* mass_fraction)){
	    cout << "  " << sqrt(rm_table[i].radius_sq);
	    mass_fraction++;
	}
    }
    cout << endl;
}

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  lagrangian_radii.C                                  /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
