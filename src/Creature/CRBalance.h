// CRBalance.h: CRBalance クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALANCE_H
#define CRBALANCE_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Base/Affine.h>
#include <vector>

#include "CRSupportArea.h"

namespace Spr{;

class CRHuman;

// バランスをとるための基本クラス
class CRBalance{
public:
	CRBalance();
	virtual ~CRBalance();

	PHJointEngine*		jointEngine;		// 関節エンジン
	PHGravityEngine*	gravityEngine;		// PHGravityEngine

//protected:
public:
	Vec3f cogPos;					// VHの重心の位置
	Quaterniond targetPostureQ;		// 目標の姿勢
	Quaterniond nowPostureQ;		// 現在の姿勢
	Vec3f balanceTorque;				// 現在の姿勢から目標の姿勢にするのに必要なトルク
	Vec3f balanceForce;
	Vec3f targetZmp;				// 目標ＺＭＰ
	Vec3f targetAccel;				// 重心の目標加速度

	// 無次元化したバネ,ダンパ係数
	Vec3f spring_noV;				// バランストルクのバネ係数
//	float spring_no;				// バランストルクのバネ係数
	float dampa_no;					// バランストルクのダンパ係数
	float springCog_no;				// 重心を安定領域内にする力のバネ係数
	float dampaCog_no;				// 重心を安定領域内にする力のダンパ係数

public:
//---------　基本インタフェース　---------//
	void Step(SGScene* scene,CRHuman* crHuman);
	void Load(SGScene* scene,CRHuman* crHuman);
	virtual void OnKeyDown(UINT &nChar,CRHuman* crHuman);
	virtual void Draw(GRRender* render,CRSupportArea* supportArea);
	virtual void Init(SGScene* scene,CRHuman* crHuman);


//-----------------　処理　----------------//
	virtual void Measure(SGScene* scene,CRHuman* crHuman);		// バランスに必要な基本的なことを計測
	virtual void ModifyBody(SGScene* scene,CRHuman* crHuman);	// 体にトルクをかけバランスをとる
	virtual void SetBalanceParameter(CRHuman* crHuman);			// バランスをとるための目標のパラメータを設定

	virtual void CalcBalanceTorque(SGScene* scene,CRHuman* crHuman);	// バランスをとるために重心に必要なトルクの計算
	virtual void CalcBalanceForce(SGScene* scene,CRHuman* crHuman);		// バランスをとるために重心に必要な力の計算
	void CalcTargetZMP(SGScene* scene,CRHuman* crHuman);		// 目標ZMPの計算
	void ControlBody(CRHuman* crHuman);							// バランスを取るための制御
	void DrawZMP(GRRender* render);								// ZMPの描画
	Vec3f GetTargetZMP(){return targetZmp;}						// 目標ZMPの取得
	Vec3f GetBalanceForce(){ return balanceForce;}				// 重心にかかる目標の力を取得
	Vec3f GetBalanceTorque(){ return balanceTorque;}				// バランスをとるのに必要な重心にかけるトルクを取得
	Vec3f GetTargetAccel(){ return targetAccel;}				// 重心の目標の加速度を取得

	// ZMPが安定領域内にいるか監視 
	bool ZmpIncludeCheck(CRSupportArea& supportArea);
	bool PosIncludeTriangle(Vec3f t_a, Vec3f t_b, Vec3f t_c, Vec3f zmp);

};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_H)
