// leapfrog1.C

#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>

main()
{
    double r[3], v[3], a[3];
    int refine;

    cin >> refine;

    double dt = 0.01/refine;

    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
    v[0] = 0;
    v[1] = 0.5;
    v[2] = 0;

    double ekin, epot, etot;

    ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    epot = -1.0/sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    etot = ekin + epot;

    cerr << "Etot [t = " << 0 << "] = " << etot << endl;
    double e_init = etot;

    double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
    for (int k = 0; k < 3; k++)
        a[k] = - r[k] / (r2 * sqrt(r2));

    for (int ns = 0; ns < 1000*refine; ns++){
        for (int k = 0; k < 3; k++){
            v[k] += 0.5 * a[k] * dt;
            r[k] += v[k] * dt;
        }
        r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
        for (int k = 0; k < 3; k++){
            a[k] = - r[k] / (r2 * sqrt(r2));
            v[k] += 0.5 * a[k] * dt;
        }
        if ( (ns%refine) == 0){
            ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            epot = -1.0/sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
            etot = ekin + epot;
            cerr << "de = " << etot - e_init << endl;
            cout << r[0] << " " << r[1] << " " << r[2] << " ";
            cout << v[0] << " " << v[1] << " " << v[2] << endl;
        }
    }
}
