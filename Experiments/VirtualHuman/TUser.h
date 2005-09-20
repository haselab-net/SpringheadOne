// TUser.h: TUser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Physics/PHHuman.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>
#include <vector>
#include "SLCamera.h"

#if !defined(AFX_TUSER_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
#define AFX_TUSER_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TUserSpring{
public:
	Spr::PHSolid* solid;
	Spr::Vec3f pos, force, targetPos, targetVel, attraction;
	bool bTarget;

	TUserSpring():solid(NULL){}
	TUserSpring(Spr::PHSolid* s, Spr::Vec3f p):solid(s), pos(p){}
	Spr::Vec3f GetPos();
	Spr::Vec3f GetVel();
	Spr::Vec3f GetForce();
	void AddForce(Spr::Vec3f f){
		force = f;
		if (solid) solid->AddForce(f, GetPos());
	}
};
class TUserSprings:public std::vector<TUserSpring>{
};

class TUser{
public:
	#define SpringNum 6

	TUser();
	virtual ~TUser();

	void ConnectSLCamera();
	void DisconnectSLCamera();
	void OnKeyDown(UINT nChar); 

	bool Draw(Spr::GRRender* render);
	void SetUserSprings(Spr::PHHuman& phHuman);
	void SetUserSprings2(Spr::PHHuman& phUser, Spr::PHHuman& phHuman);
//	bool InputUserPosition(Spr::PHHuman& phHuman, float dt,Spr::GRRender* render);
	bool InputUserPosition(Spr::PHHuman& phHuman, Spr::HISpidar4* spidar, float dt,Spr::GRRender* render);
	void SetTargetPosition(Spr::HISpidar4* spidar);
	void SetOneTargetPosition(TUserSpring* us, Spr::Vec3f pos, Spr::Vec3f vel);
	void SetAttraction(TUserSpring* us);


	bool bTUser;
	bool bSLCamera;

	TUserSprings usprings;
	SLCamera slc;	// ラベリングカメラ
};



#endif // !defined(AFX_TUSER_H__A1EDB21B_29B5_48A4_853C_1C38AE2724ED__INCLUDED_)
