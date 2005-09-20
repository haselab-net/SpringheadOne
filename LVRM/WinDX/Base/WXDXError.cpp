#include "WXDXError.h"
#include <d3drm.h>
#include <d3d.h>
#include <stdio.h>
#pragma hdrstop
namespace LVRM {
WXDXError::WXDXError()
	{
	desc = "";
	err = "";
	fname = "";
	line = 0;
	msg = "";
	num = 0;
	}
bool WXDXError::Check(DWORD hr, const char* m, const char* fn, int ln)
	{
	if (hr == 0) return false;
	bool bDisp = false;
	bDisp |= (num != hr);
	bDisp |= (line != ln);
	bDisp |= (strcmp(fname.c_str(), (fn?fn:"")) == 0);
	bDisp |= (strcmp(msg.c_str(), (m?m:"")) == 0);
	num = hr;
	msg = m ? m : "";
	fname = fn ? fn : "";
	line = ln;
	if (bDisp) DebugPrintF("%s:%d\n    %s(%x):%s %s\n", fname.c_str(), line, err, num, desc, msg.c_str());
	return true;
	}
void WXDXError::Message(DWORD n, const char* m, const char* fn, int ln)
	{
	static char* errMsg[] = {"Message", "Warning", "Error"};
	bool bDisp = false;
	if (strcmp(msg.c_str(), m) != 0) bDisp = true;
	if (!bDisp && n != num) bDisp = true;
	if (!bDisp && ln != line) bDisp = true;
	if (!bDisp && fn != fname) bDisp = true;
	num = n;
	if (num < sizeof(errMsg) / sizeof(errMsg[0]))
		err = errMsg[num];
	else err = "???";
	msg = m ? m : "";
	fname = fn ? fn : "";
	line = ln;
	if (bDisp) DebugPrintF("%s:%d\n    %s:%s\n", fname.c_str(), line, err, msg.c_str());
	}
//////////////////////////////////////////////////////
//	error to string	
//
void WXDXError::DebugPrintF(const char* f,...)
	{
	va_list arglist;
    va_start(arglist, f);
	char buf[4096];
    vsprintf(buf, f, arglist);
    OutputDebugString(buf);
    }
}	//	namespace LVRM
