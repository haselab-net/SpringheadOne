// Tokin.h: Tokin クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKIN_H__7066AA95_515A_45F7_B7BD_B5754ABBCE03__INCLUDED_)
#define AFX_TOKIN_H__7066AA95_515A_45F7_B7BD_B5754ABBCE03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <tchar.h>
#include <iostream.h>
#include <stdio.h>

#include <atlbase.h>
#include <atlimpl.cpp>

#include "TkInput_i.c"
#include "TkInput.h"
#include "TkInputDef.h"

class Tokin  
{
public:
	double angle;
	void Poll();
	void GetDeviceData();
	void SetStdData();
	RETINFO retinfo;
	ITk3dInput* pTk3dInput;
	void Uninit_tokin();
	Tokin();
	virtual ~Tokin();
	void Init_tokin();
};

#endif // !defined(AFX_TOKIN_H__7066AA95_515A_45F7_B7BD_B5754ABBCE03__INCLUDED_)
