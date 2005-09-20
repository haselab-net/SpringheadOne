#ifndef LVRM_DSHOW_WXAMCALLBACKRENDER_H
#define LVRM_DSHOW_WXAMCALLBACKRENDER_H
#include <WinDX/Base/WxComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace LVRM {;

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

}	//	namespace LVRM;

#endif
