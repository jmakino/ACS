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
 *  halo_slope.C:  a tool to make a simple estimate of the slope of the density
 *                 profile in the halo.
 *                
 *  note: in this first version, all functions are included in one file,
 *        without any use of a special library or header files.
 *_____________________________________________________________________________
 *
 *  usage: halo_slope [-h (for help)]
 * 
 *         Input is read from the standard input stream.
 *         The simplest way to run the code:
 *
 *            density < data.in
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
 *     Output of each snapshot is written according to the same format,
 *     but with the density rho appended at the end of each line:
 *
 *                      n
 *                      t
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z rho1
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z rho2
 *                      ...
 *                      mn rn_x rn_y rn_z vn_x vn_y vn_z rhon
 *
 *  Internal data format:
 *
 *     The data for an N-body system is stored internally as 1-dimensional
 *     arrays for the masses and densities, and 2-dimensional arrays for the
 *     positions and velocities of all particles.
 *_____________________________________________________________________________
 *
 *    version 1:  Jan 2003   Piet Hut, Jun Makino
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

bool read_options(int argc, char *argv[], int & k);
int get_snapshot(real * * mass, real (* * pos)[NDIM], real (* * vel)[NDIM],
		 int & n, real & t);
void put_snapshot(const real mass[], const real pos[][NDIM],
                  const real vel[][NDIM], const real dens[], int n, real t);
void delete_snapshot(const real mass[], const real pos[][NDIM],
		     const real vel[][NDIM], const real dens[]);









bool density(real mass[], real pos[][NDIM], real * * dens, int n, int k);

/*-----------------------------------------------------------------------------
 *  main  --  reads option values, and starts a loop; in each round of the loop
 *            a new shapshot is read, the densities of all particles in the
 *            snapshot are calculated, and the snapshot with densities are
 *            written out.
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    int  k;     // the kth nearest neighbor to a particle p is used to estimate
                // the density around particle p

    if (! read_options(argc, argv, k))
        return 1;                // halt criterion detected by read_options()

    real * mass;                 // masses for all particles
    real (* pos)[NDIM];          // positions for all particles
    real (* vel)[NDIM];          // velocities for all particles

    int n;                       // N, number of particles in the N-body system
    real t;                      // time

    real * dens;                 // density values for all particles

    while(get_snapshot(&mass, &pos, &vel, n, t)){
        if (! density(mass, pos, &dens, n, k))
	    return 1;
	put_snapshot(mass, pos, vel, dens, n, t);
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

bool read_options(int argc, char *argv[], int & k)
{
    int c;
    while ((c = getopt(argc, argv, "hk:")) != -1)
        switch(c){
            case 'h': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-k number-of-nearest-neighbors]"
                           << endl;
                      return false;         // execution should stop after help
            case 'k': k = atoi(optarg);
                      break;
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)]"
                           << " [-k number-of-nearest-neighbors]"
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
 *  put_snapshot  --  writes a single snapshot on the output stream cout,
 *                    including the density values for all particles.
 *-----------------------------------------------------------------------------
 */

void put_snapshot(const real mass[], const real pos[][NDIM],
                  const real vel[][NDIM], const real dens[], int n, real t)
{
    cout.precision(16);                       // full double precision

    cout << n << endl;                        // N, total particle number
    cout << t << endl;                        // current time
    for (int i = 0; i < n ; i++){
        cout << mass[i];                      // mass of particle i
        for (int k = 0; k < NDIM; k++)
            cout << ' ' << pos[i][k];         // position of particle i
        for (int k = 0; k < NDIM; k++)
            cout << ' ' << vel[i][k];         // velocity of particle i
        cout << ' ' << dens[i];               // density value for particle i
        cout << endl;
    }
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
 *  density  --  determine the density around each particle i by constructing
 *               a neighbor list, to be precise a list of squared distances
 *               to the k particles that are nearest to particle i, with a
 *               parallel list containing the masses of those particles.
 *               The Casertano-Hut density estimator is obtained by dividing
 *               the local mass in a sphere with a radius equal to the kth
 *               nearest neighbor distance by the volume within that radius,
 *               corrected for the statistical bias.
 *
 *  note: the basic algorithm is described in Casertano, S. & Hut, P., 1985,
 *        Astrophysical Journal 298, 80-94, for equal mass particles.
 *        We here generalize their algorithm for arbitrary particle masses,
 *        by introducing a volume correction factor of (k + 1/2) / (k - 1)
 *        (see their figure 1, which also shows that we have to count half of
 *        the mass of the kth nearest neighbor to determine the local mass).
 *-----------------------------------------------------------------------------
 */

bool density(real mass[], real pos[][NDIM], real * * dens, int n, int k)
{
    *dens = new real[n];                    // density values for all particles
    real *neighbor_dist_sq = new real[k];
    real *neighbor_mass = new real[k];
    const real VERY_LARGE_NUMBER = 1e300;
    const real PI = 3.14159265358979323846;

    if (k >= n){                             // no k-th nearest neighbor exists
        cerr << "density: k = " << k << " >= n = " << n << endl;
	return false;
    }
    if (k <= 1){                                    // k >= 2 in this algorithm
        cerr << "density: k = " << k << " <= 1" << endl;
	return false;
    }

    for (int i = 0; i < n ; i++){
	for (int p = 0; p < k; p++)
	    neighbor_dist_sq[p] = VERY_LARGE_NUMBER;

	for (int j = 0; j < n ; j++){
	    if (i == j)                            // particle i does not count
	        continue;                          // as its own neighbor

            real delr_sq = 0;
            for (int kk = 0; kk < NDIM ; kk++)
                delr_sq += (pos[j][kk]-pos[i][kk]) * (pos[j][kk]-pos[i][kk]);

	    int p = k-1;
	    while (p >= 0 && delr_sq < neighbor_dist_sq[p]){
	        if (p < k-1){
	            neighbor_dist_sq[p+1] = neighbor_dist_sq[p];  // shift
		    neighbor_mass[p+1] = neighbor_mass[p];       // list values
		}
		p--;
	    }
	    if (p < k-1){
	        neighbor_dist_sq[p+1] = delr_sq;           // insert new values
		neighbor_mass[p+1] = mass[j];              // in neighbor lists
	    }
	}
	real volume = (4.0/3.0) * PI * pow(neighbor_dist_sq[k-1], 1.5);
        real local_mass = mass[i];
        for (int p = 0; p < k-1; p++)
	    local_mass += neighbor_mass[p];
        local_mass += 0.5*mass[k-1];

	real volume_correction_factor = (k+0.5)/(k-1.0);
        (*dens)[i] = local_mass / (volume * volume_correction_factor);
    }

    delete neighbor_dist_sq;
    delete neighbor_mass;

    return true;
}

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  density.C                                           /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
