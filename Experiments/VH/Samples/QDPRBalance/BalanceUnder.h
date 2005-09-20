// BalanceUnder.h: BalanceUnder �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_UNDER_H
#define BALANCE_UNDER_H

#include <Creature/CRBalance.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#define ANKLE_NUM	2
#define ANKLE_DOF	6
#define BALANCE_DOF	6	// �o�����X�ɕK�v�Ȏ��R�x(���i3,��]3)

#define R_ANKLE_J	29	// �E����̊֐�(�Ō��)�̔ԍ�
#define L_ANKLE_J	37	// ������̊֐�(�Ō��)�̔ԍ�
#define R_HEEL_S	12	// �E�����̔ԍ�
#define L_HEEL_S	16	// �������̔ԍ�

class ModelUnder;
namespace Spr{;

class BalanceUnder : public CRBalance  
{
public:
	BalanceUnder();
	virtual ~BalanceUnder();
	VQuadProgram<double> quadPR;								// �񎟌v��@
	WBPreciseTimer timer;
	float transTime;
	float quadTime;

	//	  �֐�		//
	void RegistNoJoint();									// �œK���Ŏg��Ȃ��֐߂����߂�
	bool CompNoJoint(int jointNum);							// �œK���Ŏg��Ȃ��֐߂��ǂ����𔻒f

	void Init(SGScene* scene,CRHuman* crHuman);				// ������
	void SetBalanceParameter(CRHuman* crHuman);				// �o�����X�ɕK�v�ȃp�����[�^�ݒ�

	void ModifyBody(SGScene* scene,CRHuman* crHuman);		// �o�����X���Ƃ��A�̏���(�o�����X�ɕK�v�ȏd�S�g���N�v�Z�͕ʁj
	void SetPreQuadPR(SGScene* scene,CRHuman* crHuman);		// �e�֐߂̖ڕW�g���N�̎擾
	void CalcTransMatrix(SGScene* scene,CRHuman* crHuman);	// �`�d�s��̌v�Z
	void MakeTorquePattern(CRHuman* crHuman, int t);		// �`�d�s��̂��߂̃g���N,�̓p�^�[���̐���
	Vec3d GetJointAccel(PHJoint1D* joint);					// �֐߂̕��i�����x�̎擾
	void DataOut(CRHuman* crHuman);							// �t�@�C���Ƀf�[�^���o��
	void SetQuadPR(CRHuman* crHuman);						// �񎟌v��@�̖ړI�֐����v�Z
	void QuadPRSolve();										// �񎟌v��@������
	void SetJointTorque(CRHuman* crHuman);					// �œK�g���N���֐߂ɃZ�b�g
	void SetAnkle(CRHuman* crHuman);						// �񎟌v��@�Ŏ�肫��Ȃ��o�����X�𑫎�ŕۏ�


	// �����������Ȃ��悤�Ƀg���N��}����
	void SaveAnkleTorque(CRHuman* crHuman,Vec3d& ankleForce,Vec3d& ankleTorque,int ankle);
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);						// ZMP���ڐG���p�`���ɂ��邩�`�F�b�N
	Vec3f ClosestPoint(Vec3f zmp,CRSupportArea* supportArea);
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleZmpR(){return zmp[0];}
	Vec3f GetAnkleZmpL(){return zmp[1];}

	// TEST
	void CalcTransMatrix2(SGScene* scene,CRHuman* crHuman);	// �`�d�s��̌v�Z
	void MakeTorquePattern2(CRHuman* crHuman, int t);		// �`�d�s��̂��߂̃g���N,�̓p�^�[���̐���
	void SetQuadPR2(CRHuman* crHuman);						// �񎟌v��@�̖ړI�֐����v�Z

//private:
public:
	//	  �ϐ�		//
	// �`�d�s��
	PTM::VMatrixRow<float>	transT;					// �e�֐߃g���N�̓`�d�s��T(�g���N�̓`�d�j
	PTM::VMatrixRow<float>	transF;					// �e�֐߃g���N�̓`�d�s��F(�͂̓`�d)
	PTM::VVector<float>		transM;					// �e�֐߃g���N�̓`�d�s��M(���ʂ̓`�d)
	// �񎟌v��@��X_cog = tempA*t + tempB
	PTM::VMatrixRow<double> tempA;
	PTM::VVector<float> tempB;
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

	std::vector<Vec3d> unitVec;						// X,Y,Z�̒P�ʃx�N�g��
	std::vector<int> noJoint;						// �œK���Ŏg��Ȃ��֐ߔԍ�
	unsigned int useJoNum;							// �œK���Ŏg���֐߂̐�
	int count;										// �񎟌v��@�̎��s��

	PTM::TVector<BALANCE_DOF,double> targetX;

	// �Б����̂y�l�o
	Vec3f zmp[2];


};

}		// end namespace

#endif // !defined(BALANCE_UNDER_H)
