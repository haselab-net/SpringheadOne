// CRBalanceQuadPR.h: CRBalanceQuadPR �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALANCE_QUADPR_H
#define CRBALANCE_QUADPR_H

#include <SceneGraph/SGScene.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PHForceField.h>
#include <Base/Affine.h>
#include <Base/TVector.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#include <vector>

namespace Spr{;
class CRHuman;

#define DOF_NUM		6	// �œK�����Ŏg�����R�x�̐�
#define JOINT_NUM	39	// �W���C���g�i�ϐ��j�̐�
#define R_ANKLE_J	29	// �E����̊֐�(�Ō��)�̔ԍ�
#define L_ANKLE_J	37	// ������̊֐�(�Ō��)�̔ԍ�
#define R_HEEL_S	12	// �E�����̔ԍ�
#define L_HEEL_S	16	// �������̔ԍ�

class CRBalanceQuadPR  
{
public:
	CRBalanceQuadPR();
	virtual ~CRBalanceQuadPR();

//---- �o�����X����邽�߁A�d�S�ɕK�v�ȃg���N���e�֐߂ɍœK���z���� ----//

	PHJointEngine* jointEngine;									// PHJointEngine
	PHGravityEngine* gravity;									// PHGravityEngine
	VQuadProgram<double> quadPR;								// �񎟌v��@
	WBPreciseTimer timer;										// ���Ԍv��

	void Step(CRHuman* crHuman);

	//�e�֐߂̃g���N�̏d�S�ɑ΂���e����\���s����쐬
	void TransTorque(SGScene* scene,CRHuman* crHuman);
	void MakeTorquePattern(CRHuman* crHuman, int t);			// �g���N�A�̓p�^�[���̍쐬
	Vec3d GetJointAccel(PHJoint1D* joint);						// �q��Solid�̉����x,�p�����x��Joint�̉����x�֕ϊ�
	void QuadPRSolve();											// �񎟌v��@������
	void SetQuadPR(CRHuman* crHuman);							// Q,C�̐ݒ�
	void Init(SGScene* scene,CRHuman* crHuman);					// �񎟌v��@�ɕK�v�ȏ��ݒ�
	void DataOut(CRHuman* crHuman);
	void SetJointTorque(CRHuman* crHuman);						// �œK���̃g���N���֐߂ɃZ�b�g����B

	//TEST
	void SetAnkleForce(CRHuman* crHuman);


	PTM::TMatrixRow<DOF_NUM,JOINT_NUM,double> transmissionM;	// �e�֐߃g���N�̓`�d�s��M
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> transmissionD;		// �e�֐߃g���N�̓`�d�s��D
	PTM::TVector<DOF_NUM,double> transmissionC;					// �e�֐߃g���N�̓`�d�s��C
	PTM::TVector<JOINT_NUM,double> targetTorqe;					// �e�֐߂̖ړI�g���N(t0)
	double coefficient;											// �񎟌v��@�̖ړI�֐��̖ڕW�̃g���N��
																// �œK�ȃg���N�̍����ŏ��ɂ��鍀�̌W��
	
	std::vector<Vec3d> unitVec;									// X,Y,Z�̒P�ʃx�N�g��
	PTM::TMatrixRow<3,6,double> upMat;								// (6�~1)�̏�O��؂�o���s��
	PTM::TMatrixRow<3,6,double> lowMat;							// (6�~1)�̉��O��؂�o���s��
	int count;													// �񎟌v��@�̎��s��

};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_QUADPR_H)
