// forward_euler1.C

#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>

main()
{
    double r[3], v[3], a[3];
    double dt = 0.01;

    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
    v[0] = 0;
    v[1] = 0.5;
    v[2] = 0;

    for (int ns = 0; ns < 1000; ns++){
        double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
        for (int k = 0; k < 3; k++)
            a[k] = - r[k] / (r2 * sqrt(r2));
        for (int k = 0; k < 3; k++){
            r[k] += v[k] * dt;
            v[k] += a[k] * dt;
        }
        cout << r[0] << " " << r[1] << " " << r[2] << " ";
        cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
}
