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
#include <Base/TQuaternion.h>
#include <Base/Affine.h>
#include <vector>

#include "CRSupportArea.h"
#include "CRBalanceQuadPR.h"

namespace Spr{;

class CRHuman;

class CRBalance{
public:
	CRBalance();
	virtual ~CRBalance();

	PHContactEngine* contactEngine;	// 接触エンジン
	PHJointEngine* jointEngine;		// 関節エンジン
	CRSupportArea supportArea;		// 安定領域の計算クラス
	CRBalanceQuadPR balanceQuadPR;

private:
	bool bCalc;						// 計算、制御してよいか否か
	bool bReload;					// リロードするか否か
	bool bControl;					// 力をかけるか否か
	Vec3f gravity;					// 重力加速度

	Vec3f cogPos;					// VHの重心の位置
	Quaterniond targetPostureQ;		// 目標の姿勢
	Quaterniond nowPostureQ;		// 現在の姿勢
	Vec3f targetTorque;				// 現在の姿勢から目標の姿勢にするのに必要なトルク
	Vec3f targetForce;
	float dampa;					// ダンパ係数
	Vec3f spring;					// バネ係数
	float spring_no;				// 無次元化したバネ係数
	float dampa_no;					// 無次元化したダンパ係数
		
	Vec3f targetZmp;				// 目標ＺＭＰ
	Vec3f targetAccel;				// 重心の目標加速度

	
public:
//---------　基本的なインタフェース　---------//
	void Step(SGScene* scene,GRRender* render,CRHuman* crHuman);
	void Load(SGScene* scene,CRHuman* crHuman);
	void OnKeyDown(UINT &nChar,CRHuman* crHuman);
	void Draw(GRRender* render);

//-----------------　処理　----------------//

	void CalcBodyTorque(SGScene* scene,CRHuman* crHuman);	// 目標の姿勢にするために重心に必要なトルクの計算
	void CalcTargetZMP(SGScene* scene,CRHuman* crHuman);	// 目標ZMPの計算
	void ControlBody(CRHuman* crHuman);						// バランスを取るための制御
	void AppControl(SGScene* scene,CRHuman* crHuman);		// アプリケーションの制御（力を加える、リロードする）
	void DrawZMP(GRRender* render);							// ZMPの描画
	bool GetReloadCheck(){ return bReload;}					// リロードするか否か
	bool GetControlCheck(){ return bControl;}				// コントロールを続けるか否か
	Vec3f GetCog(){return cogPos;}							// 重心の位置を返す
	Vec3f GetTargetZMP(){return targetZmp;}					// 目標ZMPの取得
	Vec3f GetTargetForce(){ return targetForce;}
	Vec3f GetTargetTorque(){ return targetTorque;}

	// ZMPが安定領域内にいるか監視 
	bool ZmpIncludeCheck(CRSupportArea& supportArea);
	bool PosIncludeTriangle(Vec3f t_a, Vec3f t_b, Vec3f t_c, Vec3f zmp);



};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_H)
