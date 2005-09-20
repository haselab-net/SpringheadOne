#ifndef LVRM_DSHOW_WXAMSOCKSOURCE_H
#define LVRM_DSHOW_WXAMSOCKSOURCE_H
#include <WinDX/Base/WxComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace LVRM {;

extern const GUID CLSID_AMSockSink;

class WXAMSockSourceImp;
class WXAMSockSource:public WXAMFilterBase{
protected:
	WXAMSockSourceImp* imp;
public:
	WXAMSockSource();
	virtual ~WXAMSockSource();
	void StartReceive();
	int NumStream();
};

}	//	namespace LVRM;

#endif
