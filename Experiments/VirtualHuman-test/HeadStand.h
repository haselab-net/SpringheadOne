// HeadStand.h: HeadStand クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef HEADSTAND_H
#define HEADSTAND_H

#include <Graphics/GRRender.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"

class HeadStand  
{
public:
	HeadStand();
	virtual ~HeadStand();
	std::vector<Spr::PHJoint*> hSJoint;
	Spr::PHJoint *joHeadStand1;
	Spr::PHJoint *joHeadStand2;
	Spr::PHSolid *soHeadStand;
	void SaveHSTorque();
	std::vector<double> HeadStandTorque;
	bool checkZero;
	double checkCount;
	bool SetHSTorque(float time, float dt);

};

#endif // !defined(AFX_HEADSTAND_H__614F64F0_93C2_4295_9B4C_30290350F2A3__INCLUDED_)
