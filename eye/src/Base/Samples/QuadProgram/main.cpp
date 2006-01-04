#include <stdio.h>
#include <Base/QuadProgram.h>
#include "matrixloader.h"

#define SIZE_ 8

int main(){
	//�T���v���̍s�������Ă���
	CMatrixLoader<float,SIZE_> aObj;
	aObj.loadFile("qp_matrix_01.txt",1); 

	VQuadProgram<float> aQP;
	TVector<SIZE_, float> vmin;
	TVector<SIZE_, float> vmax;
	for(int i=0; i<SIZE_; ++i){
		vmin[i] = 1;
		vmax[i] = 20;
	}
	aQP.Init(vmin, vmax);
	// Q�̐ݒ�
	for(int i=0;i<SIZE_;i++){
		for(int j=0;j<SIZE_;j++){
			aQP.matQ[i][j] = aObj.matQ[0][i][j];
		}
	}

	// C�̐ݒ�
	aQP.vecC[0] = 0;
	aQP.vecC[1] = 1;
	aQP.vecC[2] = 12;
	aQP.vecC[3] = 10;
	aQP.vecC[4] = -8;
	aQP.vecC[5] = 6;
	aQP.vecC[6] = 4;
	aQP.vecC[7] = 2;

	int count;
	count = aQP.Solve();

	// ���ʂ̕`��
	printf("�œK��: %d�@times\n",count);
	for(int i=0;i<SIZE_;i++){
		printf("%10f\n",aQP.vecX[i]);
	}
	return 0;
}
