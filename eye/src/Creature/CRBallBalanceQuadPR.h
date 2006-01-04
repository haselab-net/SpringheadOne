// CRBallBalanceQuadPR.h: CRBallBalanceQuadPR �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALLBALANCE_QUADPR_H
#define CRBALLBALANCE_QUADPR_H

#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#include "CRBallBalance.h"

#define ANKLE_NUM	2
#define ANKLE_DOF	6	// ����(��)�̎��R�x(���i3,��]3)
#define BALANCE_DOF	6	// �o�����X�ɕK�v�Ȏ��R�x(���i3,��]3)

namespace Spr{;

class CRVirtualHuman;

class CRBallBalanceQuadPR:public CRBallBalance{

public:
	CRBallBalanceQuadPR();
	virtual ~CRBallBalanceQuadPR();

	// �N���X�̃C���X�^���X
	VQuadProgram<double> quadPR;								// �񎟌v��@
	WBPreciseTimer timer;										// ���Ԍv��

	/////// TEST�֐�&�ϐ��@////////
	float transTime;	// �`�d�s��v�Z����
	float quadTime;		// �񎟌v��@�v�Z����


	//////////�@ �֐��@�@//////////
	// �������֐�
	bool CompNoJoint(int jointNum,CRBallHuman* crHuman);			// �œK���Ŏg��Ȃ��֐߂��ǂ����𔻒f
	void DataOut(CRBallHuman* CRBallHuman);								// �t�@�C���Ƀf�[�^���o��

	// �����ݒ�
	void Init(SGScene* scene,CRBallHuman* crHuman);					// ������
	void SetBalanceParameter(CRBallHuman* crHuman);					// �o�����X�ɕK�v�ȃp�����[�^�ݒ�

	// �񎟌v��@�ł̃o�����X
	virtual void ModifyBody(SGScene* scene,CRBallHuman* crHuman);	// �o�����X���Ƃ��A�̏���(�o�����X�ɕK�v�ȏd�S�g���N�v�Z�͕ʁj
	void SetTargetJointTorque(SGScene* scene,CRBallHuman* crHuman);	// �e�֐߂̖ڕW�g���N�̎擾
	void CalcTransMatrix(SGScene* scene,CRBallHuman* crHuman);		// �`�d�s��̌v�Z
	void MakeTorquePattern(CRBallHuman* crHuman, int t);			// �`�d�s��̂��߂̃g���N,�̓p�^�[���̐���
	Vec3d GetJointAccel(PHJoint1D* joint);						// �֐߂̕��i�����x�̎擾
	virtual void SetQuadPR(CRBallHuman* crHuman);							// �񎟌v��@�̖ړI�֐����v�Z
	void QuadPRSolve();											// �񎟌v��@������
	void SetBalanceTorque(CRBallHuman* crHuman);					// �œK(�o�����X)�g���N���֐߂ɃZ�b�g����
	bool BalanceCheck(CRBallHuman* crHuman);
	
	// ����Ńo�����X��ۏ�,�����������Ȃ��悤�ɂ���
	virtual void AnkleBalance(CRBallHuman* crHuman);						// �񎟌v��@�Ŏ�肫��Ȃ��o�����X�𑫎�ŕۏ�
	Vec3d AdjustAnkleTorque(CRBallHuman* crHuman,
				Vec3d ankleForce,Vec3d ankleTorque,int ankle);	// �����������Ȃ��悤�Ƀg���N�𒲐�
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);	// ZMP���ڐG���p�`���ɂ��邩�`�F�b�N
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleRealZmpR(){return ankleRealZmp[0];}
	Vec3f GetAnkleRealZmpL(){return ankleRealZmp[1];}
	Vec3f GetAnkleGoalZmpR(){return ankleGoalZmp[0];}
	Vec3f GetAnkleGoalZmpL(){return ankleGoalZmp[1];}

	void RenewConstraint(CRBallHuman* crHuman);
	void NewConstraint(CRBallHuman* CRBallHuman,PHSolid* heel,Vec3f* maxRange,Vec3f* minRange,int ankleSide);


	// TEST
	void AnkleBalance2(CRBallHuman* CRBallHuman);
	Vec3d AdjustAnkleTorque2(CRBallHuman* crHuman,Vec3d footForce,
							Vec3d footTorque,Vec3d zmp,int ankle);	// �����������Ȃ��悤�Ƀg���N�𒲐�
	Vec3d cog_zmp,footR_zmp,footL_zmp;
	bool bAdjust;

	void OffSpring();

	//////////�@ �ϐ��@�@//////////
	unsigned int ankleNum;							// ���̐�
	unsigned int ankelDof;							// ����(��)�̎��R�x(���i3,��]3)
	unsigned int balanceDof;						// �o�����X�ɕK�v�Ȏ��R�x(���i3,��]3)
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
	PTM::VVector<float> coefficientJoint;			// �e�֐߂̃g���N�}���W��(�傫���Ɨ}��)

	PTM::TVector<BALANCE_DOF,double> targetX;		// �o�����X�̖ڕW
	PTM::VVector<float> optTorque;					// �œK�g���N
	float qdprBalanceRate;							// �o�����X�ɕK�v�ȃg���N�̉�����񎟌v��@�ŉ�����
	Vec3f ankleRealZmp[2];							// �����̃��A��ZMP
	Vec3f ankleGoalZmp[2];							// �����̏����ڕWZMP
	unsigned int useJoNum;							// �œK���Ŏg���֐߂̐�
	std::vector<Vec3d> unitVec;						// X,Y,Z�̒P�ʃx�N�g��
	int count;										// �񎟌v��@�̎��s��
	
};

}		//	end namespace Spr
#endif // !defined(CRBallBalance_QUADPR_H)
