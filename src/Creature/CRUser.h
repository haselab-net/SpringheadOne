// CRUser.h: CRUser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Creature/CRPuppet.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>

#if !defined(AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
#define AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Spr{;

class CRUser: public CRPuppet{
public:
	SGOBJECTDEF(CRUser);
	CRUser();

	void Load(SGScene* scene);						// 基本設定。FrameWorkのLoadで呼ばれる。
	bool Connect(UTRef<SGScene> scene);

	void SetSprings();
	void SetFixedPos();
	void SetSpidarPos(std::vector<HISpidar4*> spidars);
	void SetExpectedPos(float dt);
};

}

#endif // !defined(AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
