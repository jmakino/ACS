#include  <stdlib.h>
#include  <math.h>
#include  <stdiostream.h>

main()
{
    double r[64][3], v[64][3], a[64][3];
    double m = 1.0/64.0;
    double eps = 0.1;
    double dt = 0.01;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++){
                r[16*i + 4*j + k][0] = i;
                r[16*i + 4*j + k][1] = j;
                r[16*i + 4*j + k][2] = k;
            }

    for (int i = 0; i < 64; i++)
        for (int k = 0; k < 3; k++)
            v[i][k] = 0;

    for (int i = 0; i < 64; i++){
        for (int k = 0; k < 3; k++)
            a[i][k] = 0.0;
        for (int j = 0; j < 64; j++){
            double rij[3];
    	    for (int k = 0; k < 3; k++)
    	        rij[k] = r[j][k] - r[i][k];
    	    double r2 = eps * eps;
            for (int k = 0; k < 3; k++)
    	        r2 += rij[k] * rij[k];
    	    double r3 = r2 * sqrt(r2);
    	    for (int k = 0; k < 3; k++)
    	        a[i][k] += m * rij[k] / r3;
        }
    }

    double etot = 0;
    for (int i = 0; i < 64; i++){
        for (int j = i+1; j < 64; j++){
            double rij[3];
    	    for (int k = 0; k < 3; k++)
    	        rij[k] = r[j][k] - r[i][k];
    	    double r2 = eps * eps;
            for (int k = 0; k < 3; k++)
    	        r2 += rij[k] * rij[k];
    	    double r = sqrt(r2);
    	    etot -= m*m/r;
        }
	for (int k = 0; k < 3; k++)
	    etot += 0.5 * m * v[i][k] * v[i][k];
    }
//    cout.precision(20);
    cout << "Etot [t = " << 0 << "] = " << etot << endl;
    double e_init = etot;

    for (int ns = 0; ns < 1000; ns++){
        for (int i = 0; i < 64; i++){
            cout << "r = " << r[i][0] << " , " <<r[i][1]<<" , "<<r[i][2]<<endl;
//          cout << "v = " << v[i][0] << " , " <<v[i][1]<<" , "<<v[i][2]<<endl;
//          cout << "a = " << a[i][0] << " , " <<a[i][1]<<" , "<<a[i][2]<<endl;
        }
        for (int i = 0; i < 64; i++){
	    for (int k = 0; k < 3; k++)
	        v[i][k] += a[i][k] * dt/2;
	    for (int k = 0; k < 3; k++)
	        r[i][k] += v[i][k] * dt;
	}
        for (int i = 0; i < 64; i++){
	    for (int k = 0; k < 3; k++)
	        a[i][k] = 0.0;
            for (int j = 0; j < 64; j++){
	        double rij[3];
		for (int k = 0; k < 3; k++)
		    rij[k] = r[j][k] - r[i][k];
		double r2 = eps * eps;
		for (int k = 0; k < 3; k++)
		    r2 += rij[k] * rij[k];
		double r3 = r2 * sqrt(r2);
		for (int k = 0; k < 3; k++)
		    a[i][k] += m * rij[k] / r3;
	    }
	}
        for (int i = 0; i < 64; i++){
	    for (int k = 0; k < 3; k++)
	        v[i][k] += a[i][k] * dt/2;
	}
    etot = 0;
    for (int i = 0; i < 64; i++){
        for (int j = i+1; j < 64; j++){
            double rij[3];
    	    for (int k = 0; k < 3; k++)
    	        rij[k] = r[j][k] - r[i][k];
    	    double r2 = eps * eps;
            for (int k = 0; k < 3; k++)
    	        r2 += rij[k] * rij[k];
    	    double r = sqrt(r2);
    	    etot -= m*m/r;
        }
	for (int k = 0; k < 3; k++)
	    etot += 0.5 * m * v[i][k] * v[i][k];
    }
    cout << "Etot [t = " << (ns+1)*dt << "] = " << etot << endl;
    }
    cout << "delta_E = " << etot - e_init << endl;
}
