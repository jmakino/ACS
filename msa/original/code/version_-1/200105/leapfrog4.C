// leapfrog4.C

#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>
#include  <iomanip.h>

double kinetic_energy(double v[][3],
                      double m[],
                      int n)
{
    double ekin = 0;
    for (int i = 0; i < n; i++){
        for (int k = 0; k < 3; k++)
            ekin += 0.5 * m[i] * v[i][k] * v[i][k];
    }
    return ekin;
}

void acceleration(double a[][3],
                  double r[][3],
                  double m[],
                  int n,
                  double &epot)
{
    epot = 0;
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
            double r = sqrt(r2);
            double r3 = r2 * r;
            epot -= m[i]*m[j]/r;
            for (int k = 0; k < 3; k++){
                a[i][k] += m[j] * rij[k] / r3;
                a[j][k] -= m[i] * rij[k] / r3;
            }
        }
    }
}

main()
{
    int n = 3;
    double r[3][3], v[3][3], a[3][3];
    double m[3];
    int refine;

    cin >> refine;
    double dt = 0.01/refine;

    cin >> n;

    for (int i = 0; i < n; i++){
        cin >> m[i];
        for (int k = 0; k < 3; k++)
            cin >> r[i][k];
        for (int k = 0; k < 3; k++)
            cin >> v[i][k];
    }

    double epot;
    acceleration(a, r, m, n, epot);

    double e_init = epot + kinetic_energy(v,m,n);
    cerr << "Etot [t = " << 0 << "] = " << e_init << endl;

    for (int ns = 0; ns < 1000*refine; ns++){
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++)
                v[i][k] += a[i][k] * dt/2;
            for (int k = 0; k < 3; k++)
                r[i][k] += v[i][k] * dt;
        }
        acceleration(a, r, m, n, epot);
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++)
                v[i][k] += a[i][k] * dt/2;
        }
        if ( (ns%refine) == 0){
            double etot = epot + kinetic_energy(v,m,n);
            cerr << "de = " << etot - e_init << endl;
            for (int i = 0; i < n; i++){
                for (int k = 0; k < 3; k++){
                    cout << setw(12) << r[i][k] << " ";
                }
                for (int k = 0; k < 3; k++){
                    cout << setw(12) << v[i][k] << " ";
                }
                cout << endl;
            }
        }
    }
}
