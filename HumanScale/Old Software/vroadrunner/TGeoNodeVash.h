// TGeoNodeVash.h: TGeoNodeVash クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEVASH_H__A540A12D_48D6_4330_B825_99256AE78014__INCLUDED_)
#define AFX_TGEONODEVASH_H__A540A12D_48D6_4330_B825_99256AE78014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#include "TGeoNodeVashPoint.h"
#include "main.h"


//#define VASHNUM 30 //vashの数
//#define VASHSTATE 5 //vashの置く位置を変える 状態数

class TGeoNodeVash : public TGeoNode  
{
public:
	// vashの数が、VASHNUM
	//const static Vec3f vashPos[VASHSTATE][VASHNUM];
	const static int BaseLine;// = 100+2;

public:
	bool isDraw;
	bool isTextDraw;
	float radius_ratio;
	int val;

	TGeoNodeVashPoint** pp_VashPoint;
public:
	virtual void InitInstance();
	virtual void Draw();
	virtual bool isGetVash();
	//virtual bool isGetVash(Vec3f& aObj);

	GLfloat dr;
	TGeoNodeVash();
	virtual ~TGeoNodeVash();

};

//
//

class TGeoNodeVashV : public TGeoNodeVash
{
public:
	Affinef afTrans;
public:
	virtual void Draw();
	virtual bool isGetVash();
	TGeoNodeVashV(){
		TGeoNodeVash();
		val = -200;
		dr = 2.0;
	}
};

class TGeoNodeVash10 : public TGeoNodeVash
{
public:
	TGeoNodeVash10(){
		TGeoNodeVash();
		val = 10;
		pp_VashPoint = &vroadrunner::p_VashPoint10;
	}
};

//
//
class TGeoNodeVash50 : public TGeoNodeVash
{
public:
	TGeoNodeVash50(){
		TGeoNodeVash();
		val = 30;
		pp_VashPoint = &vroadrunner::p_VashPoint30;
	}
};

//
//
class TGeoNodeVash100 : public TGeoNodeVash
{
public:
	TGeoNodeVash100(){
		TGeoNodeVash();
		val = 100;
		pp_VashPoint = &vroadrunner::p_VashPoint100;
	}
};

#endif // !defined(AFX_TGEONODEVASH_H__A540A12D_48D6_4330_B825_99256AE78014__INCLUDED_)

