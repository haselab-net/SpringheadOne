// Balance.h: Balance クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_H
#define BALANCE_H

#include <Creature/CRBalance.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#define ANKLE_DOF_ 3
#define ANKLE_NUMBER 1
#define DOF_NUM 6

namespace Spr{;

class Model;

class Balance : public CRBalance  
{
public:
	Balance();
	virtual ~Balance();
	VQuadProgram<double> quadPR;							// 二次計画法
	WBPreciseTimer timer;

	//	  関数		//
	virtual void Init(SGScene* scene,CRHuman* crHuman);			// 初期化
	virtual void SetBalanceParameter(CRHuman* crHuman);			// バランスをとるための目標のパラメータを設定

	void Measure(SGScene* scene,CRHuman* crHuman);				// バランスに必要な計測
	virtual void ModifyBody(SGScene* scene,CRHuman* crHuman);	// 二次計画法でバランスをとる
	
	void SetPreQuadPR(SGScene* scene,CRHuman* crHuman);						// CalcAccelなどをする前に必要なパラメータを取得(セットする
	void CalcTransMatrix(SGScene* scene,CRHuman* crHuman);		// 伝播(各関節のトルクの重心に対する影響を表す)行列を作成
	void MakeTorquePattern(CRHuman* crHuman, int t);			// 伝播を求めるためのトルク、力パターンの作成
	Vec3d GetJointAccel(PHJoint1D* joint);						// 子のSolidの加速度,角加速度をJointの加速度へ変換
	void SetQuadPR(CRHuman* crHuman);							// Q,Cの設定
	void QuadPRSolve();											// 二次計画法を解く
	void SetJointTorque(CRHuman* crHuman);						// 最適化のトルクを関節にセットする。
	void SetAnkle(CRHuman* crHuman);							// 最適解とバランスに必要なトルクの差を足首にかける。

	void RegistNoJoint(CRHuman* crHuman);						// 最適化で使わない関節を決める
	bool CompNoJoint(int jointNum);								// 最適化で使わない関節かどうかを判断
	void DataOut(CRHuman* crHuman);								// xlsに必要なデータを出力

	void AdjustAnkleTorque(CRHuman* crHuman,
			Vec3d& ankleForce,Vec3d& ankleTorque,int ankle);	// 足裏が浮かないようにトルクを調節
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);	// ZMPが接触多角形内にあるかチェック
	Vec3f ClosestPoint(Vec3f zmp,CRSupportArea* supportArea);	// 最近傍点を求める
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleZMP(){return ankleZmp;}
	Vec3f ankleZmp;



	//	  変数		//
	// 伝播行列
	PTM::VMatrixRow<float>	transT;					// 各関節トルクの伝播行列T(トルクの伝播）
	PTM::VMatrixRow<float>	transF;					// 各関節トルクの伝播行列F(力の伝播)
	PTM::VVector<float>		transM;					// 各関節トルクの伝播行列M(質量の伝播)
	// 各関節の目的トルク(到達運動のためのトルク)
	PTM::VVector<float> targetTorque;				// 各関節の目的トルク(t0)
	PTM::VVector<float> targetTorqueP;				// 各関節の目的トルク(t0)のproportional成分
	PTM::VVector<float> targetTorqueD;				// 各関節の目的トルク(t0)のdifferential成分
	// 二次計画法の目的関数の係数
	float coefficient;								// バランス重視 or 到達運動重視
	float coefficientP;								// 目標トルクP成分と最適なトルクの差を最小にする項の係数
	float coefficientD;								// 目標トルクD成分と最適なトルクの差を最小にする項の係数
	PTM::VVector<float> coefficientJo;				// 各関節のトルク抑制係数(大きいと抑制)

	float qdprBalanceRate;							// バランスに必要なトルクの何割を二次計画法で解くか
	Vec3f cogForce;

	std::vector<Vec3d> unitVec;						// X,Y,Zの単位ベクトル
	std::vector<int> noJoint;						// 最適化で使わない関節番号
	int useJoNum;									// 最適化で使う関節の数
	int count;										// 二次計画法の試行回数
	
	// TEST
	PTM::VVector<float> BalanceTorque;
	PTM::VMatrixRow<double> optA;
	PTM::VVector<float> optB;
};

}		// end namespace
#endif // !defined(BALANCE_H)
