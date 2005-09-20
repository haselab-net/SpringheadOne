/*
 �אڃv���Z�b�T�Ƀf�[�^��ʐM����
   ex. �v���Z�b�T1�̓v���Z�b�T2�ɑ���A�v���Z�b�T0�����M����B
*/

#include <stdio.h>
#include "mpi/mpi.h"
int main(int argc, char **argv)
{
  int nsize;  /* �v���Z�b�T�������i�[���� */
  int myself; /* �����̃v���Z�b�T�ԍ�(rank)���i�[���� */
  MPI_Status status;
  int val,nextval;
  int dst;  /* �ʐM���̑��M��rank */
  int src;  /* �ʐM���̑��M��rank */

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

