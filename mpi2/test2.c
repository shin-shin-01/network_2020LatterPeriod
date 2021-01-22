#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 20
#define REPEAT 30

int main(int argc, char *argv[])
{
    int myid, procs, divN, sizea, loopend;
    double *a, *newa;
    int i, j;
    /* Add necessary variables */
    double *b;
    MPI_Request r[4];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    divN = N / procs;
    if (myid == 0) {
        sizea = divN + 1;
        loopend = divN - 1;
    } else if (myid == procs - 1){
        sizea = divN + 1;
        loopend = divN - 1;
    } else {
        sizea = divN + 2;
        loopend = divN;
    }

    a = (double *)malloc(sizea*sizeof(double));
    newa = (double *)malloc(sizea*sizeof(double));

    for (i = 1; i <= loopend; i++)
        a[i] = 0.0;
    if (myid == 0) 
        a[0] = 100.0;
    else
        a[0] = 0.0;
    if (myid == procs-1)
        a[sizea-1] = 10.0;
    else
        a[sizea-1] = 0.0;

    /* Allocate an array for MPI_Gather */
    /* 0 に集めるので配列を用意 各プロセスから divN 個数 */
    if (myid==0)
        b = (double *)malloc(divN*procs*sizeof(double)); 
  
    for (j = 0; j < REPEAT; j++){
        /* Exchange values */
        /* 左のプロセスとの通信 a[0]で受信しa[1]を送信 */
        /* MPI_Isend
          1: 送信データの先頭
          2: 要素数
          3: データ型
          4: 送信先プロセス番号
          5: タグ
          6: コミュニケータ
          7: リクエスト情報の格納場所
        */
        if (myid != 0) {
            MPI_Isend(&(a[1]), 1, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, &(r[0]));
            MPI_Irecv(&(a[0]), 1, MPI_DOUBLE, myid-1, 0, MPI_COMM_WORLD, &(r[1]));
        }
        /* 右のプロセスとの通信 a[loopend]を送信しa[loopend+1]に受信 */
        if ( myid != procs - 1 ) {
            MPI_Isend(&(a[loopend]), 1, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &(r[2]));
            MPI_Irecv(&(a[loopend+1]), 1, MPI_DOUBLE, myid+1, 0, MPI_COMM_WORLD, &(r[3]));
        }
        /* Modify loop to calculate internal area only */
        /* Exchange に関係しない箇所の計算を行っておく */
        for (i = 2; i <= loopend-1; i++)
            newa[i] = (a[i-1] + a[i] + a[i+1])/3.0;

        /* Wait for all non-blocking communications */
        if (myid == 0) {
            MPI_Wait(&(r[2]), MPI_STATUS_IGNORE);
            MPI_Wait(&(r[3]), MPI_STATUS_IGNORE);
        } else if (myid == procs - 1){
            MPI_Wait(&(r[0]), MPI_STATUS_IGNORE);
            MPI_Wait(&(r[1]), MPI_STATUS_IGNORE);
        } else {
            MPI_Wait(&(r[0]), MPI_STATUS_IGNORE);
            MPI_Wait(&(r[1]), MPI_STATUS_IGNORE);
            MPI_Wait(&(r[2]), MPI_STATUS_IGNORE);
            MPI_Wait(&(r[3]), MPI_STATUS_IGNORE);
        }

        /* Calculate edges */
        /* Wait で取得した値を用いて計算 */
        newa[1] = (a[0] + a[1] + a[2])/3.0;
        newa[loopend] = (a[loopend-1] + a[loopend] + a[loopend+1])/3.0;

        for (i = 1; i <= loopend; i++)
            a[i] = newa[i];

        /* Gather data into rank 0 and print */
        /* MPI_Gather
         1: 送信するデータの格納場所の先頭
         2: 送信するデータの要素数
         3: 送信するデータの型
         4: 受信する...(1)
         5: 受信する...(2)
         6: 受信する...(3)
         7: データを集めて格納するランク
         8: コミュニケータ

         今回のプログラムではプロセス0が全ての結果をまとめる
         myid != 0 では受信しないので NULLを指定
        */
        if (myid==0) MPI_Gather(&(a[0]), divN, MPI_DOUBLE, b, divN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        else MPI_Gather(&(a[1]), divN, MPI_DOUBLE, NULL, divN, MPI_DOUBLE, 0, MPI_COMM_WORLD);


        if (myid == 0) {
            printf("Step %2d: ", j);
                       
            /* Modify loop range and array to print all elements */
            for (i = 0; i < divN * procs; i++)
                printf(" %6.2f", b[i]);
            printf("\n");
        }
    }

    free(a);
    free(newa);
    /* Free the array for MPI_Gather */
    if (myid == 0) free(b); 

    MPI_Finalize();
}
