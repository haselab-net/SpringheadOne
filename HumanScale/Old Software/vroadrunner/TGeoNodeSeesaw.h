// TGeoNodeSeesaw.h: TGeoNodeSeesaw クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODESEESAW_H__93003892_F83E_4B63_A9AD_C5F71D9039D4__INCLUDED_)
#define AFX_TGEONODESEESAW_H__93003892_F83E_4B63_A9AD_C5F71D9039D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"

class TGeoNodeSeesaw : public TGeoNode  
{
public:
	void SetCenter(Vec3f center);
	virtual void SetState();
	float slope; // Rad(20);
	GLfloat w,h,l;
	Vec3f center;
	Vec3f globalCenter;
	int startState; //
	int state; // 正負ゼロ
	int pre_state; //
	char shaft; //回転軸
	virtual void InitInstance();
	virtual void Draw();
	TGeoNodeSeesaw();
	virtual ~TGeoNodeSeesaw();

};

class TGeoNodeSeesawX : public TGeoNodeSeesaw 
{
public:
	virtual void SetState();
	/*
	virtual void SetState(){
	}
	*/

	TGeoNodeSeesawX(){
		TGeoNodeSeesaw();
		shaft = 'x';
	}

};

class TGeoNodeSeesawZ : public TGeoNodeSeesaw 
{
public:
	virtual void SetState();
	/*
	virtual void SetState(){
	}
	*/

	TGeoNodeSeesawZ(){
		TGeoNodeSeesaw();
		shaft = 'z';
	}
};

#endif // !defined(AFX_TGEONODESEESAW_H__93003892_F83E_4B63_A9AD_C5F71D9039D4__INCLUDED_)
