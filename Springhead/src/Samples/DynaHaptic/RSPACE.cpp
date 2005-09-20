// RSPACE.cpp: RSPACE クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dynahaptic.h"
#include "RSPACE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RSPACE::RSPACE()
{	
	static const int DISTANCE_AFTER_ROBOTX = SPACE_LENGTH/2;
	static const int DISTANCE_AFTER_ROBOTY = SPACE_WIDTH/2;
	static const int DISTANCE_AFTER_ROBOTZ = SPACE_HIGH/2;

	

}

RSPACE::~RSPACE()
{

}
