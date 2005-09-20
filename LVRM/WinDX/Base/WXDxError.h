#ifndef LVRM_DDRAW_CCKDXERROR_H
#define LVRM_DDRAW_CCKDXERROR_H
#include "WXDXBase.h"
#include <string>

#define DXERRMSG(n, msg) \
	Message(n, (msg), __FILE__, __LINE__)
#define CKDXERR(err)	\
	Check((err), "", __FILE__, __LINE__)
#define CKDXERRMSG(err, msg)	\
	Check((err), (msg), __FILE__, __LINE__)

#ifndef TRACE
#define TRACE WXDXError::DebugPrintF
#endif

namespace LVRM {
class WXDXError
	{
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
	virtual bool Check(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
	virtual void Message(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
    static void DebugPrintF(const char* f,...);
	};
}	//	namespace LVRM

#endif