// QDPR.h: QDPR クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef QDPR_H
#define QDPR_H

//#include<Creature/CRVirtualHuman.h>
#include "BalanceUnder.h"
#include <Base/QuadProgram.h>
#include <WinBasis/WBPreciseTimer.h>



namespace Spr{;

class QDPR
{
public:
	QDPR();
	virtual ~QDPR();

	Spr::WBPreciseTimer timer;			// 時間計測
	VQuadProgram<float> quadPR;			// 二次計画法

//***********  関数  ***********//

	void Init(BalanceUnder* human);

	void Step(BalanceUnder* human);
	void Set(BalanceUnder* human);
	void Calc();
	void SetBalanceParameter(BalanceUnder* human);
	void SetQuadPR();
	void DataOut();

//***********  変数  ***********//

	int count;								// 試行回数
	float coefficient;
	PTM::VMatrixRow<double> tempA;
	PTM::VVector<double> tempB;
	PTM::VVector<double> x_goal;
	PTM::VVector<double> x_cog;
	PTM::VVector<float> optTorque;

	int useJointNum,ankleDOF,ankleNum,balanceDOF;


};

}		//	end namespace Spr

#endif // !defined(AFX_QDPR_H__00647B99_7E98_4F17_B8BF_F5E1DBF70656__INCLUDED_)
