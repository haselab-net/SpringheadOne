#ifndef Spr_DSHOW_WXAMCALLBACKRENDER_H
#define Spr_DSHOW_WXAMCALLBACKRENDER_H
#include <WinDX/Base/WXComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace Spr {;

extern const GUID CLSID_AMDDSRender;

class WXAMCallbackRendererImp;
class WXAMCallbackRenderer:public WXAMFilterBase{
protected:
	WXAMCallbackRendererImp* imp;
public:
	WXAMCallbackRenderer();
	virtual ~WXAMCallbackRenderer();
	void MediaType(const GUID& t);
	void Callback(void (*f)(SIZE bmpSize, BYTE* bmpPtr, int bmpLinePitch));
};

}	//	namespace Spr;

#endif
