              // Time-stamp: <2003-09-15 14:08:34 piet>
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
 *  sphere.C:  generates initial conditions for a cold sphere
 *
 *_____________________________________________________________________________
 *
 *  usage: sphere -n number_of_particles
 *                    [-h (for help)] [-s random_number_generator_seed]
 * 
 *         The number of particles has to be specified, since there is no
 *         natural default value.  If no seed is specified for the random
 *         number generator, a random value for the seed is chosen which
 *         depends on the unix clock and will be different every second.
 *
 *         Example:  "sphere -n 3 > data.out"  will produce a file in the
 *         following format, for particle number, time, and masses mi,
 *         positions ri, and velocities vi for particles i:
 *
 *                      3
 *                      0
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z
 *                      m3 r3_x r3_y r3_z v3_x v3_y v3_z
 *_____________________________________________________________________________
 *
 *  Our sphere has unit radius and constant density.  The total mass is unity,
 *  so each particle will have mass 1/N, for N particles in total.  Each
 *  particle will be sprinkled somewhere randomly within the unit sphere, with
 *  initial velocity zero.
 *_____________________________________________________________________________
 *
 *    version 1:  Jan 2003   Piet Hut, Jun Makino
 *_____________________________________________________________________________
 */

#include  <iostream>
#include  <cmath>                   // to include sqrt(), etc.
#include  <cstdlib>                 // formerly <cstdlib.h>; for atoi(), atof()
                                    // and rand(), srand()
#include  <unistd.h>                // for getopt()
#include  <time.h>                  // for time()
using namespace std;

typedef double  real;                      // "real" as a general name for the
                                           // standard floating-point data type

const int NDIM = 3;                        // number of spatial dimensions

bool read_options(int argc, char *argv[], int &n, int &seed);
real randunit(int seed);
real randinter(real a, real b);
bool sphere(real mass[], real pos[][NDIM], real vel[][NDIM], int n);
void put_snapshot(real mass[], real pos[][NDIM],
                  real vel[][NDIM], int n, real t);

/*-----------------------------------------------------------------------------
 *  main  --  read in option values, invoke the model builder
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    int n = 0;                        // N, number of particles; start with an
    				      // unphysical value, to allow us to force
    				      // the user to make a definite choice.
    int seed = 0;                     // seed for the random number generator;
    				      // a default of zero will be replaced by
    				      // the time taken from the unix clock.

    if (! read_options(argc, argv, n, seed))
        return 1;

    if (n <= 0){
	cerr << "a value of N = " << n << " is not allowed." << endl;
	return 1;
    }

    if (seed == 0)       /* no particular positive seed provided?            */
        seed = time(0);  /* then give a random value, different every second */

    cerr << "seed = " << seed << endl;

    randunit(seed);

    real * mass = new real[n];                  // masses for all particles
    real (* pos)[NDIM] = new real[n][NDIM];     // positions for all particles
    real (* vel)[NDIM] = new real[n][NDIM];     // velocities for all particles

    if (! sphere(mass, pos, vel, n))
	return 1;
    put_snapshot(mass, pos, vel, n, 0);
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

bool read_options(int argc, char *argv[], int &n, int &seed)
{
    int c;
    while ((c = getopt(argc, argv, "hn:s:")) != -1)
        switch(c){
            case 'n': n = atoi(optarg);
                      break;
            case 's': seed = atoi(optarg);
                      break;
            case 'h':
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)] [-n number_of_particles]\n"
                           << "         [-s random_number_generator_seed]"
                           << endl;
                      return false;      // execution stops after help or error
            }

    return true;                         // ready to continue program execution
}

/*-----------------------------------------------------------------------------
 *  randunit  --  returns a random real number within the unit interval
 *                note: based on      @(#)rand.c   4.1 (Berkeley) 12/21/80,
 *                      but returning a positive number smaller than unity.
 *
 *  note: to initialize the random number generator, invoke it with an nonzero
 *        argument, which will then become the seed;
 *        to run the random number generator, invoke it with argument 0.
 *-----------------------------------------------------------------------------
 */
real randunit(int seed)
{
    const real MAXN = 2147483647;  // the maximum value which rand() can return

    static int randx;

    if (seed)
        {
        randx = seed;
        return(0.0);        // to make the compiler happy, we return a value,
        }                   // even though it will not be used in this case
    else
        return((real)((randx = randx * 1103515245 + 12345) & 0x7fffffff)/MAXN);
}

/*-----------------------------------------------------------------------------
 *  randinter  --  returns a random real number within an interval [a,b]
 *                 by invoking  randunit() .
 *-----------------------------------------------------------------------------
 */
real  randinter(real a, real b)
    {
    return(a + (b-a)*randunit(0));
    }

/*-----------------------------------------------------------------------------
 *  sphere  --  constructs a homogeneous sphere, with unit radius and unit
 *              total mass, and particle velocities zero.
 *-----------------------------------------------------------------------------
 */

bool sphere(real mass[], real pos[][NDIM], real vel[][NDIM], int n)
{
    if (NDIM != 3){
	cerr << "sphere: NDIM = " << NDIM << "not supported " << endl;
	return false;
    }

    for (int i = 0; i < n; i++)
        mass[i] = 1.0 / (real) n;

    const real PI = 3.14159265358979323846;

    for (int i = 0; i < n; i++){
	real r = pow(randinter(0.0, 1.0), 1.0/3.0);
	real theta = randinter(0.0, PI);
	real phi = randinter(0.0, 2*PI);
	pos[i][0] = r*sin(theta)*cos(phi);
	pos[i][1] = r*sin(theta)*sin(phi);
	pos[i][2] = r*cos(theta);
	for (int k = 0; k < NDIM; k++)
	    vel[i][k] = 0;
    }
    return true;
}

/*-----------------------------------------------------------------------------
 *  put_snapshot  --  write a single snapshot on the output stream cout, in
 *                    the same format as described above for get_snapshot.
 *
 *  note: we use "const" here for the arguments, since they are not intended
 *        to be altered by a call to put_snapshot.
 *-----------------------------------------------------------------------------
 */

void put_snapshot(real mass[], real pos[][NDIM],
                  real vel[][NDIM], int n, real t)
{
    cout.precision(16);

    cout << n << endl;
    cout << t << endl;
    for (int i = 0; i < n ; i++){
        cout << mass[i];
        for (int k = 0; k < NDIM; k++)
            cout << ' ' << pos[i][k];
        for (int k = 0; k < NDIM; k++)
            cout << ' ' << vel[i][k];
        cout << endl;
    }
}
    
/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  sphere.C                                            /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
