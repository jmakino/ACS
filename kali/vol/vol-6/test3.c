#define  NMAX  100000
#define  NDIM  3
#define  EPS  0.000001

int main()
{
    int i, j, k, n;
    double r[NMAX][NDIM];
    double sum;
    double rji[NDIM];
    double r2;

    scanf("%d", &n);

    for (i = 0; i < n; i++)
	for (k = 0; k < NDIM; k++)
	    r[i][k] = 1.0 + (i * NDIM + k) * EPS;

    sum = 0;
    for (i = 0; i < n; i++){
        for (j = i+1; j < n; j++){
            for (k = 0; k < 3; k++)
                rji[k] = r[j][k] - r[i][k];
	    r2 = 0;
            for (k = 0; k < NDIM; k++)
                r2 += rji[k] * rji[k];
	    sum += r2;
	}
    }
    printf("n = %d ; sum = %.10g\n", n, sum);
}

