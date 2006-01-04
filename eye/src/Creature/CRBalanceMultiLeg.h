// CRBalanceMultiLeg.h: CRBalanceMultiLeg クラスのインターフェイス
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
	
	void ModifyBody(SGScene* scene,CRHuman* crHuman);				// バランスをとる一連の処理(バランスに必要な重心トルク計算は別）
	void CalcBalanceForce(SGScene* scene,CRHuman* crHuman);
	void CalcBalanceTorque(SGScene* scene, CRHuman* crHuman);
	void CalcTransMatrixOneLeg(SGScene* scene,CRHuman* crHuman);	// 伝播行列の計算(片足用)
	void MakeTorquePatternOneLeg(CRHuman* crHuman, int t);			// 伝播行列のためのトルク,力パターンの生成(片足用)
	void SetQuadPROneLeg(CRHuman* crHuman);							// 二次計画法の目的関数を計算(片足用)

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
