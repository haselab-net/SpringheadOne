// CRBalanceMultiLeg.h: CRBalanceMultiLeg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(CRBALANCE_MULTI_LEG_H)
#define CRBALANCE_MULTI_LEG_H
#include "CRHuman.h"
#include "CRBalanceQuadPR.h"
namespace Spr{;

class CRBalanceMultiLeg : public CRBalanceQuadPR  
{
public:
	CRBalanceMultiLeg();
	virtual ~CRBalanceMultiLeg();

	enum TBlance{
		BOTH_LEG,
		RIGHT_LEG,
		LEFT_LEG,
		FALL
	} balanceState;
	
	void ModifyBody(SGScene* scene,CRHuman* crHuman);				// �o�����X���Ƃ��A�̏���(�o�����X�ɕK�v�ȏd�S�g���N�v�Z�͕ʁj
	void CalcBalanceForce(SGScene* scene,CRHuman* crHuman);
	void CalcBalanceTorque(SGScene* scene, CRHuman* crHuman);
	void CalcTransMatrixOneLeg(SGScene* scene,CRHuman* crHuman);	// �`�d�s��̌v�Z(�Б��p)
	void MakeTorquePatternOneLeg(CRHuman* crHuman, int t);			// �`�d�s��̂��߂̃g���N,�̓p�^�[���̐���(�Б��p)
	void SetQuadPROneLeg(CRHuman* crHuman);							// �񎟌v��@�̖ړI�֐����v�Z(�Б��p)

	void CalcAnkleZMP(CRHuman* crHuman);

	void AnkleBalance(CRHuman* crHuman);
	void ChangeParameterSize();
	void SetBalanceState(TBlance state);

	bool changeQDPR;
	bool balanceStateChange;
	float cogDownRate;
	Vec3d cog_zmp,ankleR_zmp,ankleL_zmp;

};

}
#endif // !defined(CRBALANCE_MULTI_LEG_H)
