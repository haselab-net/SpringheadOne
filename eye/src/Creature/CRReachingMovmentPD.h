// CRReachingMovmentPD.h: CRReachingMovmentPD クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_REACHING_MOVEMENT_PD_H
#define CR_REACHING_MOVEMENT_PD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRHuman.h"
#include <iostream>

namespace Spr{;


struct CRMovmentSolid{
	PHSolid* solid;

	double goalTime;		// 到達運動時間
	double nowTime;			// 到達運動を始めてからの経過時間
	int startStep;			// 開始時のシミュレータのステップ数
	Vec3d goalPos;			// 到達地点
	Vec3d startPos;			// スタート地点
	Vec3d pdForce;			// PD制御による力
	double spring,damp;		// PD制御のバネ・ダンパ係数
	double firstDistance;	// スタート地点から到達地点までの距離

	bool bStart;			// 到達運動するか否か
	bool bGoal;				// 到達しているか否か
	bool bStyle;			// true:躍度最小，false:PDのみ
	
	double jerkPos;			// 到達地点までの目標の残り距離(躍度最小によって求められた)
	Vec3d jerkVelocity;		// 目標速度(躍度最小によって求められた)

	std::vector<int> useJoints;	// 到達運動で使用する関節
	std::vector<Vec3f> useJointsSpring;	// 到達運動で使用する関節のもとのバネとダンパ
};

class CRReachingMovmentPD  
{
public:
	CRReachingMovmentPD();
	virtual ~CRReachingMovmentPD();

	CRHuman*			model;

	//////////　 関数　　//////////
	void Step(SGScene* scene);		// 到達運動実行
	void Load(CRHuman* crHuman);
	void Draw(GRRender* render);


	void MinimumJerkModel(SGScene* scene);			// 躍度最小モデルで残りの距離，目標速度を計算
	void PDControl(SGScene* scene);				// 到達運動のためのPD制御の力
	void StartReachingMovment(int i)
			{movmentSolid[i].bStart = true;}		// 到達運動をスタートさせる
	void SetTarget(SGScene* scene,Vec3f goal);		// 到達運動のゴールを設定する
	void RegistMovmentSolid(SGScene* scene			// 到達運動する剛体の登録
		,PHSolid* solid,double k,double b,double finishTime,std::vector<int>& joints);
	void CheckReachingMiovment(SGScene* scene);		// 到達運動を終了するか否か
	bool CheckGoal(CRMovmentSolid* moveSolid,SGScene* scene);
	void Init(CRMovmentSolid* moveSolid);			// 初期化
	void StayCurrentPos();
	void DataOut();

	void StayHere(CRMovmentSolid* moveSolid,int i);
	
	//////////　 変数　　//////////
	std::vector<CRMovmentSolid> movmentSolid;		// 到達運動する剛体の数
	bool pdType;									// true:躍度最小+PD，false:PD
	bool bMove;
	bool offSpring;

};

}		// end namespace
#endif // !defined(CR_REACHING_MOVEMENT_PD_H)
