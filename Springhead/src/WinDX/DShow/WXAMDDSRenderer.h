#ifndef Spr_DSHOW_WXAMDDSRENDER_H
#define Spr_DSHOW_WXAMDDSRENDER_H
#include <WinDX/Base/WXComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterBase.h"

namespace Spr {;

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


}	//	namespace Spr;

#endif
