// CRBallBalanceQuadPR.h: CRBallBalanceQuadPR クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALLBALANCE_QUADPR_H
#define CRBALLBALANCE_QUADPR_H

#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#include "CRBallBalance.h"

#define ANKLE_NUM	2
#define ANKLE_DOF	6	// 足首(踵)の自由度(並進3,回転3)
#define BALANCE_DOF	6	// バランスに必要な自由度(並進3,回転3)

namespace Spr{;

class CRVirtualHuman;

class CRBallBalanceQuadPR:public CRBallBalance{

public:
	CRBallBalanceQuadPR();
	virtual ~CRBallBalanceQuadPR();

	// クラスのインスタンス
	VQuadProgram<double> quadPR;								// 二次計画法
	WBPreciseTimer timer;										// 時間計測

	/////// TEST関数&変数　////////
	float transTime;	// 伝播行列計算時間
	float quadTime;		// 二次計画法計算時間


	//////////　 関数　　//////////
	// 諸処理関数
	bool CompNoJoint(int jointNum,CRBallHuman* crHuman);			// 最適化で使わない関節かどうかを判断
	void DataOut(CRBallHuman* CRBallHuman);								// ファイルにデータを出力

	// 初期設定
	void Init(SGScene* scene,CRBallHuman* crHuman);					// 初期化
	void SetBalanceParameter(CRBallHuman* crHuman);					// バランスに必要なパラメータ設定

	// 二次計画法でのバランス
	virtual void ModifyBody(SGScene* scene,CRBallHuman* crHuman);	// バランスをとる一連の処理(バランスに必要な重心トルク計算は別）
	void SetTargetJointTorque(SGScene* scene,CRBallHuman* crHuman);	// 各関節の目標トルクの取得
	void CalcTransMatrix(SGScene* scene,CRBallHuman* crHuman);		// 伝播行列の計算
	void MakeTorquePattern(CRBallHuman* crHuman, int t);			// 伝播行列のためのトルク,力パターンの生成
	Vec3d GetJointAccel(PHJoint1D* joint);						// 関節の並進加速度の取得
	virtual void SetQuadPR(CRBallHuman* crHuman);							// 二次計画法の目的関数を計算
	void QuadPRSolve();											// 二次計画法を解く
	void SetBalanceTorque(CRBallHuman* crHuman);					// 最適(バランス)トルクを関節にセットする
	bool BalanceCheck(CRBallHuman* crHuman);
	
	// 足首でバランスを保障,足裏が浮かないようにする
	virtual void AnkleBalance(CRBallHuman* crHuman);						// 二次計画法で取りきれないバランスを足首で保障
	Vec3d AdjustAnkleTorque(CRBallHuman* crHuman,
				Vec3d ankleForce,Vec3d ankleTorque,int ankle);	// 足裏が浮かないようにトルクを調節
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);	// ZMPが接触多角形内にあるかチェック
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleRealZmpR(){return ankleRealZmp[0];}
	Vec3f GetAnkleRealZmpL(){return ankleRealZmp[1];}
	Vec3f GetAnkleGoalZmpR(){return ankleGoalZmp[0];}
	Vec3f GetAnkleGoalZmpL(){return ankleGoalZmp[1];}

	void RenewConstraint(CRBallHuman* crHuman);
	void NewConstraint(CRBallHuman* CRBallHuman,PHSolid* heel,Vec3f* maxRange,Vec3f* minRange,int ankleSide);


	// TEST
	void AnkleBalance2(CRBallHuman* CRBallHuman);
	Vec3d AdjustAnkleTorque2(CRBallHuman* crHuman,Vec3d footForce,
							Vec3d footTorque,Vec3d zmp,int ankle);	// 足裏が浮かないようにトルクを調節
	Vec3d cog_zmp,footR_zmp,footL_zmp;
	bool bAdjust;

	void OffSpring();

	//////////　 変数　　//////////
	unsigned int ankleNum;							// 足の数
	unsigned int ankelDof;							// 足首(踵)の自由度(並進3,回転3)
	unsigned int balanceDof;						// バランスに必要な自由度(並進3,回転3)
	// 伝播行列
	PTM::VMatrixRow<float>	transT;					// 各関節トルクの伝播行列T(トルクの伝播）
	PTM::VMatrixRow<float>	transF;					// 各関節トルクの伝播行列F(力の伝播)
	PTM::VVector<float>		transM;					// 各関節トルクの伝播行列M(質量の伝播)
	// 二次計画法のX_cog = tempA*t + tempB
	PTM::VMatrixRow<double> tempA;
	PTM::VVector<float> tempB;
	// 各関節の目的トルク(到達運動のためのトルク)
	PTM::VVector<float> targetTorque;				// 各関節の目的トルク(t0)
	PTM::VVector<float> targetTorqueP;				// 各関節の目的トルク(t0)のproportional成分
	PTM::VVector<float> targetTorqueD;				// 各関節の目的トルク(t0)のdifferential成分
	// 二次計画法の目的関数の係数
	float coefficient;								// バランス重視 or 到達運動重視
	float coefficientP;								// 目標トルクP成分と最適なトルクの差を最小にする項の係数
	float coefficientD;								// 目標トルクD成分と最適なトルクの差を最小にする項の係数
	PTM::VVector<float> coefficientJoint;			// 各関節のトルク抑制係数(大きいと抑制)

	PTM::TVector<BALANCE_DOF,double> targetX;		// バランスの目標
	PTM::VVector<float> optTorque;					// 最適トルク
	float qdprBalanceRate;							// バランスに必要なトルクの何割を二次計画法で解くか
	Vec3f ankleRealZmp[2];							// 両足のリアルZMP
	Vec3f ankleGoalZmp[2];							// 両足の初期目標ZMP
	unsigned int useJoNum;							// 最適化で使う関節の数
	std::vector<Vec3d> unitVec;						// X,Y,Zの単位ベクトル
	int count;										// 二次計画法の試行回数
	
};

}		//	end namespace Spr
#endif // !defined(CRBallBalance_QUADPR_H)
