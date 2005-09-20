// Balance.h: Balance クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_H
#define BALANCE_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>

#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PHHuman.h>

#include <Base/TQuaternion.h>
#include <Base/Affine.h>

#include <vector>

using namespace Spr;

class TBalance  
{
public:
	TBalance();
	virtual ~TBalance();
	//描画のための関数
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}

//////////////////////////////////////////////////////////////////
//						関数								 　//
////////////////////////////////////////////////////////////////

//---------　基本的なインタフェース　---------//
	void Step(SGScene* scene,GRRender* render,PHHuman& human);
	void Load(SGScene* scene,PHContactEngine* contactEngine);
	void OnKeyDown(UINT &nChar);
	void Draw(GRRender* render);


//-----------------　処理　----------------//

	Vec3f CalcTargetZMP(SGScene* scene,PHHuman& human);	//ZMPとトルクの計算
	void SetTarget(SGScene* scene,PHHuman& human);		//計算に必要なパラメータの設定
	void Control(PHHuman& human);						//バランスを取るための制御
	Vec3f GetCOG(PHHuman& human);						//VHの重心の計算


//////////////////////////////////////////////////////////////////
//						変数								 　//
////////////////////////////////////////////////////////////////

	//PHHuman human;
	PHContactEngine* balanceContactEngine;

	bool bCalc;			//計算、制御してよいか否か

	float totalMass;				//総質量

	// 処理Ａ
	Quaterniond targetPostureQ;		//目標の姿勢
	Quaterniond nowPostureQ;		//現在の姿勢
	Vec3f targetVelocity;			//目標の加速度
	Vec3f torque;					//現在の姿勢から目標の姿勢にするのに必要なトルク
	float spring;					//バネ係数
	float dampa;					//ダンパ係数
	Vec3f spring_all;
	float spring_no;				//無次元化したバネ係数
	float dampa_no;					//無次元化したダンパ係数
		
	// 処理Ｂ
	Vec3f targetZmp;				//目標ＺＭＰ
	Vec3f gravity;					//重力加速度
	Vec3f cogPos;					//重心の位置
	Vec3f primaryPos;				//体の基準となるsolidの位置（腰）
	float primarrMass;				//体の基準となるsolidの質量
	Vec3f targetAccel;				//体の基準となるsolidの目標加速度
	
};

#endif // !defined(AFX_BALANCE_H__75359E4A_F1E4_4B0F_978F_DD5F3792DF35__INCLUDED_)
