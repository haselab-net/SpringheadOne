/*
 隣接プロセッサにデータを通信する
   ex. プロセッサ1はプロセッサ2に送り、プロセッサ0から受信する。
*/

#include <stdio.h>
#include "mpi/mpi.h"
int main(int argc, char **argv)
{
  int nsize;  /* プロセッサ総数を格納する */
  int myself; /* 自分のプロセッサ番号(rank)を格納する */
  MPI_Status status;
  int val,nextval;
  int dst;  /* 通信時の送信先rank */
  int src;  /* 通信時の送信元rank */

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myself);

  val=myself*100;
  if(nsize>1){
    dst=(myself+1)%nsize;
    src=(myself-1+nsize)%nsize;
    MPI_Send(&val,1,MPI_INT,dst ,1234,MPI_COMM_WORLD);
    MPI_Recv(&nextval,1,MPI_INT,src ,1234,MPI_COMM_WORLD, &status);

    printf("Proc %d gets %d from Proc %d\n",myself,nextval,src);
  }

  MPI_Finalize();
  return 0;
}

