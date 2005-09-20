/*
 0以外のプロセッサから0へ送信し、受信にかかる時間を総計する。
 さらに、通信スループットを計測する。
*/
#include <stdio.h>
#include "mpi/mpi.h"

#define SIZE 100000
int main(int argc, char **argv)
{
  int nsize;  /* プロセッサ総数を格納する */
  int myself; /* 自分のプロセッサ番号(rank)を格納する */
  MPI_Status status;
  int val[SIZE],i;
  double btime,etime;  /* 時間計測に用いる */

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myself);

  for(i=0;i＜SIZE;i++)val[i]=myself*100;
  if(nsize>1 && myself==0){
    btime=MPI_Wtime();
    for(i=1;i＜nsize;i++){ /* プロセッサ0の動作 */
      MPI_Recv(&val,SIZE,MPI_INT,i ,1234,MPI_COMM_WORLD, &status);
    }  
    etime=MPI_Wtime();
    printf("Elapsed time is %10.3f sec\n",(etime-btime));
    printf("Throughput is %10.3f MB/sec\n",1.0e-6*SIZE*(nsize-1)*sizeof(int)/(etime-btime));
  }
  else if(nsize>1 && myself!=0){/* プロセッサ0以外の動作 */
    MPI_Send(&val,SIZE,MPI_INT,0 ,1234,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}