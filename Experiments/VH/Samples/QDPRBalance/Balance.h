// Balance.h: Balance �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_H
#define BALANCE_H

#include <Creature/CRBalance.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#define ANKLE_DOF_ 3
#define ANKLE_NUMBER 1
#define DOF_NUM 6

namespace Spr{;

class Model;

class Balance : public CRBalance  
{
public:
	Balance();
	virtual ~Balance();
	VQuadProgram<double> quadPR;							// �񎟌v��@
	WBPreciseTimer timer;

	//	  �֐�		//
	virtual void Init(SGScene* scene,CRHuman* crHuman);			// ������
	virtual void SetBalanceParameter(CRHuman* crHuman);			// �o�����X���Ƃ邽�߂̖ڕW�̃p�����[�^��ݒ�

	void Measure(SGScene* scene,CRHuman* crHuman);				// �o�����X�ɕK�v�Ȍv��
	virtual void ModifyBody(SGScene* scene,CRHuman* crHuman);	// �񎟌v��@�Ńo�����X���Ƃ�
	
	void SetPreQuadPR(SGScene* scene,CRHuman* crHuman);						// CalcAccel�Ȃǂ�����O�ɕK�v�ȃp�����[�^���擾(�Z�b�g����
	void CalcTransMatrix(SGScene* scene,CRHuman* crHuman);		// �`�d(�e�֐߂̃g���N�̏d�S�ɑ΂���e����\��)�s����쐬
	void MakeTorquePattern(CRHuman* crHuman, int t);			// �`�d�����߂邽�߂̃g���N�A�̓p�^�[���̍쐬
	Vec3d GetJointAccel(PHJoint1D* joint);						// �q��Solid�̉����x,�p�����x��Joint�̉����x�֕ϊ�
	void SetQuadPR(CRHuman* crHuman);							// Q,C�̐ݒ�
	void QuadPRSolve();											// �񎟌v��@������
	void SetJointTorque(CRHuman* crHuman);						// �œK���̃g���N���֐߂ɃZ�b�g����B
	void SetAnkle(CRHuman* crHuman);							// �œK���ƃo�����X�ɕK�v�ȃg���N�̍��𑫎�ɂ�����B

	void RegistNoJoint(CRHuman* crHuman);						// �œK���Ŏg��Ȃ��֐߂����߂�
	bool CompNoJoint(int jointNum);								// �œK���Ŏg��Ȃ��֐߂��ǂ����𔻒f
	void DataOut(CRHuman* crHuman);								// xls�ɕK�v�ȃf�[�^���o��

	void AdjustAnkleTorque(CRHuman* crHuman,
			Vec3d& ankleForce,Vec3d& ankleTorque,int ankle);	// �����������Ȃ��悤�Ƀg���N�𒲐�
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);	// ZMP���ڐG���p�`���ɂ��邩�`�F�b�N
	Vec3f ClosestPoint(Vec3f zmp,CRSupportArea* supportArea);	// �ŋߖT�_�����߂�
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleZMP(){return ankleZmp;}
	Vec3f ankleZmp;



	//	  �ϐ�		//
	// �`�d�s��
	PTM::VMatrixRow<float>	transT;					// �e�֐߃g���N�̓`�d�s��T(�g���N�̓`�d�j
	PTM::VMatrixRow<float>	transF;					// �e�֐߃g���N�̓`�d�s��F(�͂̓`�d)
	PTM::VVector<float>		transM;					// �e�֐߃g���N�̓`�d�s��M(���ʂ̓`�d)
	// �e�֐߂̖ړI�g���N(���B�^���̂��߂̃g���N)
	PTM::VVector<float> targetTorque;				// �e�֐߂̖ړI�g���N(t0)
	PTM::VVector<float> targetTorqueP;				// �e�֐߂̖ړI�g���N(t0)��proportional����
	PTM::VVector<float> targetTorqueD;				// �e�֐߂̖ړI�g���N(t0)��differential����
	// �񎟌v��@�̖ړI�֐��̌W��
	float coefficient;								// �o�����X�d�� or ���B�^���d��
	float coefficientP;								// �ڕW�g���NP�����ƍœK�ȃg���N�̍����ŏ��ɂ��鍀�̌W��
	float coefficientD;								// �ڕW�g���ND�����ƍœK�ȃg���N�̍����ŏ��ɂ��鍀�̌W��
	PTM::VVector<float> coefficientJo;				// �e�֐߂̃g���N�}���W��(�傫���Ɨ}��)

	float qdprBalanceRate;							// �o�����X�ɕK�v�ȃg���N�̉�����񎟌v��@�ŉ�����
	Vec3f cogForce;

	std::vector<Vec3d> unitVec;						// X,Y,Z�̒P�ʃx�N�g��
	std::vector<int> noJoint;						// �œK���Ŏg��Ȃ��֐ߔԍ�
	int useJoNum;									// �œK���Ŏg���֐߂̐�
	int count;										// �񎟌v��@�̎��s��
	
	// TEST
	PTM::VVector<float> BalanceTorque;
	PTM::VMatrixRow<double> optA;
	PTM::VVector<float> optB;
};

}		// end namespace
#endif // !defined(BALANCE_H)
