#ifndef LVRM_DSHOW_WXAMDDSRENDER_H
#define LVRM_DSHOW_WXAMDDSRENDER_H
#include <WinDX/Base/WxComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace LVRM {;

extern const GUID CLSID_AMDDSRender;

class WXAMDDSRendererImp;
class WXAMDDSRenderer:public WXAMFilterBase{
protected:
	WXAMDDSRendererImp* imp;
public:
	WXAMDDSRenderer();
	virtual ~WXAMDDSRenderer();
	void Surf(WXDDSurfaceBase& surf);
	void MediaType(const GUID& t);
};


}	//	namespace LVRM;

#endif
