// hermite1.C

#include  <iostream>
#include  <cmath>
using namespace std;

int main()
{
    int n = 3;
    double r[n][3], v[n][3], a[n][3], jk[n][3];
    const double m = 1;
    double dt, t_end;

    cerr << "Please provide a value for the time step" << endl;
    cin >> dt;
    cerr << "and for the duration of the run" << endl;
    cin >> t_end;

    double s3 = sqrt(3.0);
    double ss3 = sqrt(s3);

    r[0][0] = 1;
    r[0][1] = 0;
    v[0][0] = 0;
    v[0][1] = 1 / ss3;

    r[1][0] = -0.5;
    r[1][1] = 0.5 * s3;
    v[1][0] = -0.5 * ss3;
    v[1][1] = -0.5 / ss3;

    r[2][0] = r[1][0];
    r[2][1] = -r[1][1];
    v[2][0] = -v[1][0];
    v[2][1] = v[1][1];

    for (int i = 0; i < n; i++)
      r[i][2] = v[i][2] = 0;

    v[0][0] += 0.0001;

    for (int i = 0; i < n; i++)
        for (int k = 0; k < 3; k++)
            jk[i][k] = 0.0;
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
                a[i][k] += m * rji[k] / r3;
                a[j][k] -= m * rji[k] / r3;
                jk[i][k] += m * (vji[k] - 3 * rv * rji[k]) / r3;
                jk[j][k] -= m * (vji[k] - 3 * rv * rji[k]) / r3;
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
            epot -= m*m/r;
        }
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m * v[i][k] * v[i][k];
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
                    a[i][k] += m * rji[k] / r3;
                    a[j][k] -= m * rji[k] / r3;
                    jk[i][k] += m * (vji[k] - 3 * rv * rji[k]) / r3;
                    jk[j][k] -= m * (vji[k] - 3 * rv * rji[k]) / r3;
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
            epot -= m*m/sqrt(r2);
        }
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m * v[i][k] * v[i][k];
    }
    double e_out = ekin + epot;

    cerr << "Final total energy E_out = " << e_out << endl;
    cerr << "absolute energy error: E_out - E_in = " << e_out - e_in << endl;
    cerr << "relative energy error: (E_out - E_in) / E_in = "
         << (e_out - e_in) / e_in << endl;
}
