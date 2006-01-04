// CRReachingMovment.h: CRReachingMovment クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_REACHING_MOVEMENT_H
#define CR_REACHING_MOVEMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRHuman.h"

#define JARK_JOINT 7
#define HAND_DOF 3

namespace Spr{;

class CRReachingMovment  
{
public:
	CRReachingMovment();
	virtual ~CRReachingMovment();

	//////////　 関数　　//////////
	void Step(SGScene* scene,CRHuman* crHuman);
	void Load();
	void Draw(GRRender* render);
	void DataOut(CRHuman* crHuman);

	void MinimumJerkModel(SGScene* scene,CRHuman* crHuman);			// 躍度最小モデル
	void MakeJacobian(CRHuman* crHuman,Vec3d targetPos);			// ヤコビアンの計算
	Vec3f MakeOneJacobian(PHJointBase* joint,Vec3f targetPos);		// 一つの関節のヤコビアンを計算
	Vec3f GetWorldPos(PHJointBase* joint);
	void MakeTorque(SGScene* scene,CRHuman* crHuman);
	void SetTarget(SGScene* scene,CRHuman* crHuman,Vec3f goal);
	void StartReachingMovment(){bStart = true;}
	

	//////////　 変数　　//////////
/*
	PTM::VMatrixRow<float> jacobian;		// ヤコビアン
	PTM::VMatrixRow<float> jacobianInv;		// ヤコビアンの擬似逆行列
	PTM::VVector<float> jointTorque;		// 到達運動に使用する関節のトルク
	PTM::VVector<float> jointAngle;			// 到達運動に使用する関節の角度
	PTM::VVector<float> jointVelocity;		// 到達運動に使用する関節の角速度
*/
	PTM::TMatrixRow<HAND_DOF,JARK_JOINT,double> jacobian;		// ヤコビアン
	PTM::TMatrixRow<JARK_JOINT,HAND_DOF,double> jacobianInv;		// ヤコビアンの擬似逆行列
	PTM::TMatrixRow<JARK_JOINT,JARK_JOINT,double> weightMat;		// ヤコビアンの擬似逆行列
	PTM::TVector<JARK_JOINT,double> jointTorque;					// 到達運動に使用する関節のトルク
	PTM::TVector<JARK_JOINT,double> jointAngle;					// 到達運動に使用する関節の角度
	PTM::TVector<JARK_JOINT,double> jointVelocity;				// 到達運動に使用する関節の角速度
	Vec3d jerkPos;												// 躍度最小モデルで出す手先の位置
	Vec3d jerkVelocity;											// 躍度最小モデルで出す手先の速度

	std::vector< PTM::TVector<6,double> > jacobian6;				//ヤコビアン並進＆回転

	bool bStart;
	int startStep;
	double goalTime;
	Vec3d goalPos;
	Vec3d startPos;
	double nowTime;

};

}		// end namespace
#endif // !defined(CR_REACHING_MOVEMENT_H)
