// Binocular.cpp: CBinocular クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dynahaptic.h"
#include "Binocular.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CBinocular::CBinocular()
{

}

CBinocular::~CBinocular()
{

}

void CBinocular::TransFunc(float *DegLX, float *DegRX, float *DegLY, float *DegRY)
{
	OcularL.ErrIntX += OcularL.RelAngX;
	OcularR.ErrIntX += OcularR.RelAngX;
	
	*DegLX = (P2 * K + P1 * Sigma) * OcularL.ErrIntX
		- (P2 * N + P1 * V) * OcularL.RelAngX
		- (P1 * K + P2 * Sigma) * OcularR.ErrIntX
		+ (P1 * N + P2 * V) * OcularR.RelAngX;

	*DegRX = (P1 * K + P2 * Sigma) * OcularL.ErrIntX
		- (P1 * N + P2 * V) * OcularL.RelAngX
		- (P2 * K + P1 * Sigma) * OcularR.ErrIntX
		+ (P2 * N + P1 * V) * OcularR.RelAngX;


	

	OcularL.ErrIntY += OcularL.RelAngY;
	OcularR.ErrIntY += OcularR.RelAngY;
	*DegLY = (P2 * K + P1 * Sigma) * OcularL.ErrIntY
		- (P2 * N + P1 * V) * OcularL.RelAngY
		- (P1 * K + P2 * Sigma) * OcularR.ErrIntY
		+ (P1 * N + P2 * V) * OcularR.RelAngY;
	*DegRY = (P1 * K + P2 * Sigma) * OcularL.ErrIntY
		- (P1 * N + P2 * V) * OcularL.RelAngY
		- (P2 * K + P1 * Sigma) * OcularR.ErrIntY
		+ (P2 * N + P1 * V) * OcularR.RelAngY;
	*DegRY = - *DegRY;
}