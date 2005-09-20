// Binocular.h: CBinocular クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINOCULAR_H__1DC6A737_EBEC_417A_9DAE_19474AC40213__INCLUDED_)
#define AFX_BINOCULAR_H__1DC6A737_EBEC_417A_9DAE_19474AC40213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "Ocular.h" //for test
#endif // _MSC_VER > 1000

class CBinocular  
{
public:
	CBinocular();
	virtual ~CBinocular();

public:
	COcular OcularL;
	COcular OcularR;
	float P1;
	float P2;
	float Sigma;
	float N;
	float V;
	float K;
   	void TransFunc(float *DegLX, float *DegRX, float *DegLY, float *DegRY);

};

#endif // !defined(AFX_BINOCULAR_H__1DC6A737_EBEC_417A_9DAE_19474AC40213__INCLUDED_)
