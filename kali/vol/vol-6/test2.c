#define  N  10000000

double new_a(double old_a)
{
    return sqrt( (old_a * (old_a + 1))/(old_a + 0.001*old_a) );
}

int main()
{
    int i;
    double a;

    a = 1.0;

    for (i = 0; i < N; i++)
        a = new_a(a);
    
    printf("N = %d ; a = %g\n", N, a);
}
