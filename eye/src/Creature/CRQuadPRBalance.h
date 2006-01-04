// CRQuadPRBalance.h: CRQuadPRBalance クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRQUADPR_BALANCE_H
#define CRQUADPR_BALANCE_H

#include <WinBasis/WBPreciseTimer.h>
#include <Base/QuadProgram.h>
#include "CRBalance.h"

#define R_ANKLE_J	29	// 右足首の関節(最後の)の番号
#define L_ANKLE_J	37	// 左足首の関節(最後の)の番号
#define R_HEEL_S	12	// 右足踵の番号
#define L_HEEL_S	16	// 左足踵の番号
#define ANKLE_DOF 3
#define ANKLE_NUM 2
#define DOF_NUM 6

namespace Spr{;

//バランスを取るため、重心に必要なトルクを各関節に最適分配してバランスをとるクラス。
class CRQuadPRBalance : public CRBalance  
{
public:
	CRQuadPRBalance();
	virtual ~CRQuadPRBalance();

	// クラスのインスタンス
	VQuadProgram<double> quadPR;								// 二次計画法
	WBPreciseTimer timer;										// 時間計測

	//////////　 関数　　//////////
	void RegistNoJoint(CRHuman* crHuman);						// 最適化で使わない関節を決める
	bool CompNoJoint(int jointNum);								// 最適化で使わない関節かどうかを判断

	std::vector<int> noJoint;						// 最適化で使わない関節番号
};
}		//	end namespace Spr
#endif // !defined(CRQUADPR_BALANCE_H)
