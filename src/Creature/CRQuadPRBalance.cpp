// CRQuadPRBalance.cpp: CRQuadPRBalance クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRQuadPRBalance.h"
#include "CRHuman.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRQuadPRBalance::CRQuadPRBalance()
{

}

CRQuadPRBalance::~CRQuadPRBalance()
{

}
void  CRQuadPRBalance::RegistNoJoint(CRHuman* crHuman){
	noJoint.clear();
	// 上半身
	//	(胸）
	noJoint.push_back(3);
	noJoint.push_back(4);
	noJoint.push_back(5);

	//	(首）
	noJoint.push_back(6);
	noJoint.push_back(7);
	noJoint.push_back(8);

	//	(右手首)
	noJoint.push_back(14);
	noJoint.push_back(15);

	//	(左手首)
	noJoint.push_back(21);
	noJoint.push_back(22);

	// 下半身
	//	(右足首)
	noJoint.push_back(27);
	noJoint.push_back(28);
	noJoint.push_back(29);
	//	(右土踏まず)
	noJoint.push_back(30);

	//	(左足首)
	noJoint.push_back(35);
	noJoint.push_back(36);
	noJoint.push_back(37);

	//	(左土踏まず)
	noJoint.push_back(38);

}
bool CRQuadPRBalance::CompNoJoint(int jointNum){
	for(int i = 0; i < noJoint.size(); i++){
		if(jointNum == noJoint[i])
			return true;
	}
	return false;
}

}		//	end namespace Spr