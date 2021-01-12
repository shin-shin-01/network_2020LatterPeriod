#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 20
#define REPEAT 30

int main(int argc, char *argv[])
{
    /* Add necessary variables */
    int myid, procs;
    int blocks_per_procs;
    int sizea, loopend;

    double *a, *newa;
    int i, j;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);

    /* Get myid and procs */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    /* Divide N into procs */
    blocks_per_procs = N / procs;
    if (myid == 0 || myid == procs-1) {
        sizea = blocks_per_procs+1;
        loopend = blocks_per_procs-1;
    } else {
        sizea = blocks_per_procs+2;
        loopend = blocks_per_procs;
    }

    /* Modify sizes */
    a = (double *)malloc(sizea*sizeof(double));
    newa = (double *)malloc(loopend*sizeof(double));

    /* Modify range of array to initialize */
    for (i = 1; i < sizea-1; i++)
        a[i] = 0.0;

    if (myid == 0)
    	a[0] = 100.0;
    else
        a[0] = 0.0;

    if (myid == procs-1)
	a[sizea-1] = 10.0;
    else
        a[sizea-1] = 0.0;

    for (j = 0; j < REPEAT; j++){
        /* Modify range of loops */
        for (i = 1; i <= loopend; i++)
            newa[i] = (a[i-1] + a[i] + a[i+1])/3.0;
        for (i = 1; i <= loopend; i++)
            a[i] = newa[i];

        /* Let only rank 0 to print out its own data */
        if (myid == 0) {
	    printf("Step %2d: ", j);
       	    for (i = 0; i < N; i++)
                printf(" %6.2f", a[i]);
            printf("\n");
	}
    }

    free(a);
    free(newa);

    /* Finalize MPI */
    MPI_Finalize();

    return 0;
}

