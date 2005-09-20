// Step.h: Step クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEP_H__D82A8162_9A04_4BEA_974D_06FE378B33E3__INCLUDED_)
#define AFX_STEP_H__D82A8162_9A04_4BEA_974D_06FE378B33E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class Step  
{
public:
	int step;
	float walkvel;
	void WalkInStep();
	Step();
	virtual ~Step();

private:
	DWORD dwInterval,dwStepTime;
	double grav_x[6],grav_y[6],d_grav;
	float pitch;
};

#endif // !defined(AFX_STEP_H__D82A8162_9A04_4BEA_974D_06FE378B33E3__INCLUDED_)
