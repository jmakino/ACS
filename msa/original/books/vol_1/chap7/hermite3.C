//-----------------------------------------------------------------------------
// hermite3.C
//-----------------------------------------------------------------------------
#include  <iostream>
#include  <cmath>
#include  <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
    double dt = atof(argv[1]);
    double t_end = atof(argv[2]);

    int n;
    cin >> n;

    double t;
    cin >> t;

    double * m = new double[n];
    double (* r)[3] = new double[n][3];
    double (* v)[3] = new double[n][3];
    double (* a)[3] = new double[n][3];
    double (* jk)[3] = new double[n][3];

    for (int i = 0; i < n ; i++){
        cin >> m[i];
        for (int k = 0; k < 3; k++)
            cin >> r[i][k];
        for (int k = 0; k < 3; k++)
            cin >> v[i][k];
    }

    for (int i = 0; i < n; i++)
        for (int k = 0; k < 3; k++)
            a[i][k] = jk[i][k] = 0.0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rji[3], vji[3];
            for (int k = 0; k < 3; k++){
                rji[k] = r[j][k] - r[i][k];
                vji[k] = v[j][k] - v[i][k];
            }
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rji[k] * rji[k];
            double r3 = r2 * sqrt(r2);
            double rv = 0;
            for (int k = 0; k < 3; k++)
                rv += rji[k] * vji[k];
            rv /= r2;
            for (int k = 0; k < 3; k++){
                a[i][k] += m[j] * rji[k] / r3;
                a[j][k] -= m[i] * rji[k] / r3;
                jk[i][k] += m[j] * (vji[k] - 3 * rv * rji[k]) / r3;
                jk[j][k] -= m[i] * (vji[k] - 3 * rv * rji[k]) / r3;
            }
        }
    }

    double ekin = 0, epot = 0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rji[3];
            for (int k = 0; k < 3; k++)
                rji[k] = r[j][k] - r[i][k];
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rji[k] * rji[k];
            double r = sqrt(r2);
            epot -= m[i]*m[j]/r;
        }
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m[i] * v[i][k] * v[i][k];
    }
    double e_in = ekin + epot;
    cerr << "Initial total energy E_in = " << e_in << endl;

    double dt_out = 0.01;
    double t_out = dt_out;

    double old_r[n][3], old_v[n][3], old_a[n][3], old_j[n][3];

    for (double t = 0; t < t_end; t += dt){
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++){
                old_r[i][k] = r[i][k];
                old_v[i][k] = v[i][k];
                old_a[i][k] = a[i][k];
                old_j[i][k] = jk[i][k];
                r[i][k] += v[i][k]*dt + a[i][k]*dt*dt/2 + jk[i][k]*dt*dt*dt/6;
                v[i][k] += a[i][k]*dt + jk[i][k]*dt*dt/2;
            }
        }
        for (int i = 0; i < n; i++)
            for (int k = 0; k < 3; k++)
                a[i][k] = jk[i][k] = 0.0;
        for (int i = 0; i < n; i++){
            for (int j = i+1; j < n; j++){
                double rji[3], vji[3];
                for (int k = 0; k < 3; k++){
                    rji[k] = r[j][k] - r[i][k];
                    vji[k] = v[j][k] - v[i][k];
                }
                double r2 = 0;
                for (int k = 0; k < 3; k++)
                    r2 += rji[k] * rji[k];
                double r3 = r2 * sqrt(r2);
                double rv = 0;
                for (int k = 0; k < 3; k++)
                    rv += rji[k] * vji[k];
                rv /= r2;
                for (int k = 0; k < 3; k++){
                    a[i][k] += m[j] * rji[k] / r3;
                    a[j][k] -= m[i] * rji[k] / r3;
                    jk[i][k] += m[j] * (vji[k] - 3 * rv * rji[k]) / r3;
                    jk[j][k] -= m[i] * (vji[k] - 3 * rv * rji[k]) / r3;
                }
            }
        }
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++){
                v[i][k] = old_v[i][k] + (old_a[i][k] + a[i][k])*dt/2
                                      + (old_j[i][k] - jk[i][k])*dt*dt/12;
                r[i][k] = old_r[i][k] + (old_v[i][k] + v[i][k])*dt/2
                                      + (old_a[i][k] - a[i][k])*dt*dt/12;
            }
        }
        if (t >= t_out){
            for (int i = 0; i < n; i++){
                for (int k = 0; k < 3; k++)
                    cout << r[i][k] << " ";
                for (int k = 0; k < 3; k++)
                    cout << v[i][k] << " ";
                cout << endl;
            }
            t_out += dt_out;
        }
    }

    epot = ekin = 0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rji[3];
            for (int k = 0; k < 3; k++)
                rji[k] = r[j][k] - r[i][k];
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rji[k] * rji[k];
            epot -= m[i]*m[j]/sqrt(r2);
        }
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m[i] * v[i][k] * v[i][k];
    }
    double e_out = ekin + epot;

    cerr << "Final total energy E_out = " << e_out << endl;
    cerr << "absolute energy error: E_out - E_in = " << e_out - e_in << endl;
    cerr << "relative energy error: (E_out - E_in) / E_in = "
         << (e_out - e_in) / e_in << endl;
}
//-----------------------------------------------------------------------------
