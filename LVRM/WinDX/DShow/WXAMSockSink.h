#ifndef LVRM_DSHOW_WXAMSOCKSINK_H
#define LVRM_DSHOW_WXAMSOCKSINK_H
#include <WinDX/Base/WxComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace LVRM {;

extern const GUID CLSID_AMSockSink;

class WXAMSockSinkImp;
class WXAMSockSink:public WXAMFilterBase{
protected:
	WXAMSockSinkImp* imp;
public:
	WXAMSockSink();
	WXAMSockSink(WXAMSockSink& ref);
	virtual ~WXAMSockSink();
	void MediaType(const GUID& t);
	void MediaSubtype(const GUID& t);
};

}	//	namespace LVRM;

#endif
