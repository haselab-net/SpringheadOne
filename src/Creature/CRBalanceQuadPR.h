// CRBalanceQuadPR.h: CRBalanceQuadPR クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALANCE_QUADPR_H
#define CRBALANCE_QUADPR_H

#include <SceneGraph/SGScene.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PHForceField.h>
#include <Base/Affine.h>
#include <Base/TVector.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#include <vector>

namespace Spr{;
class CRHuman;

#define DOF_NUM		6	// 最適化問題で使う自由度の数
#define JOINT_NUM	39	// ジョイント（変数）の数
#define R_ANKLE_J	29	// 右足首の関節(最後の)の番号
#define L_ANKLE_J	37	// 左足首の関節(最後の)の番号
#define R_HEEL_S	12	// 右足踵の番号
#define L_HEEL_S	16	// 左足踵の番号

class CRBalanceQuadPR  
{
public:
	CRBalanceQuadPR();
	virtual ~CRBalanceQuadPR();

//---- バランスを取るため、重心に必要なトルクを各関節に最適分配する ----//

	PHJointEngine* jointEngine;									// PHJointEngine
	PHGravityEngine* gravity;									// PHGravityEngine
	VQuadProgram<double> quadPR;								// 二次計画法
	WBPreciseTimer timer;										// 時間計測

	void Step(CRHuman* crHuman);

	//各関節のトルクの重心に対する影響を表す行列を作成
	void TransTorque(SGScene* scene,CRHuman* crHuman);
	void MakeTorquePattern(CRHuman* crHuman, int t);			// トルク、力パターンの作成
	Vec3d GetJointAccel(PHJoint1D* joint);						// 子のSolidの加速度,角加速度をJointの加速度へ変換
	void QuadPRSolve();											// 二次計画法を解く
	void SetQuadPR(CRHuman* crHuman);							// Q,Cの設定
	void Init(SGScene* scene,CRHuman* crHuman);					// 二次計画法に必要な諸設定
	void DataOut(CRHuman* crHuman);
	void SetJointTorque(CRHuman* crHuman);						// 最適化のトルクを関節にセットする。

	//TEST
	void SetAnkleForce(CRHuman* crHuman);


	PTM::TMatrixRow<DOF_NUM,JOINT_NUM,double> transmissionM;	// 各関節トルクの伝播行列M
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> transmissionD;		// 各関節トルクの伝播行列D
	PTM::TVector<DOF_NUM,double> transmissionC;					// 各関節トルクの伝播行列C
	PTM::TVector<JOINT_NUM,double> targetTorqe;					// 各関節の目的トルク(t0)
	double coefficient;											// 二次計画法の目的関数の目標のトルクと
																// 最適なトルクの差を最初にする項の係数
	
	std::vector<Vec3d> unitVec;									// X,Y,Zの単位ベクトル
	PTM::TMatrixRow<3,6,double> upMat;								// (6×1)の上三つを切り出す行列
	PTM::TMatrixRow<3,6,double> lowMat;							// (6×1)の下三つを切り出す行列
	int count;													// 二次計画法の試行回数

};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_QUADPR_H)
