// Target.cpp: CTarget �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Target.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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


