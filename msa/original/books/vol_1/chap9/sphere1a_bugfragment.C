
    for (int i = 0; i < n; i++){
	real r = pow(randinter(0.0, 1.0), 1.0/3.0);
	real theta = randinter(0.0, PI);
	real phi = randinter(0.0, 2*PI);
	pos[i][0] = r*sin(theta)*cos(phi);
	pos[i][1] = r*sin(theta)*sin(phi);
	pos[i][2] = r*cos(theta);
	for (int k = 0; k < NDIM; k++)
	    vel[i][k] = 0;
    }
