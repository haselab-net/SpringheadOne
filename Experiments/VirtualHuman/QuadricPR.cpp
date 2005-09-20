// QuadricPR.cpp: QuadricPR �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuadricPR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

QuadricPR::QuadricPR()
{
	input[4] = 0;			//0:min�̍œK�����A1:max
	input[6] = 1000;		//�s�{�b�g�̍ő��
	input[7]=1;				//0�F�ړI�֐����v�Z���Ȃ��@1:�œK�������������Ƃ��A�ړI�֐��̒l���v�Z����
	input[8]=0;				//0�F���̃p�����[�^���o�͂��Ȃ��@1�F����
	input[9]=0;				//0�F
	input[10]=0;			//0�F�s�{�b�g�̉񐔂��o�͂���@1�F���Ȃ�
	input[11]=0;			
	input[12]=0;
	input[13]=80;
	
	tol[0] = tol[1] = 0;
	strcpy(title, "Quadratic Ploblem");
	strcpy(pfile, "quad_savefile");
	title_len = 0;
	pfile_len = 0;
}

QuadricPR::~QuadricPR()
{

}



void QuadricPR::CalcQuadPR(THuman& human, int numConst, int numVari, bool flagEq){
	//numConst:����������̐�(M)�AnumVari:�ϐ��̐�(N)
	//flagEq:����������ɕs���������邩�Ȃ����B
	
	human.joints.size();
	///// �e�p�����[�^�̓��I�������m�� /////
	//����
	Quad = new float[numConst*numVari];
	cost = new float[numVari];
	A = new float[numConst*numVari];
	rhs = new float[numConst];
	kt = new long int[numConst];

	//�o�͐�
	solution = new float[numVari];
	rc = new float[numVari];
	dual = new float[numVari];
	slk = new float[numConst];
	
	if(flagEq)
		workSpace = new float[2*(numConst+numVari)^2+12*(numConst+numVari)+16];
	else
		workSpace = new float[2*(numConst+numVari)^2+8*(numConst+numVari)+6];

	
	////// �e�p�����[�^�̐ݒ� //////

	for(int i = 0; i < numConst; ++i)
		kt[i] = 0;

	if(numConst > numVari)
		input[0] = numVari;
	else
		input[0] = numConst;

	input[1] = numVari;
	input[2] = numConst;
	input[3] = numVari;
	input[5] = (int)workSpace;

	//Quad & A
	for(int i = 0; i < numConst; ++i){
		for(int j = 0; j < numVari; ++j){
			//cost[j] = -2*
			if(i ==j)
				Quad[i*numVari + j] = 1.0;
			else
				Quad[i*numVari + j] = 0.0;
		}
	}

	//cost
	for(int time = 0; time < numVari/human.joints.size(); ++time){
		for(int jointNum = 0; jointNum < human.joints.size(); ++jointNum){
			cost[time*(numVari/human.joints.size())+jointNum] = -2*jointOpt[time][jointNum];
		}
	}

	//A:������

	//rhs:������


	//�񎟌v��@�������֐� quadpr_
	quadpr_(A, kt, rhs, cost, Quad, input, tol, title, pfile,
		obj, solution, rc, dual,slk,iout, workSpace, title_len, pfile_len);

	//����delete������̂������ƃ`�F�b�N����B�ꏊ��(�f�X�g���N�^�j
	delete [] Quad, A, cost,rhs,kt,solution,rc,dual,slk,workSpace;

}