#ifndef Spr_DDRAW_CCKDXERROR_H
#define Spr_DDRAW_CCKDXERROR_H
#include "WxComBase.h"
#include <string>

#define WXERRMSG(n, msg) \
	Singleton<WXDXError>().Message(n, (msg), __FILE__, __LINE__)
#define WXCHECK(err)	\
	Singleton<WXDXError>().Check((err), "", __FILE__, __LINE__)
#define WXCHECKMSG(err, msg)	\
	Singleton<WXDXError>().Check((err), (msg), __FILE__, __LINE__)

#ifndef TRACE
#define TRACE WXDXError::DebugPrintF
#endif

namespace Spr {;

///	DirectX の 返り値をチェックし、エラーがあればエラーメッセージを表示する。
class SPR_DLL WXDXError{
protected:
	DWORD num;
	char* err;
	char* desc;
	std::string msg;
	std::string fname;
	int line;
public:
	WXDXError();
	DWORD Num(){return num;}
	const char* Err(){return err;}
	const char* Desc(){return desc;}
	bool Check(DWORD n, const char* msg, const char* fn=NULL, int ln=0){
		if (!n) return true;
		return CheckImp(n, msg, fn, ln);
	}
	bool CheckImp(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
	void Message(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
    static void __cdecl DebugPrintF(const char* f,...);
	static bool DDErrorToString(char*& err, char*& desc, HRESULT error);
	static bool D3DErrorToString(char*& err, char*& desc, HRESULT error);
};

}	//	namespace Spr

#endif