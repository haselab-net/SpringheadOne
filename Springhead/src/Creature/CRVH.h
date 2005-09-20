// CRVH.h: CRVH クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRVH_H
#define CRVH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRVirtualHuman.h"

namespace Spr{;

class CRVH : public CRVirtualHuman  
{
public:
	SGOBJECTDEF(CRVH);
	CRVH();
	virtual ~CRVH();

	// CRHuman 
	virtual bool Connect(UTRef<SGScene> scene);
	virtual void SetSolidInfo();
	virtual void SetJointInfo();
	virtual void SetJointSpring(float dt);
	virtual void SetJointInitAngle();

	// CRVirtualHuman
	virtual void SetBodyNum();
	virtual void RegistNoUSeJoint();
	virtual void RegistSupportParts(UTRef<SGScene> scene);
	
};

}		// end namespace
#endif // !defined(CRVH_H)
