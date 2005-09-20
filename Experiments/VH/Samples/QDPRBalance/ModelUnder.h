// ModelUnder.h: ModelUnder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////


#ifndef MODEL_UNDER_H
#define MODEL_UNDER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include<Creature/CRHuman.h>
#include<Creature/CRPuppet.h>
#include "BalanceUnder.h"

namespace Spr{;
class ModelUnder : public CRHuman  
{
public:
	ModelUnder();
	virtual ~ModelUnder();

	BalanceUnder balance;
	CRPuppet::PositionSpring stayPosi[2];
	CRPuppet::PostureSpring  stayPost[2];


	void Load(SGScene* scene);
	virtual void LoadDerivedModel(SGScene* scene);
	void SetJointSpring(float dt);

	void MaintainBalance(SGScene* scene,GRRender* render);
	void RegistSupportParts(UTRef<SGScene> scene);
	void ExceptContactParts(CREnumContactVertex* contact);
	void SetJointInitAngle();
	
};

}		// end namespace
#endif // !defined(MODEL_UNDER_H)
