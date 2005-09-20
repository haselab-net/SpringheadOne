// PHMagneticEngine.h: PHMagneticEngine クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_)
#define AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_

#include "PHSolid.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>

namespace Spr{;

typedef Vec3f			Vector;
DEF_RECORD(XMagneticEngine,{
	GUID Guid(){ return WBGuid("A6217078-938D-4bdc-9AA8-59738B5C0CC6"); }
	Vector pos;
});


class PHMagneticEngine : public SGBehaviorEngine  
{
public:
	void ApplyForce();
	bool AddChildObject(SGObject* o,SGScene* s);
	XMagneticEngine	mag;
	PHSolids		solids;
	virtual void Step(SGScene*s);
	SGOBJECTDEF(PHMagneticEngine);
	PHMagneticEngine();
	virtual ~PHMagneticEngine();

};

}
#endif // !defined(AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_)
