#include "WXDDOverlaySurface.h"
#include "WXDDraw.h"
#pragma hdrstop

namespace Spr {
using namespace std;
//----------------------------------------------------------------------------
//	WXDDOverlaySurface
//
static const int timeout = 100;

WXDDOverlaySurface::WXDDOverlaySurface(){
}
WXDDOverlaySurface::~WXDDOverlaySurface(){
	Release();
	if (bCallDeleteCallbacks){
		bCallDeleteCallbacks = false;
		deleteCallbacks.Call(this);
	}
}
bool WXDDOverlaySurface::Flip(){
	if (!pDDS) return false;
	AUTOLOCK;
	if (desc.dwBackBufferCount){
		HRESULT hr = pDDS->Flip(NULL, DDFLIP_WAIT);
		return hr == NOERROR;
	}
	return false;
}

HRESULT WINAPI WXDDOverlaySurface::EnumBackBufferCallback(IDirectDrawSurface* pNewDDS, DDSURFACEDESC* pDesc, void* pContext){
//	if (!(pDesc->ddsCaps.dwCaps & DDSCAPS_BACKBUFFER)) return DDENUMRET_OK;
	WXDDOverlaySurface& surf = *(WXDDOverlaySurface*)pContext;
	surf.backBuffers.push_back(pNewDDS);
	return DDENUMRET_OK;
}
bool WXDDOverlaySurface::Create(bool bCallCallback, int nBack){
	AUTOLOCK;
	if (nBack){
		Caps(DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY |
			(desc.ddsCaps.dwCaps & 	~(DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_BACKBUFFER)  ));
		desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
		desc.dwBackBufferCount = nBack;
		backBuffers.clear();
		if (baseclass::Create(bCallCallback)){
			IDirectDrawSurface2* surfCur = pDDS2;
			for(int i=0; i<nBack; i++){
				surfCur->EnumAttachedSurfaces(this, EnumBackBufferCallback);
				surfCur = backBuffers.end()[-1].pDDS2;
			}
			return true;
		}else{
			return false;
		}
	}else{
		Caps(DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY |
			(desc.ddsCaps.dwCaps & 	~(DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_BACKBUFFER)  ));
		return baseclass::Create(bCallCallback);
	}
}
void WXDDOverlaySurface::Release(bool bCallCallback){
}
}	//	namespace Spr
