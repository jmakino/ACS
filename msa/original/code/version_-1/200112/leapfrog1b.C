// leapfrog1b.C

#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>

main()
{
    int n = 3;
    double r[3][3], v[3][3], a[3][3];
    double m = 1;
    int refine;
    const double PI = 3.14159265358979323846;

    cin >> refine;

    double dt = 0.01/refine;

    for (int i = 0; i < n; i++){
        double phi = i * 2 * PI / 3;
        r[i][0] = cos (phi);
        r[i][1] = sin (phi);
        r[i][2] = 0;
    }

    for (int i = 0; i < n; i++)
        for (int k = 0; k < 3; k++)
            a[i][k] = 0.0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rij[3];
            for (int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rij[k] * rij[k];
            double r3 = r2 * sqrt(r2);
            for (int k = 0; k < 3; k++){
                a[i][k] += m * rij[k] / r3;
                a[j][k] -= m * rij[k] / r3;
            }
        }
    }

    double v_abs = sqrt(-a[0][0]);

    for (int i = 0; i < n; i++){
        double phi = i * 2 * PI / 3;
        v[i][0] = - v_abs * sin (phi);
        v[i][1] = v_abs * cos (phi);
        v[i][2] = 0;
    }

    v[0][0] += 0.001;

    double ekin, epot, etot;
    epot = ekin = 0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rij[3];
            for (int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rij[k] * rij[k];
            double r = sqrt(r2);
            epot -= m*m/r;
        }
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m * v[i][k] * v[i][k];
    }
    etot = epot + ekin;
    cerr << "Etot [t = " << 0 << "] = " << etot << endl;
    double e_init = etot;

    for (int i = 0; i < n; i++)
        for (int k = 0; k < 3; k++)
            a[i][k] = 0.0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rij[3];
            for (int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];
            double r2 = 0;
            for (int k = 0; k < 3; k++)
                r2 += rij[k] * rij[k];
            double r3 = r2 * sqrt(r2);
            for (int k = 0; k < 3; k++){
                a[i][k] += m * rij[k] / r3;
                a[j][k] -= m * rij[k] / r3;
            }
        }
    }

    for (int ns = 0; ns < 1000*refine; ns++){
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++)
                v[i][k] += a[i][k] * dt/2;
            for (int k = 0; k < 3; k++)
                r[i][k] += v[i][k] * dt;
        }
        for (int i = 0; i < n; i++)
            for (int k = 0; k < 3; k++)
                a[i][k] = 0.0;
        for (int i = 0; i < n; i++){
            for (int j = i+1; j < n; j++){
                double rij[3];
                for (int k = 0; k < 3; k++)
                    rij[k] = r[j][k] - r[i][k];
                double r2 = 0;
                for (int k = 0; k < 3; k++)
                    r2 += rij[k] * rij[k];
                double r3 = r2 * sqrt(r2);
                for (int k = 0; k < 3; k++){
                    a[i][k] += m * rij[k] / r3;
                    a[j][k] -= m * rij[k] / r3;
                }
            }
        }
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++)
                v[i][k] += a[i][k] * dt/2;
        }
        if ( (ns%refine) == 0){
            epot = ekin = 0;
            for (int i = 0; i < n; i++){
                for (int j = i+1; j < n; j++){
                    double rij[3];
                    for (int k = 0; k < 3; k++)
                        rij[k] = r[j][k] - r[i][k];
                    double r2 = 0;
                    for (int k = 0; k < 3; k++)
                        r2 += rij[k] * rij[k];
                    double r = sqrt(r2);
                    epot -= m*m/r;
                }
                for (int k = 0; k < 3; k++)
                    ekin += 0.5 * m * v[i][k] * v[i][k];
            }
            etot = epot + ekin;
            cerr << "de = " << etot - e_init << endl;
            for (int i = 0; i < n; i++){
                for (int k = 0; k < 3; k++)
                    cout << r[i][k] << " ";
                for (int k = 0; k < 3; k++)
                    cout << v[i][k] << " ";
                cout << endl;
            }
        }
    }
}
