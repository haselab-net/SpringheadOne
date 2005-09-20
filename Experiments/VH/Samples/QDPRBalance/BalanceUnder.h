// BalanceUnder.h: BalanceUnder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_UNDER_H
#define BALANCE_UNDER_H

#include <Creature/CRBalance.h>
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>

#define ANKLE_NUM	2
#define ANKLE_DOF	6
#define BALANCE_DOF	6	// バランスに必要な自由度(並進3,回転3)

#define R_ANKLE_J	29	// 右足首の関節(最後の)の番号
#define L_ANKLE_J	37	// 左足首の関節(最後の)の番号
#define R_HEEL_S	12	// 右足踵の番号
#define L_HEEL_S	16	// 左足踵の番号

class ModelUnder;
namespace Spr{;

class BalanceUnder : public CRBalance  
{
public:
	BalanceUnder();
	virtual ~BalanceUnder();
	VQuadProgram<double> quadPR;								// 二次計画法
	WBPreciseTimer timer;
	float transTime;
	float quadTime;

	//	  関数		//
	void RegistNoJoint();									// 最適化で使わない関節を決める
	bool CompNoJoint(int jointNum);							// 最適化で使わない関節かどうかを判断

	void Init(SGScene* scene,CRHuman* crHuman);				// 初期化
	void SetBalanceParameter(CRHuman* crHuman);				// バランスに必要なパラメータ設定

	void ModifyBody(SGScene* scene,CRHuman* crHuman);		// バランスをとる一連の処理(バランスに必要な重心トルク計算は別）
	void SetPreQuadPR(SGScene* scene,CRHuman* crHuman);		// 各関節の目標トルクの取得
	void CalcTransMatrix(SGScene* scene,CRHuman* crHuman);	// 伝播行列の計算
	void MakeTorquePattern(CRHuman* crHuman, int t);		// 伝播行列のためのトルク,力パターンの生成
	Vec3d GetJointAccel(PHJoint1D* joint);					// 関節の並進加速度の取得
	void DataOut(CRHuman* crHuman);							// ファイルにデータを出力
	void SetQuadPR(CRHuman* crHuman);						// 二次計画法の目的関数を計算
	void QuadPRSolve();										// 二次計画法を解く
	void SetJointTorque(CRHuman* crHuman);					// 最適トルクを関節にセット
	void SetAnkle(CRHuman* crHuman);						// 二次計画法で取りきれないバランスを足首で保障


	// 足裏が浮かないようにトルクを抑える
	void SaveAnkleTorque(CRHuman* crHuman,Vec3d& ankleForce,Vec3d& ankleTorque,int ankle);
	bool AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea);						// ZMPが接触多角形内にあるかチェック
	Vec3f ClosestPoint(Vec3f zmp,CRSupportArea* supportArea);
	Vec3f CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp);

	Vec3f GetAnkleZmpR(){return zmp[0];}
	Vec3f GetAnkleZmpL(){return zmp[1];}

	// TEST
	void CalcTransMatrix2(SGScene* scene,CRHuman* crHuman);	// 伝播行列の計算
	void MakeTorquePattern2(CRHuman* crHuman, int t);		// 伝播行列のためのトルク,力パターンの生成
	void SetQuadPR2(CRHuman* crHuman);						// 二次計画法の目的関数を計算

//private:
public:
	//	  変数		//
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
	PTM::VVector<float> coefficientJo;				// 各関節のトルク抑制係数(大きいと抑制)

	float qdprBalanceRate;							// バランスに必要なトルクの何割を二次計画法で解くか

	std::vector<Vec3d> unitVec;						// X,Y,Zの単位ベクトル
	std::vector<int> noJoint;						// 最適化で使わない関節番号
	unsigned int useJoNum;							// 最適化で使う関節の数
	int count;										// 二次計画法の試行回数

	PTM::TVector<BALANCE_DOF,double> targetX;

	// 片足ずつのＺＭＰ
	Vec3f zmp[2];


};

}		// end namespace

#endif // !defined(BALANCE_UNDER_H)
