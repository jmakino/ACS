//-----------------------------------------------------------------------------
// mk_pyth.C:  prints initial conditions for the Pythagorean problem.
//             ref.: Szebehely, V. and Peters, C.F., 1967, Astron. J. 72, 876.
//-----------------------------------------------------------------------------
#include  <iostream>
using namespace std;

typedef double  real;                      // "real" as a general name for the
                                           // standard floating-point data type
#define  NDIM     3                        // number of spatial dimensions

int main()
{
    const int n = 3;           // number of particles
    const real t = 0;          // time
    real r[n][NDIM];           // r[i][k] : position component k for particle i
    real v[n][NDIM];           // v[i][k] : velocity component k for particle i
    real m[n];                 // m[i] : mass for particle i

    m[0] = 3;
    m[1] = 4;
    m[2] = 5;

    r[0][0] = 1;
    r[0][1] = 3;
    r[0][2] = 0;

    r[1][0] = -2;
    r[1][1] = -1;
    r[1][2] = 0;

    r[2][0] = 1;
    r[2][1] = -1;
    r[2][2] = 0;

    for (int i = 0; i < n ; i++)
        for (int k = 0; k < NDIM; k++)
	    v[i][k] = 0;

    cout.precision(16);

    cout << n << endl;                      // first output line:  N
    cout << t << endl;                      // second output line:  time
    for (int i = 0; i < n; i++){
        cout << m[i];                       // each next output line:
        for (int k = 0; k < NDIM; k++)      //   m r_x r_y r_z v_x v_y v_z
            cout << ' ' << r[i][k];
        for (int k = 0; k < NDIM; k++)
            cout << ' ' << v[i][k];
        cout << endl;
    }
}
//-----------------------------------------------------------------------------
