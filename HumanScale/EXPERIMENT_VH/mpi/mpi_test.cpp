/*
 プロセッサ数とプロセッサ番号を表示させる
*/
#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
  int nsize;  /* プロセッサ総数を格納する */
  int myself; /* 自分のプロセッサ番号(rank)を格納する */

  MPI_Status status;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myself);
  printf("This is %d in %d\n",myself,nsize);

  MPI_Finalize();
  return 0;
}

