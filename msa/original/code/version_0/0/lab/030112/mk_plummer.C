              // Time-stamp: <2003-01-12 11:03:14 piet>
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
 *  mk_plummer1.C:  generates initial conditions for a Plummer model
 *
 *                  ref.: Aarseth, S.J., Henon, M. and Wielen, R., 1974,
 *                          Astron. and Astrophys. 37, p. 183.
 *                        Binney, J. and Tremaine, S., 1987, Galactic Dynamics
 *                          (Princeton: Princeton Univ. Press), pp. 42-43, 225.
 *_____________________________________________________________________________
 *
 *  usage: mk_plummer -n number_of_particles
 *                    [-h (for help)] [-s random_number_generator_seed]
 * 
 *         The number of particles has to be specified, since there is no
 *         natural default value.  If no seed is specified for the random
 *         number generator, a random value for the seed is chosen which
 *         depends on the unix clock and will be different every second.
 *
 *         Example:  "mk_plummer -n 3 > data.out"  will produce a file in
 *         the following format, for particle number, time, and masses mi,
 *         positions ri, and velocities vi for particles i:
 *
 *                      3
 *                      0
 *                      m1 r1_x r1_y r1_z v1_x v1_y v1_z
 *                      m2 r2_x r2_y r2_z v2_x v2_y v2_z
 *                      m3 r3_x r3_y r3_z v3_x v3_y v3_z
 *_____________________________________________________________________________
 *
 *  A Plummer model is characterized by a density rho as a function of radius r
 *
 *                    M               1
 *                                                                  4      3
 *     rho (r)  =  -------  ----------------------     ;     V  =  --- pi r
 *                            __           __  5/2            o     3      o
 *                    V      |          2    |
 *                     o     |         r     |
 *                           |  1  +  ----   |
 *                           |          2    |
 *                           |         r     |
 *                           |__        o  __|
 *
 *
 *  Here  r  is the structural length, V  the associated structural volume, and
 *         o                            o
 *  M the total mass.  Note that the central density is what one would obtain
 *  by distributing the total mass homogenously over the structural volume, and
 *                                                                5/2
 *  that for large r the density approaches  rho(r) = (M/V )(r /r)
 *                                                        o   o
 *.............................................................................
 *
 *  For our Plummer model realization, we choose units in which G = M = r  = 1
 *                                                                       o
 *  where G is the gravitational constant.  The resultant particle realization
 *  is not transformed to a center-of-mass frame, nor is it transformed to
 *  standard virial units.  Other tools can be used for those transformations.
 *_____________________________________________________________________________
 *
 *    version 1:  Jan 2002   Piet Hut, Jun Makino
 *    version 2:  Jan 2003   Piet Hut, Jun Makino
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
#define  NDIM     3                        // number of spatial dimensions

void begin_put_snapstream();
void end_put_snapstream();
bool read_options(int argc, char *argv[], int &n, int &seed);
real randunit(int seed);
real randinter(real a, real b);
void mk_plummer(real mass[], real pos[][NDIM], real vel[][NDIM], int n);
void to_com(real mass[], real pos[][NDIM], real vel[][NDIM], int n);
void put_snapshot(real mass[], real pos[][NDIM],
                  real vel[][NDIM], int n, real t);

/*-----------------------------------------------------------------------------
 *  main  --  read in option values, invoke the Plummer model builder
 *-----------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
    int n = 1;                   // N, number of particles
    int seed = 0;                // seed for the random number generator

    if (! read_options(argc, argv, n, seed))
        return 1;

    if (seed == 0)       /* no particular positive seed provided?            */
        seed = time(0);  /* then give a random value, different every second */

    cerr << "seed = " << seed << endl;

    randunit(seed);

    real mass[n];                // masses for all particles
    real pos[n][NDIM];           // positions for all particles
    real vel[n][NDIM];           // velocities for all particles

    mk_plummer(mass, pos, vel, n);

    to_com(mass, pos, vel, n);

    begin_put_snapstream();
    put_snapshot(mass, pos, vel, n, 0);
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

bool read_options(int argc, char *argv[], int &n, int &seed)
{
    int c;
    while ((c = getopt(argc, argv, "hn:s:")) != -1)
        switch(c){
            case 'h': cout << "usage: " << argv[0]
                           << " [-h (for help)] [-n number_of_particles]\n"
                           << "         [-s random_number_generator_seed]"
                           << endl;
                      return false;       // execution should stop after help
            case 'n': n = atoi(optarg);
                      break;
            case 's': seed = atoi(optarg);
                      break;
            case '?': cerr << "usage: " << argv[0]
                           << " [-h (for help)] [-n number_of_particles]\n"
                           << "         [-s random_number_generator_seed]"
                           << endl;
                      return false;      // execution should stop after error
            }

    return true;                         // ready to continue program execution
}

/*-----------------------------------------------------------------------------
 *  randunit  --  returns a random real number within the unit interval
 *                note: based on      @(#)rand.c   4.1 (Berkeley) 12/21/80,
 *                      but returning a positive number smaller than unity.
 *-----------------------------------------------------------------------------
 */
real randunit(int seed)
{
    const real MAXN = 2147483647;  // the maximum value which rand() can return

    static int randx;

    if (seed)
        {
        randx = seed;
        return(0);        // to make the compiler happy, we return a real value
        }
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
 *  mk_plummer  --  constructs a Plummer model, as an N-body shapshot, using
 *                  units in which G = M = r  = 1
 *                                          o
 *  The position of each particle is constructed using the accumulative mass
 *  function 
 *         
 *         _ R                       _ R 
 *        |         2               |      2      2 -5/2         3      2 -3/2
 *  m  =  |   4 pi r  rho(r) dr  =  |   3 r (1 + r )     dr  =  R (1 + R )
 *   R   _|                        _|
 *           0                         0
 *
 *  Inverting this relation, we find the radius R  associated with each value
 *                                               m
 *  for the accumulative mass:
 *                                          -2/3      -1/2
 *                                 R  =  ( m     - 1 )
 *                                  m
 *
 *  We use this relationship to determine the radial distance of each particle
 *  starting from random values for m.  The simplest approach would be to pick
 *  a new m value randomly between 0 and 1 for each particle.  However, for a
 *  few particle system such an approach could lead to large fluctuations such
 *  as an absence of particles in the center or in the outskirts of the system.
 *  In an attempt to minimize such statistical fluctuations we first divide the
 *  system into N equal-mass shells, after which we drop one particle randomly
 *  placed inside each of the N shells.  The two spherical angles are choosen
 *  at random, to complete the three-dimensional positioning of each particle.
 *  
 *  Choosing the magnitude of the velocity is a bit more complicated.  We use
 *  von Neumann's rejection technique, cf. Aarseth et al. (1974), eq. (A4,5).
 *  We take random number pairs (x,y) with x uniformly in [0,1] and y uniformly
 *  in [0, 0.1].  If
 *                                2          2  7/2
 *                    y < g(x) = x  ( 1.0 - x  )
 *
 *  we keep that (x,y) value to construct a velocity value; if not, we keep
 *  spinning the random number generator until we find such a pair.  The
 *  resulting value x multiplied by the local escape velocity  v  gives the
 *  velocity v for the particle:                                e
 *
 *                      1/2          2  -1/4
 *      v  =  x v   =  2    x ( 1 + r  )
 *               e
 *
 *  where r is the radial position of the particle.  The angles are determined
 *  for the isotropic velocity distribution as they were for the positions:
 *  uniformly over the unit sphere, expressed in terms of spherical coordinates
 *-----------------------------------------------------------------------------
 */

void mk_plummer(real mass[], real pos[][NDIM], real vel[][NDIM], int n)
{
    const real pi = 2 * asin(1.0);
/*
 *  Calculating the coordinates is easiest in STRUCTURAL units;
 *  conversion to VIRIAL units will be performed below.
 *
 *    Recipe for scaling to the proper system of units:
 *
 *  Since G = M = 1, if we switch from a coordinate system with
 *  length unit  r_old  to a coordinate system with length unit  r_new ,
 *  the length units simply scale by a factor  C = r_new / r_old .
 *  Consequently, the coordinate values of physical quantities
 *  such as positions should transform inversely to maintain the same
 *  coordinate-invariant meaning. Similarly, the square of the velocities
 *  should transform inversely proportional to the positions,
 *  since  GM = 1  (cf. a relation such as  v*v = G*M/r ).
 *  To summarize: If
 *                       r_unit(new) = C * r_unit(old)  ,
 *                then
 *                       pos(new) = (1/C) * pos(old)
 *                and
 *                       vel(new) = sqrt(C) * vel(old)  .
 */
    real scalefactor = 16.0 / (3.0 * pi);
    real inv_scalefactor = 1.0 / scalefactor;
    real sqrt_scalefactor = sqrt( scalefactor );
/*
 *  the position coordinates are determined by inverting the cumulative
 *  mass-radius relation, with the cumulative mass drawn randomly from
 *  [0, mfrac]; cf. Aarseth et al. (1974), eq. (A2).
 */
    for (int i = 0; i < n; i++)
        mass[i] = 1.0 / (real) n;

    for (int i = 0; i < n; i++){
        real m_min = (real) i / (real) n;
        real m_max = (real) (i+1) / (real) n;
        real radius = 1.0 / sqrt( pow (randinter(m_min,m_max),-2.0/3.0) - 1.0);
        real theta = acos(randinter(-1.0, 1.0));
        real phi = randinter(0.0, 2*pi);
        pos[i][0] = radius * sin( theta ) * cos( phi );
        pos[i][1] = radius * sin( theta ) * sin( phi );
        pos[i][2] = radius * cos( theta );
/*
 *  the velocity coordinates are determined using von Neumann's rejection
 *  technique, cf. Aarseth et al. (1974), eq. (A4,5).
 *  First we take initial values for x, the ratio of velocity and escape
 *  velocity (q in Aarseth et al.), and y, as a trick to enter the body of the
 *  while loop.
 */
        real x = 0.0;
        real y = 0.1;
/*
 *  Then we keep spinning the random number generator until we find a pair
 *  of values (x,y), so that y < g(x) = x*x*pow( 1.0 - x*x, 3.5) . Whenever
 *  an y-value lies above the g(x) curve, the (x,y) pair is discarded, and
 *  a new pair is selected. The value 0.1 is chosen as a good upper limit for
 *  g(x) in [0,1] : 0.1 > max g(x) = 0.092 for 0 < x < 1.
 */
        while (y > x*x*pow( 1.0 - x*x, 3.5)){
            x = randinter(0.0,1.0);
            y = randinter(0.0,0.1);
        }
/*
 *  If y < g(x), proceed to calculate the velocity components:
 */
        real velocity = x * sqrt(2.0) * pow( 1.0 + radius*radius, -0.25);
        theta = acos(randinter(-1.0, 1.0));
        phi = randinter(0.0, 2*pi);
        vel[i][0] = velocity * sin( theta ) * cos( phi );
        vel[i][1] = velocity * sin( theta ) * sin( phi );
        vel[i][2] = velocity * cos( theta );
/*
 * Now transform to the VIRIAL coordinates by applying
 * the scaling factors to the positions and velocities:
 */
        for (int k = 0; k < NDIM; k++){
        pos[i][k] *= inv_scalefactor;
        vel[i][k] *= sqrt_scalefactor;
        }
    }
}

/*-----------------------------------------------------------------------------
 *  begin_put_snapstream  --  writes the first part of an XLM document that
 *                            will contain a stream of snapshots
 *-----------------------------------------------------------------------------
 */

void begin_put_snapstream()
{
    cout << "<?xml version=\"1.0\"?>" << endl;
    cout << "<nbody-snapstream version=\"0.1\">" << endl;
}
    
/*-----------------------------------------------------------------------------
 *  end_put_snapstream  --  writes the last part of an XLM document that
 *                          will contain a stream of snapshots
 *-----------------------------------------------------------------------------
 */

void end_put_snapstream()
{
    cout << "</nbody-snapstream>" << endl;
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

    cout << "<nbody-snapshot>" << endl;

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
    cout << "</nbody-snapshot>" << endl;
}
    
/*-----------------------------------------------------------------------------
 *  to_com  --  transforms an N-body snapshot to center-of-mass frame.
 *-----------------------------------------------------------------------------
 */

void to_com(real mass[], real pos[][NDIM], real vel[][NDIM], int n)
{
    real mass_com;                        // total mass of the n-body system
    real pos_com[NDIM];                   // position of center of mass
    real vel_com[NDIM];                   // velocity of center of mass

    mass_com = 0;
    for (int k = 0; k < NDIM ; k++)
        pos_com[k] = vel_com[k] = 0;

    for (int i = 0; i < n ; i++){
        mass_com += mass[i];
        for (int k = 0; k < NDIM ; k++){
            pos_com[k] += mass[i] * pos[i][k];
            vel_com[k] += mass[i] * vel[i][k];
        }
    }

    for (int k = 0; k < NDIM ; k++){
        pos_com[k] /= mass_com;
        vel_com[k] /= mass_com;
    }

    for (int i = 0; i < n ; i++)
        for (int k = 0; k < NDIM ; k++){
            pos[i][k] -= pos_com[k];
            vel[i][k] -= vel_com[k];
        }
}

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  mk_plummer1.C                                       /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
