#define  N  100000000

int main()
{
    int i;
    double a;

    a = 1.0;
    for (i = 0; i < N; i++){
        a = sqrt( (a * (a + 1))/(a + 0.001*a) );
    }
    
    printf("a = %g\n", a);
}
