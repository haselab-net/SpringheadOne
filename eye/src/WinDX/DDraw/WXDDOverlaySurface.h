#ifndef Spr_DDRAW_WXDDOVERLAYSURF_H
#define Spr_DDRAW_WXDDOVERLAYSURF_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WXDDSurf.h"

namespace Spr {

//----------------------------------------------------------------------------
//	WXDDOverlaySurface

/**	A flippable surface which has both back buffer and front buffer.
	If you need DirectDraw and don't need Direct3D, use this class
	as a flippable surface.	*/
class WXDDOverlaySurface:public WXDDSurface{
public:
	typedef WXDDSurface baseclass;
	std::vector<WXDDSurface> backBuffers;
	WXDDOverlaySurface();
	~WXDDOverlaySurface();
	//	描画バッファと表示バッファの入れ替え
	virtual bool Flip();
	
	bool Create(int nBack = 0){return Create(true, nBack);}
	void Release(){Release(true);}
protected:
	static HRESULT WINAPI EnumBackBufferCallback(IDirectDrawSurface* pDDS, DDSURFACEDESC* pDesc, void* pContext);	//	バックバッファの列挙
	void ReleaseCallback(WXDDSurface* surf);
	virtual bool Create(bool bCallCallback, int nBack=0);
	virtual void Release(bool bCallCallback);
};

}	//	namespace Spr

#endif