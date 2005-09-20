// Target.cpp: CTarget クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Target.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CTarget::CTarget()
: x(0)
, y(0)
, z(0)
, a(0)
, b(0)
, c(0)
{

}

CTarget::~CTarget()
{

}

void CTarget::setPos(float fx,float fy,float fz, float fa,float fb,float fc)
{
	x = fx;
	y = fy;
	z = fz;
	a = fa;
	b = fb;
	c = fc;
}


