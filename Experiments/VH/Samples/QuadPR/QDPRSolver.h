// QDPRSolver.h: QDPRSolver クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef QDPR_SOLVER_H
#define QDPR_SOLVER_H

#include <Base/QuadProgram.h>
#include <Base/Affine.h>
#include <Base/Base.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Physics/PhJoint.h>
#include <fstream>
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIZE_J 1000	// 関節の総次元
#define SIZE_P 6	// パラメータの次元（力3次元、トルク3次元）
#define STEP 0.01	// 体の構造と目標の変化
#define ADD 0.1		// Qの対角成分に加えるもの

namespace Spr{;
class QDPRSolver  
{
public:
	QDPRSolver();
	virtual ~QDPRSolver();

	Spr::WBPreciseTimer timer;			// 時間計測
	VQuadProgram<float> quadPR;			// 二次計画法

//////////		関数	 ///////////////
	void Init();
	void Set(int seed);
	void Calc();
	void Out();
	void DataOut();
	void ParamOut();


//////////		変数	 ///////////////
	/*
	int count;					// 試行回数
	//bool bFile;				// ファイルで変数設定するか否か 
	DWORD time;					// 一回の試行にかかった時間
	//bool bSpringhead;			// Springheadの関数を使うか否か

	std::vector<SpVec3d> f_t;	// 各関節にかかる力とトルク（6次元）
	SpVec3d f_t_g;				// 全身にかかる目標の力とトルク

	float force_torque[SIZE_P][SIZE_J];
	float force_torque_g[SIZE_P];
	*/
	
	int count;								// 試行回数
	float coefficient;
	PTM::VMatrixRow<double> tempA;
	PTM::VVector<double> tempB;
	PTM::VVector<double> x_goal;
	PTM::VVector<double> x_cog;
	PTM::VVector<float> optTorque;

	int useJointNum,ankleDOF,ankleNum,balanceDOF;


};
}
#endif // !defined(QDPR_SOLVER_H)

