#include <stdio.h>
#include <stdlib.h>
/* Add header file */

#define N 20
#define REPEAT 30

int main(int argc, char *argv[])
{
    /* Add necessary variables */

    double *a, *newa;
    int i, j;

    /* Initialize MPI */

    /* Get myid and procs */

    /* Divide N into procs */

    /* Modify sizes */
    a = (double *)malloc(N*sizeof(double));
    newa = (double *)malloc(N*sizeof(double));

    /* Modify range of array to initialize */
    for (i = 1; i < N-1; i++)
        a[i] = 0.0;
    a[0] = 100.0;
    a[N-1] = 10.0;

    for (j = 0; j < REPEAT; j++){
        /* Modify range of loops */
        for (i = 1; i <= N-2; i++)
            newa[i] = (a[i-1] + a[i] + a[i+1])/3.0;
        for (i = 1; i <= N-2; i++)
            a[i] = newa[i];

        /* Let only rank 0 to print out its own data */
        printf("Step %2d: ", j);
        for (i = 0; i < N; i++)
            printf(" %6.2f", a[i]);
        printf("\n");
    }

    free(a);
    free(newa);

    /* Finalize MPI */

}

