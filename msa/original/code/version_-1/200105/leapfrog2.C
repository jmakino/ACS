// leapfrog2.C

#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>

main()
{
    int n = 3;
    double r[3][3], v[3][3], a[3][3];
    double m = 1;
    int refine;

    cin >> refine;

    double dt = 0.01/refine;

    r[0][0] = 0.9700436;
    r[0][1] = -0.24308753;
    r[0][2] = 0;
    v[0][0] = 0.466203685;
    v[0][1] = 0.43236573;
    v[0][2] = 0;

    r[1][0] = -r[0][0];
    r[1][1] = -r[0][1];
    r[1][2] = -r[0][2];
    v[1][0] = v[0][0];
    v[1][1] = v[0][1];
    v[1][2] = v[0][2];

    r[2][0] = 0;
    r[2][1] = 0;
    r[2][2] = 0;
    v[2][0] = -2 * v[0][0];
    v[2][1] = -2 * v[0][1];
    v[2][2] = -2 * v[0][2];

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
