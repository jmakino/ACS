//-----------------------------------------------------------------------------
// leapfrog1a.C
//-----------------------------------------------------------------------------
#include  <iostream>
#include  <cmath>
using namespace std;

int main()
{
    double r[3], v[3], a[3];
    double dt;

    cerr << "Please provide a value for the time step" << endl;

    cin >> dt;

    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
    v[0] = 0;
    v[1] = 0.5;
    v[2] = 0;

    double ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    double epot = -1.0/sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    double e_in = ekin + epot;

    cerr << "Initial total energy E_in = " << e_in << endl;

    double dt_out = 0.01;
    double t_out = dt_out;

    for (double t = 0; t < 10; t += dt){
        for (int k = 0; k < 3; k++){
            v[k] += 0.5 * a[k] * dt;
            r[k] += v[k] * dt;
        }
        double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
        for (int k = 0; k < 3; k++){
            a[k] = - r[k] / (r2 * sqrt(r2));
            v[k] += 0.5 * a[k] * dt;
        }
        if (t >= t_out){
            cout << r[0] << " " << r[1] << " " << r[2] << " ";
            cout << v[0] << " " << v[1] << " " << v[2] << endl;
            t_out += dt_out;
        }
    }

    ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    epot = -1.0/sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    double e_out = ekin + epot;

    cerr << "Final total energy E_out = " << e_out << endl;
    cerr << "absolute energy error: E_out - E_in = " << e_out - e_in << endl;
    cerr << "relative energy error: (E_out - E_in) / E_in = "
         << (e_out - e_in) / e_in << endl;
}
//-----------------------------------------------------------------------------
