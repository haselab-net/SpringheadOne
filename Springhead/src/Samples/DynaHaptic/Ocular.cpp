// Ocular.cpp: COcular �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ocular.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

COcular::COcular()
: x(0)
, y(0)
, z(0)
, a(0)
, b(0)
, c(0)

{

}

COcular::~COcular()
{

}

void COcular::SetPosition(float fx, float fy, float fz, float fa, float fb, float fc){
	x = fx;
	y = fy;
	z = fz;
	a = fa;
	b = fb;
	c = fc;
}
