#ifndef Spr_DSHOW_WXAMSOCKSOURCE_H
#define Spr_DSHOW_WXAMSOCKSOURCE_H
#include <WinDX/Base/WXComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace Spr {;

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

}	//	namespace Spr;

#endif
