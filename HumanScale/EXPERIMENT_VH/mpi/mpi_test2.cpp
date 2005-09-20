/*
 0�ȊO�̃v���Z�b�T����0�֑��M���A��M�ɂ����鎞�Ԃ𑍌v����B
 ����ɁA�ʐM�X���[�v�b�g���v������B
*/
#include <stdio.h>
#include "mpi/mpi.h"

#define SIZE 100000
int main(int argc, char **argv)
{
  int nsize;  /* �v���Z�b�T�������i�[���� */
  int myself; /* �����̃v���Z�b�T�ԍ�(rank)���i�[���� */
  MPI_Status status;
  int val[SIZE],i;
  double btime,etime;  /* ���Ԍv���ɗp���� */

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myself);

  for(i=0;i��SIZE;i++)val[i]=myself*100;
  if(nsize>1 && myself==0){
    btime=MPI_Wtime();
    for(i=1;i��nsize;i++){ /* �v���Z�b�T0�̓��� */
      MPI_Recv(&val,SIZE,MPI_INT,i ,1234,MPI_COMM_WORLD, &status);
    }  
    etime=MPI_Wtime();
    printf("Elapsed time is %10.3f sec\n",(etime-btime));
    printf("Throughput is %10.3f MB/sec\n",1.0e-6*SIZE*(nsize-1)*sizeof(int)/(etime-btime));
  }
  else if(nsize>1 && myself!=0){/* �v���Z�b�T0�ȊO�̓��� */
    MPI_Send(&val,SIZE,MPI_INT,0 ,1234,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}