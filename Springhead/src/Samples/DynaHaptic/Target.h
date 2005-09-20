// Target.h: CTarget クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <FrameWork/FWAppD3D.h>

#if !defined(AFX_TARGET_H__25B757A0_4879_4BFB_96F2_8C421B6AFDAF__INCLUDED_)
#define AFX_TARGET_H__25B757A0_4879_4BFB_96F2_8C421B6AFDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTarget : public Spr::PHSolid 
{
public:	
	void setPos(float fx,float fy,float fz, float fa,float fb,float fc);
	CTarget();
	virtual ~CTarget();

protected:
	Spr::Affinef af;
	float x;
	float y;
	float z;
	float a;
	float b;
	float c;

};

#endif // !defined(AFX_TARGET_H__25B757A0_4879_4BFB_96F2_8C421B6AFDAF__INCLUDED_)
