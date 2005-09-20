/*
 �v���Z�b�T���ƃv���Z�b�T�ԍ���\��������
*/
#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
  int nsize;  /* �v���Z�b�T�������i�[���� */
  int myself; /* �����̃v���Z�b�T�ԍ�(rank)���i�[���� */

  MPI_Status status;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myself);
  printf("This is %d in %d\n",myself,nsize);

  MPI_Finalize();
  return 0;
}

