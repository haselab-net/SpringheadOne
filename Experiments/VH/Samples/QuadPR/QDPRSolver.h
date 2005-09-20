// QDPRSolver.h: QDPRSolver �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef QDPR_SOLVER_H
#define QDPR_SOLVER_H

#include <Base/QuadProgram.h>
#include <Base/Affine.h>
#include <Base/Base.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Physics/PhJoint.h>
#include <fstream>
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIZE_J 1000	// �֐߂̑�����
#define SIZE_P 6	// �p�����[�^�̎����i��3�����A�g���N3�����j
#define STEP 0.01	// �̂̍\���ƖڕW�̕ω�
#define ADD 0.1		// Q�̑Ίp�����ɉ��������

namespace Spr{;
class QDPRSolver  
{
public:
	QDPRSolver();
	virtual ~QDPRSolver();

	Spr::WBPreciseTimer timer;			// ���Ԍv��
	VQuadProgram<float> quadPR;			// �񎟌v��@

//////////		�֐�	 ///////////////
	void Init();
	void Set(int seed);
	void Calc();
	void Out();
	void DataOut();
	void ParamOut();


//////////		�ϐ�	 ///////////////
	/*
	int count;					// ���s��
	//bool bFile;				// �t�@�C���ŕϐ��ݒ肷�邩�ۂ� 
	DWORD time;					// ���̎��s�ɂ�����������
	//bool bSpringhead;			// Springhead�̊֐����g�����ۂ�

	std::vector<SpVec3d> f_t;	// �e�֐߂ɂ�����͂ƃg���N�i6�����j
	SpVec3d f_t_g;				// �S�g�ɂ�����ڕW�̗͂ƃg���N

	float force_torque[SIZE_P][SIZE_J];
	float force_torque_g[SIZE_P];
	*/
	
	int count;								// ���s��
	float coefficient;
	PTM::VMatrixRow<double> tempA;
	PTM::VVector<double> tempB;
	PTM::VVector<double> x_goal;
	PTM::VVector<double> x_cog;
	PTM::VVector<float> optTorque;

	int useJointNum,ankleDOF,ankleNum,balanceDOF;


};
}
#endif // !defined(QDPR_SOLVER_H)

