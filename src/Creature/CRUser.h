// CRUser.h: CRUser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Creature/CRHuman.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>
#include <vector>
//#include "SLCamera.h"

#if !defined(AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
#define AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRUser{
	class CRUserSpring{
	public:
		Spr::PHSolid* solid;
		Spr::Vec3f pos, force, targetPos, targetVel, attraction;
		bool bTarget;

		CRUserSpring():solid(NULL){}
		CRUserSpring(Spr::PHSolid* s, Spr::Vec3f p):solid(s), pos(p){}
		Spr::Vec3f GetPos();
		Spr::Vec3f GetVel();
		Spr::Vec3f GetForce();
		void AddForce(Spr::Vec3f f){
			force = f;
			if (solid) solid->AddForce(f, GetPos());
		}
	};
	class CRUserSprings:public std::vector<CRUserSpring>{
	};

public:
	#define SpringNum 6

	CRUser();
	virtual ~CRUser();

	void Init(Spr::SGScene* scene);

	void ConnectSLCamera();
	void DisconnectSLCamera();
	void OnKeyDown(UINT nChar); 

	bool Draw(Spr::GRRender* render);
	void SetUserSprings();
//	bool InpuCRUserPosition(Spr::CRHuman& phHuman, float dt,Spr::GRRender* render);
	void Step(Spr::HISpidar4* spidar, float dt);
	void InputUserPosition(float dt);
	void SetTargetPosition(Spr::HISpidar4* spidar);
	void SetTargetPosition2(float dt);
	void SetOneTargetPosition(CRUserSpring* us, Spr::Vec3f pos, Spr::Vec3f vel);
	void SetAttraction(CRUserSpring* us);

	bool bCRUser;
	bool bSLCamera;
	Spr::CRHuman crHuman;

	CRUserSprings usprings;
//	SLCamera slc;	// ラベリングカメラ
};



#endif // !defined(AFX_CRUser_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
