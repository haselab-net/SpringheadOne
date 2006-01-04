// CRVirtualHuman.h: CRVirtualHuman クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRVIRTUALHUMAN_H
#define CRVIRTUALHUMAN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CRHuman.h"
#include "CRPuppet.h"
#include "CRBalanceQuadPR.h"
//#include "CRBalanceMultiLeg.h"
#include "CRReachingMovment.h"
#include "CRReachingMovmentPD.h"

#define ANKLE_NUM 2

namespace Spr{;

class CRVirtualHuman : public CRHuman  
{
public:
	SGOBJECTDEF(CRVirtualHuman);
///////////////     変数     ///////////////
	CRPuppet::PostureSpring  stayPost[2];
	CRBalanceQuadPR balanceQDPR;		// バランスをとるクラス
	//CRBalanceMultiLeg balanceQDPR;		// バランスをとるクラス
	CRReachingMovment rMovment;			// 到達運動クラス
	CRReachingMovmentPD rMovmentPD;
	
///////////////     関数     ///////////////
	CRVirtualHuman();
	virtual ~CRVirtualHuman();

	void Draw(GRRender* render);					// 描画
	void OnKeyDown(unsigned int nChar,SGScene* scene);
		
	// バランス関係
	void MaintainBalance(SGScene* scene);			// バランスをとる
	virtual void RegistNoUSeJoint();				// 最適化(バランス)で使わない関節を登録

	// 到達運動
	void ReachingMovment(SGScene* scene);

	// virtual(Loadで呼ばれるもの）
	void LoadDerivedModel(SGScene* scene);						// モデルのロード&セット
	virtual void SetJointSpring(float dt);						// 体の硬さを設定
	virtual void SetJointInitAngle();									// 初期姿勢
	float GetChildMass(PHJointBase* j);							// あるJointの子のSolidの重さを取得
	virtual void SetBodyNum();									// バランスなどに必要なSolid,Jointの番号の登録

	// TEST
	void OffSpring();

protected:
	// 安定領域関係
	virtual void RegistSupportParts(UTRef<SGScene> scene);				// 安定領域計算で必要なものを登録
	void ExceptContactParts(CRContactDetector* contact);		// 安定領域の接触点に考えないSolid登録

};

}		// end namespace
#endif // !defined(CRVIRTUALHUMAN_H)
