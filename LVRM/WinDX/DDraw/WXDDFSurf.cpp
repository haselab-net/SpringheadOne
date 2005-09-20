#include "WXDDSurf.h"
#include "WXDDFSurf.h"
#include "WXDDraw.h"
#pragma hdrstop

namespace LVRM {
using namespace std;
//----------------------------------------------------------------------------
//	WXDDFlipSurface
//
HRESULT WINAPI WXDDFlipSurface::EnumBackBufferCallback(IDirectDrawSurface* pDDS, DDSURFACEDESC* pDesc, void* pContext)
	{
	if (!(pDesc->ddsCaps.dwCaps & DDSCAPS_BACKBUFFER))
		return DDENUMRET_OK;
	WXDDFlipSurface& surf = *(WXDDFlipSurface*)pContext;
	surf.pDDS = pDDS;
	surf.pDDS->QueryInterface(IID_IDirectDrawSurface2, (void**)&surf.pDDS2);
#if DIRECTDRAW_VERSION >= 0x0500
	surf.pDDS->QueryInterface(IID_IDirectDrawSurface3, (void**)&surf.pDDS3);
#endif
	return DDENUMRET_OK;
	}
static const int timeout = 100;

WXDDFlipSurface::WXDDFlipSurface()
	{
	bFlippable = false;
	}
WXDDFlipSurface::~WXDDFlipSurface()
	{
	Release();
	if (bCallDeleteCallbacks)
		{
		bCallDeleteCallbacks = false;
		deleteCallbacks.Call(this);
		}
	}
bool WXDDFlipSurface::Flip()
	{
	if (!pDDS) return false;
	AUTOLOCK;
	bool rtv;
	if (bFlippable)
		{
		rtv = !pWXDD->CKDDERR(pWXDD->surfPrimary.pDDS->Flip(NULL, DDFLIP_WAIT));
		}
	else rtv = front.Blt(*this);
	return rtv;
	}
bool WXDDFlipSurface::Create(bool bCallCallback, int nBack)
	{
	AUTOLOCK;
	if (pDDS) return true;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->Create()) goto leave;
	if (pWXDD->IsFullscreen())
		{
		WXDDSurface& surfPri = pWXDD->surfPrimary;
		if (surfPri.pDDS)
			{
			pWXDD->DDERRMSG(1, "Another surface already uses the primary surface.");
			goto leave;
			}
		//	create complex surface
		surfPri.Caps(DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX |
			(desc.ddsCaps.dwCaps & 	~(DDSCAPS_OFFSCREENPLAIN | DDSCAPS_BACKBUFFER)  ));
		surfPri.desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
		surfPri.desc.dwBackBufferCount = nBack;
		if (!surfPri.Create()) goto leave;
		surfPri.pDDS->EnumAttachedSurfaces(this, EnumBackBufferCallback);
		//	get desc
		descOld = desc;
		pDDS->GetSurfaceDesc(&desc);
		desc.Calc();
		//	set palette
		if (desc.ddpfPixelFormat.dwFlags&DDPF_PALETTEINDEXED8) 
			{
			if (!pWXDD->CreatePalette()) goto leave;
			if (pWXDD->CKDDERR(pDDS->SetPalette(pWXDD->pPalette))) goto leave;
			}
		bFlippable = true;
		if (bCallCallback) createCallbacks.Call(this);
		}
	else	//	we are not in the fullscreen mode.
		{
		bFlippable = false;
		if (!pWXDD->surfPrimary.pDDS)
			{
			pWXDD->surfPrimary.desc.dwFlags = 0;
			pWXDD->surfPrimary.Caps(DDSCAPS_PRIMARYSURFACE);
			if (!pWXDD->surfPrimary.Create()) goto leave;
			}
		if (!front.Create()) goto leave;
		Caps(DDSCAPS_OFFSCREENPLAIN | (desc.ddsCaps.dwCaps & 
			~(DDSCAPS_PRIMARYSURFACE | DDSCAPS_BACKBUFFER)  ));
		SIZE sz = front.Size();
		if (sz.cx < 8) sz.cx = 8;
		if (sz.cy < 8) sz.cy = 8;
		WXDDSurface::Size(sz);
		if (!WXDDSurface::Create(bCallCallback)) goto leave;
		}
	pWXDD->surfPrimary.releaseCallbacks.Add(new WXCallback<WXDDSurface, WXDDFlipSurface>(this, &WXDDFlipSurface::ReleaseCallback));
	return true;
leave:
	Release();
	return false;
	}
void WXDDFlipSurface::Release(bool bCallCallback)
	{
	AUTOLOCK
	if (!pDDS) return;
	if (!pWXDD) return;
	pWXDD->surfPrimary.releaseCallbacks.Del(WXCallback<WXDDSurface, WXDDFlipSurface>(this, &WXDDFlipSurface::ReleaseCallback));
	if (desc.ddsCaps.dwCaps & DDSCAPS_BACKBUFFER)
		{
		WXDDSurface::Release(bCallCallback);
		pWXDD->surfPrimary.Release();
		}
	else WXDDSurface::Release(bCallCallback);
	front.Release();
	}
bool WXDDFlipSurface::FitSurface()
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (pWXDD->IsFullscreen()) return true;
	SIZE sz = Size();
	SIZE fsz = front.Size();
	if ((fsz.cx<8 || fsz.cy<8) && (sz.cx>=fsz.cx && sz.cy>=fsz.cy)) return true;
	if (sz.cx == fsz.cx && sz.cy == fsz.cy) return true;
	Release();
	return Create();
	}
bool WXDDFlipSurface::Restore()
	{
	AUTOLOCK;
	TRACEALL("WXDDFlipSurface::Restore\n");
	bool rtv = false;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (pWXDD->surfPrimary.pDDS)
		{
		if (desc.ddsCaps.dwCaps & DDSCAPS_BACKBUFFER)
			rtv = front.Restore();
		else
			{
			rtv = front.Restore();
			if (rtv) rtv = WXDDSurface::Restore();
			}
		}
	if (!rtv)
		{
		Release();
		rtv=Create();
		}
	return rtv;
	}
void WXDDFlipSurface::ReleaseCallback(WXDDSurface* pri){
	Release();
}

}	//	namespace LVRM
