#include "WXDDSurf.h"
#include "WXDDraw.h"
#pragma hdrstop
namespace Spr {
//----------------------------------------------------------------------------
//	WXDDSurfaceBase
//
void WXDDSurfaceBase::DDraw(WXDDraw& dd)
	{
	if (pWXDD==&dd) return;
	Release();
	pWXDD=&dd;
	}
//----------------------------------------------------------------------------
//	WXDDSurfaceDesc
//
void WXDDSurfaceDesc::Calc()
	{
	bitPerPixel=0;
	bytePerPixel=0;
	pixelPerByte=0;
	if (ddpfPixelFormat.dwFlags & DDPF_RGB)
		{
		bitPerPixel = ddpfPixelFormat.dwRGBBitCount;
		bytePerPixel = (ddpfPixelFormat.dwRGBBitCount+7) / 8;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_YUV)
		{
		bitPerPixel = ddpfPixelFormat.dwYUVBitCount;
		bytePerPixel = (ddpfPixelFormat.dwYUVBitCount+7) / 8;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED1)
		{
		bitPerPixel = 1;
		pixelPerByte = 8;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED2)
		{
		bitPerPixel = 2;
		pixelPerByte = 4;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4)
		{
		bitPerPixel = 4;
		pixelPerByte = 2;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
		{
		bitPerPixel = 8;
		pixelPerByte = 1;
		bytePerPixel = 1;
		}
	else if (ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXEDTO8)
		{
		bitPerPixel = 8;
		pixelPerByte = 1;
		bytePerPixel = 1;
		}
	szLock.cx = rcLock.right - rcLock.left;
	szLock.cy = rcLock.bottom - rcLock.top;
	lineLen = szLock.cx * bytePerPixel;
	if (lineLen == 0 && pixelPerByte) lineLen = (szLock.cx+pixelPerByte-1) / pixelPerByte;
	}

WXDDSurfaceDesc::WXDDSurfaceDesc()
	{
	Clear();
	}
WXDDSurfaceDesc::WXDDSurfaceDesc(const DDSURFACEDESC& desc)
	{
	Clear();
	DDSURFACEDESC* pDesc = this;
	memcpy(pDesc, &desc, sizeof(DDSURFACEDESC));
	Calc();
	}
void WXDDSurfaceDesc::Clear()
	{
	DDSURFACEDESC* pDesc = this;
	memset(pDesc, 0, sizeof(DDSURFACEDESC));
	dwSize = sizeof(DDSURFACEDESC);
	bLock = false;
	bytePerPixel = 0;
	pixelPerByte = 0;
	bitPerPixel = 0;
	lineLen = 0;
	memset(&rcLock, 0, sizeof(rcLock));
	memset(&szLock, 0, sizeof(szLock));
	}
SIZE WXDDSurfaceDesc::Size() const
	{
	SIZE sz;
	sz.cx = dwWidth;
	sz.cy = dwHeight;
	return sz;
	}
void WXDDSurfaceDesc::Size(const SIZE& sz)
	{
	dwWidth = sz.cx;
	dwHeight = sz.cy;
	dwFlags |= DDSD_WIDTH | DDSD_HEIGHT;
	}
void WXDDSurfaceDesc::Caps(DWORD caps)
	{
	ddsCaps.dwCaps = caps;
	dwFlags |= DDSD_CAPS;
	}
DWORD WXDDSurfaceDesc::Caps()
	{
	if (dwFlags & DDSD_CAPS) return ddsCaps.dwCaps;
	return 0;
	}
void WXDDSurfaceDesc::PixelFormat(WXDDSurfaceBase::PixelFormatT t){
	switch(t){
	case WXDDSurfaceBase::RGB555:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddpfPixelFormat.dwRGBBitCount = 16;
		ddpfPixelFormat.dwRBitMask = 0x001F;
		ddpfPixelFormat.dwGBitMask = 0x07C0;
		ddpfPixelFormat.dwBBitMask = 0xF800;
		break;
	case WXDDSurfaceBase::RGB565:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddpfPixelFormat.dwRGBBitCount = 16;
		ddpfPixelFormat.dwRBitMask = 0xF800;
		ddpfPixelFormat.dwGBitMask = 0x07E0;
		ddpfPixelFormat.dwBBitMask = 0x001F;
	case WXDDSurfaceBase::RGB24:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddpfPixelFormat.dwRGBBitCount = 24;
		ddpfPixelFormat.dwRBitMask = 0xFF0000;
		ddpfPixelFormat.dwGBitMask = 0x00FF00;
		ddpfPixelFormat.dwBBitMask = 0x0000FF;
	case WXDDSurfaceBase::RGB32:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddpfPixelFormat.dwRGBBitCount = 32;
		ddpfPixelFormat.dwRBitMask = 0x00FF0000;
		ddpfPixelFormat.dwGBitMask = 0x0000FF00;
		ddpfPixelFormat.dwBBitMask = 0x000000FF;
		break;
	case WXDDSurfaceBase::UYVY:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFourCC = MAKEFOURCC('U', 'Y', 'V', 'Y');
		ddpfPixelFormat.dwFlags = DDPF_FOURCC;
		ddpfPixelFormat.dwYUVBitCount = 16;
		ddpfPixelFormat.dwYBitMask = 0xFF00FF00;
		ddpfPixelFormat.dwUBitMask = 0x000000FF;
		ddpfPixelFormat.dwVBitMask = 0x00FF0000;
		break;
	case WXDDSurfaceBase::VYUY:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFourCC = MAKEFOURCC('V', 'Y', 'U', 'Y');
		ddpfPixelFormat.dwFlags = DDPF_FOURCC;
		ddpfPixelFormat.dwYUVBitCount = 16;
		ddpfPixelFormat.dwYBitMask = 0xFF00FF00;
		ddpfPixelFormat.dwUBitMask = 0x00FF0000;
		ddpfPixelFormat.dwVBitMask = 0x000000FF;
		break;
	case WXDDSurfaceBase::Y221:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y', '2', '2', '1');
		ddpfPixelFormat.dwFlags = DDPF_FOURCC;
		ddpfPixelFormat.dwYUVBitCount = 16;
		ddpfPixelFormat.dwYBitMask = 0x00FF00FF;
		ddpfPixelFormat.dwUBitMask = 0xFF000000;
		ddpfPixelFormat.dwVBitMask = 0x0000FF00;
		break;
	case WXDDSurfaceBase::YUY2:
		dwFlags |= DDSD_PIXELFORMAT;
		ddpfPixelFormat.dwSize = sizeof(ddpfPixelFormat);
		ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y', 'U', 'Y', '2');
		ddpfPixelFormat.dwFlags = DDPF_FOURCC;
		ddpfPixelFormat.dwYUVBitCount = 16;
		ddpfPixelFormat.dwYBitMask = 0x00FF00FF;
		ddpfPixelFormat.dwUBitMask = 0x0000FF00;
		ddpfPixelFormat.dwVBitMask = 0xFF000000;
		break;
	}
}
//----------------------------------------------------------------------------
//	WXDDSInterface
//
WXDDSInterface::WXDDSInterface()
	{
	pDDS = NULL;
	pDDS2 = NULL;
#if DIRECTDRAW_VERSION >= 0x0500
	pDDS3 = NULL;
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	pDDS7 = NULL;
#endif
	}
WXDDSInterface::WXDDSInterface(const WXDDSInterface& cidds){
	pDDS=cidds.pDDS;
	if (pDDS) pDDS->AddRef();
	pDDS2=cidds.pDDS2;
	if (pDDS2) pDDS2->AddRef();
#if DIRECTDRAW_VERSION >= 0x0500
	pDDS3=cidds.pDDS3;
	if (pDDS3) pDDS3->AddRef();
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	pDDS7=cidds.pDDS7;
	if (pDDS3) pDDS7->AddRef();
#endif
}

WXDDSInterface::WXDDSInterface(IDirectDrawSurface* intf){
	pDDS=intf;
	if (pDDS) pDDS->AddRef();
	pDDS->QueryInterface(IID_IDirectDrawSurface2, (void**)&pDDS2);
#if DIRECTDRAW_VERSION >= 0x0500
	pDDS->QueryInterface(IID_IDirectDrawSurface3, (void**)&pDDS3);
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	pDDS->QueryInterface(IID_IDirectDrawSurface7, (void**)&pDDS7);
#endif
}

WXDDSInterface WXDDSInterface::operator =(const WXDDSInterface& cidds)
	{
	Release();
	pDDS=cidds.pDDS;
	if (pDDS) pDDS->AddRef();
	pDDS2=cidds.pDDS2;
	if (pDDS2) pDDS2->AddRef();
#if DIRECTDRAW_VERSION >= 0x0500
	pDDS3=cidds.pDDS3;
	if (pDDS3) pDDS3->AddRef();
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	pDDS7=cidds.pDDS7;
	if (pDDS7) pDDS7->AddRef();
#endif
	return *this;
	}
void WXDDSInterface::Release()
	{
#if DIRECTDRAW_VERSION >= 0x0500
	RELEASE(pDDS3);
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	RELEASE(pDDS7);
#endif
	RELEASE(pDDS2);
	int ref = 0;
	if(pDDS) ref = pDDS->Release();
	TRACEALL("WXDDSInterface::Release %d %x\n", ref, pDDS);
	pDDS=NULL;
	}
WXDDSInterface::~WXDDSInterface()
	{
	Release();
	}


//----------------------------------------------------------------------------
//	WXDDSurfaceBase
//
bool WXDDSurfaceBase::Blt(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags){
	if (sz.cx == -1){
		SIZE szDest = Size();
		szDest.cx -= dest.x;
		szDest.cy -= dest.y;
		SIZE szSrc = surf.Size();
		szSrc.cx -= src.x;
		szSrc.cy -= src.y;
		sz.cx = std::min(szSrc.cx, szDest.cx);
		sz.cy = std::min(szSrc.cy, szDest.cy);
	}
	return BltImp(surf, dest, sz, src, flags);
}
bool WXDDSurfaceBase::Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, POINT ptSrc, SIZE szSrc, DWORD flags, DDOVERLAYFX* fx){
	RECT rcDest, rcSrc;
	RECT* prcSrc = &rcSrc;
	if (szSrc.cx == -1){
		if (ptSrc.x == 0 && ptSrc.y == 0){
			prcSrc = NULL;
		}else{
			szSrc = surf.Size();
			szSrc.cx -= ptSrc.x;
			szSrc.cy -= ptSrc.y;
			rcSrc.left = ptSrc.x;
			rcSrc.top = ptSrc.y;
			rcSrc.right = ptSrc.x + szSrc.cx;
			rcSrc.bottom = ptSrc.y + szSrc.cy;
		}
	}
	if (szDest.cx == -1){
		szDest = Size();
		szDest.cx -= ptDest.x;
		szDest.cy -= ptDest.y;
		rcDest.left = ptDest.x;
		rcDest.top = ptDest.y;
		rcDest.right = ptDest.x + szDest.cx;
		rcDest.bottom = ptDest.y + szDest.cy;
	}
	return OverlayImp(&rcDest, surf, prcSrc, flags, fx);
}
void WXDDSurfaceBase::PixelFormat(WXDDSurfaceBase::PixelFormatT t){
	Desc().PixelFormat(t);
}
//----------------------------------------------------------------------------
//	WXDDSurface
//
WXDDSurface::WXDDSurface()
	{
	pWXDD = NULL;
	hDC = NULL;
	bCallDeleteCallbacks = true;
	}
WXDDSurface::WXDDSurface(IDirectDrawSurface* intf):WXDDSInterface(intf){
	pWXDD = NULL;
	hDC = NULL;
	bCallDeleteCallbacks = true;
}
WXDDSurface::~WXDDSurface()
	{
	Release();
	if (bCallDeleteCallbacks)
		{
		bCallDeleteCallbacks = false;
		deleteCallbacks.Call(this);
		}
	}
bool WXDDSurface::Create(bool bCallCallback)
	{
	AUTOLOCK;
	if (pDDS) return true;
	//	Backup original surface desc.
	descOld = desc;
	//	check ddraw
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->pDDraw && !pWXDD->Create()) goto leave;
	//	Create surface
	if (!WXCHECK(pWXDD->pDDraw->CreateSurface(&desc, &pDDS, NULL)))goto leave;
	pDDS->QueryInterface(IID_IDirectDrawSurface2, (void**)&pDDS2);
#if DIRECTDRAW_VERSION >= 0x0500
	pDDS->QueryInterface(IID_IDirectDrawSurface3, (void**)&pDDS3);
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	pDDS->QueryInterface(IID_IDirectDrawSurface7, (void**)&pDDS7);
#endif
	//	get surface description
	pDDS->GetSurfaceDesc(&desc);
	desc.Calc();
	//	set palette
	if (desc.ddpfPixelFormat.dwFlags&DDPF_PALETTEINDEXED8) 
		{
		if (!pWXDD->CreatePalette()) goto leave;
		if (!WXCHECK(pDDS->SetPalette(pWXDD->pPalette))) goto leave;
		}
	//	call create callbacks
	if (bCallCallback) createCallbacks.Call(this);
	return true;
leave:
	desc = descOld;
	return false;
	}
void WXDDSurface::Release(bool bCallCallback)
	{
	AUTOLOCK;
	if (!pDDS) return;
	ReleaseDC();
	Unlock();
	if (bCallCallback) releaseCallbacks.Call(this);
	desc = descOld;
	WXDDSInterface::Release();
	}

bool WXDDSurface::BltImp(WXDDSurface& surf, POINT dest, SIZE size, POINT src, DWORD flags){
	RECT rcDest = {dest.x, dest.y, dest.x + size.cx, dest.y + size.cy};
	RECT rcSrc = {src.x, src.y, src.x + size.cx, src.y + size.cy};
	AUTOLOCK;
	if (IsLost()) Restore();
	if (!pDDS) return false;
	bool rtv = WXCHECK(pDDS->Blt(&rcDest, surf.pDDS, &rcSrc, flags, NULL));
	return rtv;
}
bool WXDDSurface::StopOverlay(WXDDSurface& surf){
	if (!surf.pDDS || !pDDS) return false;
	if (! (Caps() & DDSCAPS_OVERLAY)) return false;
	return WXCHECK((*this)->UpdateOverlay(NULL, surf, NULL, DDOVER_HIDE, NULL));
}
bool WXDDSurface::OverlayImp(RECT* prcDest, WXDDSurface& surf, RECT* prcSrc, DWORD flags, DDOVERLAYFX* fx){
	if (!surf.pDDS || !pDDS) return false;
	if (! (Caps() & DDSCAPS_OVERLAY)) return false;
	return WXCHECK((*this)->UpdateOverlay(prcSrc, surf, prcDest, flags, fx));
}

bool WXDDSurface::Clear(DWORD color,DWORD flags)
	{
	POINT pt={0,0};
	SIZE sz={0xFFFFFFFF,0xFFFFFFFF};
	return Clear(pt, sz, color, flags);
	}
bool WXDDSurface::ClearImp(POINT dest, SIZE sz, DWORD color, DWORD flags){
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (sz.cx == -1) sz = Size();
	RECT rcDest;
	rcDest.left = dest.x;
	rcDest.top = dest.y;
	rcDest.right = dest.x + sz.cx;
	rcDest.bottom = dest.y + sz.cy;
	DDBLTFX bltFx;
	memset(&bltFx, 0, sizeof(bltFx));
	bltFx.dwSize = sizeof(bltFx);
	bltFx.dwFillColor = color;
	return WXCHECK(pDDS->Blt(&rcDest, NULL, NULL, DDBLT_COLORFILL|flags, &bltFx));
}

bool WXDDSurface::Lock(POINT pt, SIZE sz)
	{
	AUTOLOCK;
	if (desc.bLock) goto leave;
	if (IsLost() && !Restore()) goto leave;
	//	rect to lock
	if (pt.x > (int)desc.dwWidth) desc.rcLock.left = desc.dwWidth;
	else desc.rcLock.left = pt.x;
	if (pt.y > (int)desc.dwHeight) desc.rcLock.top = desc.dwHeight;
	else desc.rcLock.top = pt.y;
	if (sz.cx<0 || pt.x+sz.cx>(int)desc.dwWidth) desc.rcLock.right = desc.dwWidth;
	else desc.rcLock.right = pt.x + sz.cx;
	if (sz.cy<0 || pt.y+sz.cy>(int)desc.dwHeight) desc.rcLock.bottom = desc.dwHeight;
	else desc.rcLock.bottom = pt.y + sz.cy;
//	if (WXCHECK(pDDS->Lock(&desc.rcLock, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL)))
	if (WXCHECK(pDDS->Lock(&desc.rcLock, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
		{
		desc.Calc();
		desc.bLock = true;
		}
leave:

	return desc.bLock;
	}
void WXDDSurface::Unlock()
	{
	if (!pDDS) return;
	if (!desc.bLock) return;
	pDDS->Unlock(desc.lpSurface);
	desc.bLock = false;
	memset(&desc.rcLock, 0, sizeof(desc.rcLock));
	memset(&desc.szLock, 0, sizeof(desc.szLock));
	desc.Calc();

	}
HDC WXDDSurface::GetDC()
	{
	AUTOLOCK;
	if (hDC) return hDC;
	if (IsLost() && !Restore()) return hDC;
	WXCHECK(pDDS->GetDC(&hDC));
	return hDC;
	}
void WXDDSurface::ReleaseDC()
	{
	if (!hDC) return;
	WXCHECK(pDDS->ReleaseDC(hDC));
	hDC = NULL;

	}
bool WXDDSurface::Restore()
	{
	AUTOLOCK;
	bool rtv = false;
	if (!pDDS) return Create();
  	if (pDDS->IsLost() == DDERR_SURFACELOST)
		{
		HRESULT hr = pDDS->Restore();
		if (hr!=DDERR_OUTOFMEMORY && hr!=DDERR_OUTOFVIDEOMEMORY)
			{
			Release();
			rtv = Create();
			goto leave;
			}
		if (!WXCHECKMSG(hr, "restore surface failed.")) goto leave;
		}
	rtv = true;
leave:

	return rtv;
	}
bool WXDDSurface::IsLost()
	{
	AUTOLOCK;
	if (!pDDS || pDDS->IsLost() != DD_OK) return true;

	return false;
	}
SIZE WXDDSurface::Size()
	{
	return desc.Size();
	}
inline int GetMaskLeft(DWORD m)
	{
    int i;
	for(i=0; i<32; i++)
		{
		if (m & 0x80000000) break;
		m <<= 1;
		}
	return 32-i;
	}
inline DWORD MoveToMask(BYTE bCol, DWORD mask)
	{
	DWORD col = bCol;
	int shift = GetMaskLeft(mask)-8;
	if (shift > 0) col <<= shift;
	else col >>= -shift;
	col &= mask;
	return col;
	}
DWORD WXDDSurface::ColorToPixel(COLORREF col)
	{
	DDPIXELFORMAT& fmt = desc.ddpfPixelFormat;
	DWORD rtv = 0;
	if (fmt.dwFlags & DDPF_RGB)
		{
		rtv |= MoveToMask(GetRValue(col), fmt.dwRBitMask);
		rtv	|= MoveToMask(GetGValue(col), fmt.dwGBitMask);
		rtv	|= MoveToMask(GetBValue(col), fmt.dwBBitMask);
		}
	else rtv = col;
	return rtv;
	}


//----------------------------------------------------------------------------
//	WXDDWindowManage
//
WXDDWindowManage::WXDDWindowManage()
	{
	pWndPlaceOld = NULL;
	pMainWndPlaceOld = NULL;
	}
WXDDWindowManage::~WXDDWindowManage()
	{
	delete pWndPlaceOld;
	delete pMainWndPlaceOld;
	}
	
static WNDPROC wprocOld;
static LRESULT CALLBACK WpcMinMaxInfo( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	if (uMsg == WM_GETMINMAXINFO)
		{
		LPMINMAXINFO pmmi = (LPMINMAXINFO) lParam;
		pmmi->ptMaxTrackSize.x = pmmi->ptMaxSize.x * 2;
		pmmi->ptMaxTrackSize.y = pmmi->ptMaxSize.y * 2;
		return 0;
		}
	if (wprocOld) return wprocOld(hwnd, uMsg, wParam, lParam);
	return NULL;
	} 
bool WXDDWindowManage::FitWindow(POINT ptTo, SIZE szTo)
	{
	AUTOLOCK;
	if (!hWnd)
		{
		WXERRMSG(2, "This operation needs hWnd.");
		return false;
		}
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}

	//	set minmaxinfo handler
	if (wprocOld) 
		{
	
		return false;
		}
	wprocOld = (WNDPROC)GetWindowLong(hWndMain, GWL_WNDPROC);
	SetWindowLong(hWndMain, GWL_WNDPROC, (LONG)WpcMinMaxInfo);

	bool rtv = true;
	int i;
	for(i=0; i<4; i++)
		{
		RECT rcClient, rcMain;
		GetWindowRect(hWndMain, &rcMain);
		GetClientRect(hWnd, &rcClient);

		RECT rcScCl;
		POINT pt;
		pt.x = rcClient.left;
		pt.y = rcClient.top;
		::ClientToScreen(hWnd, &pt);
		rcScCl.left		= pt.x;
		rcScCl.top		= pt.y;
		pt.x = rcClient.right;
		pt.y = rcClient.bottom;
		::ClientToScreen(hWnd, &pt);
		rcScCl.right	= pt.x;
		rcScCl.bottom	= pt.y;
		TRACEALL("rcMain (%d,%d) (%d,%d)\n", rcMain.left, rcMain.top, rcMain.right, rcMain.bottom);
		TRACEALL("rcScCl (%d,%d) (%d,%d)\n", rcScCl.left, rcScCl.top, rcScCl.right, rcScCl.bottom);
		
		rcMain.left += ptTo.x - rcScCl.left;
		rcMain.top += ptTo.y - rcScCl.top;
		rcMain.right += szTo.cx - (rcScCl.right - rcScCl.left);
		rcMain.bottom += szTo.cy - (rcScCl.bottom - rcScCl.top);
		MoveWindow(hWndMain, rcMain.left, rcMain.top, rcMain.right-rcMain.left, rcMain.bottom-rcMain.top, false);
		if (rcScCl.left == ptTo.x &&
			rcScCl.top == ptTo.y &&
			rcScCl.right == ptTo.x+szTo.cx &&
			rcScCl.bottom == ptTo.y+szTo.cy) break;
		}	
	if (i==4) rtv = false;
	//	restore handler
	SetWindowLong(hWndMain, GWL_WNDPROC, (LONG)wprocOld);
	wprocOld = NULL;

	InvalidateRect(hWndMain, NULL, false);
	return rtv;
	}
bool WXDDWindowManage::RestoreWindowPlace()
	{
	if (!hWnd || !pWndPlaceOld || !pMainWndPlaceOld) return false;
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}
	SetWindowPlacement(hWnd, pWndPlaceOld);
	SetWindowPlacement(hWndMain, pMainWndPlaceOld);
	delete pWndPlaceOld;
	pWndPlaceOld = NULL;
	delete pMainWndPlaceOld;
	pMainWndPlaceOld = NULL;
	return true;
	}
void WXDDWindowManage::SaveWindowPlace()
	{
	if (!hWnd) return;
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}
	if (!pWndPlaceOld) pWndPlaceOld = new WINDOWPLACEMENT;
	pWndPlaceOld->length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd, pWndPlaceOld);
	if (!pMainWndPlaceOld) pMainWndPlaceOld = new WINDOWPLACEMENT;
	pMainWndPlaceOld->length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWndMain, pMainWndPlaceOld);
	}
void WXDDWindowManage::ClientToScreen(POINT* pt, SIZE* sz)
	{
	POINT ptClient = *pt;
	::ClientToScreen(hWnd, pt);
	RECT rc;
	::GetClientRect(hWnd, &rc);
	SIZE szMax;
	szMax.cx = rc.right - rc.left - ptClient.x;
	szMax.cy = rc.bottom - rc.top - ptClient.y;
	if (szMax.cx < 0) szMax.cx = 0;
	if (szMax.cy < 0) szMax.cy = 0;
	if (sz->cx < 0 || sz->cx > szMax.cx) sz->cx = szMax.cx;
	if (sz->cy < 0 || sz->cy > szMax.cy) sz->cy = szMax.cy;
	}
SIZE WXDDWindowManage::Size()
	{
	SIZE sz;
	RECT rc;
	::GetClientRect(hWnd, &rc);
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	return sz;
	}
//----------------------------------------------------------------------------
//	WXDDFrontSurface
//
WXDDFrontSurface::WXDDFrontSurface()
	{
	pWXDD = NULL;
	pClipper = NULL;
	}
WXDDFrontSurface::~WXDDFrontSurface()
	{
	Release();
	}
void WXDDFrontSurface::ClientToScreen(POINT* pt, SIZE* sz)
	{
	if (pWXDD->IsFullscreen()) return;
	WXDDWindowManage::ClientToScreen(pt, sz);
	}

bool WXDDFrontSurface::Create()
	{
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	pWXDD->surfPrimary.Create();
	if (!pClipper && !WXCHECK(pWXDD->pDDraw->CreateClipper(0, &pClipper, NULL)))
		{
	
		return false;
		}
	if (hWnd) pClipper->SetHWnd(0, hWnd);

	return true;
	}
void WXDDFrontSurface::Release()
	{
	if (!pClipper) return;
	RELEASE(pClipper);
	if (pWXDD->IsFullscreen()) pWXDD->surfPrimary.Release();
	}
bool WXDDFrontSurface::IsLost()
	{
	return !pClipper || !pWXDD || pWXDD->surfPrimary.IsLost();
	}
bool WXDDFrontSurface::Restore()
	{
	AUTOLOCK;
	if (!pClipper) return Create();
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	bool rtv = pWXDD->surfPrimary.Restore();

	return rtv;
	}
bool WXDDFrontSurface::SetClipper()
	{
	AUTOLOCK;
	bool rtv = false;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->surfPrimary.Restore()) goto leave;
	if (!pWXDD->IsFullscreen())
		{
		if (!pClipper) goto leave;
		rtv = WXCHECK(pWXDD->surfPrimary.pDDS->SetClipper(pClipper));
		}
	else
		{
		if (!pWXDD->pClipper) goto leave;
		rtv = WXCHECK(pWXDD->surfPrimary.pDDS->SetClipper(pWXDD->pClipper));
		}
leave:

	return rtv;
	}
bool WXDDFrontSurface::BltImp(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags){
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (!SetClipper()) return false;
	if (sz.cx < 0){
		sz = Size();
		SIZE szSrc = surf.Size();
		sz.cx = std::min(sz.cx, szSrc.cx);
		sz.cy = std::min(sz.cy, szSrc.cy);
	}
	ClientToScreen(&dest, &sz);
	bool rtv = pWXDD->surfPrimary.Blt(surf, dest, sz, src, flags);

	return rtv;
}
bool WXDDFrontSurface::StopOverlay(WXDDSurface& surf){
	return pWXDD->surfPrimary.StopOverlay(surf);
}
bool WXDDFrontSurface::OverlayImp(RECT* prcDest, WXDDSurface& surf, RECT* prcSrc, DWORD flags, DDOVERLAYFX* fx){
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (!SetClipper()) return false;
	POINT dest = {prcDest->left, prcDest->top};
	SIZE szDest = {prcDest->right - prcDest->left, prcDest->bottom - prcDest->top};
	ClientToScreen(&dest, &szDest);
	prcDest->left = dest.x;
	prcDest->top = dest.y;
	prcDest->right = dest.x + szDest.cx;
	prcDest->bottom = dest.y + szDest.cy;
	return  pWXDD->surfPrimary.OverlayImp(prcDest, surf, prcSrc, flags, fx);
}
bool WXDDFrontSurface::Clear(DWORD color, DWORD flags){
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (!SetClipper()) return false;
	bool rtv = pWXDD->surfPrimary.Clear(color, flags);
	return rtv;
}
bool WXDDFrontSurface::Clear(POINT pt, SIZE sz, DWORD color, DWORD flags)
	{
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (!SetClipper())
		{
	
		return false;
		}
	ClientToScreen(&pt, &sz);
	bool rtv = pWXDD->surfPrimary.Clear(pt, sz, color, flags);

	return rtv;
	}
bool WXDDFrontSurface::Lock(POINT pt, SIZE sz)
	{
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (!SetClipper())
		{
	
		return false;
		}
	ClientToScreen(&pt, &sz);
	bool rtv = pWXDD->surfPrimary.Lock(pt, sz);

	return rtv;
	}
void WXDDFrontSurface::Unlock()
	{
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	pWXDD->surfPrimary.Unlock();

	}
HDC WXDDFrontSurface::GetDC()
	{
	AUTOLOCK;
	if (IsLost() && !Restore()) return NULL;
	if (!SetClipper())
		{
		
		return NULL;
		}
	HDC hdc = pWXDD->surfPrimary.GetDC();

	return hdc;
	}
void WXDDFrontSurface::ReleaseDC()
	{
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	pWXDD->surfPrimary.ReleaseDC();

	}
bool WXDDFrontSurface::FitWindowToSurface()
	{
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	POINT pt={0,0};
	SIZE sz={pWXDD->surfPrimary.desc.dwWidth, pWXDD->surfPrimary.desc.dwHeight};
	bool rtv = FitWindow(pt, sz);

	return rtv;
	}
DWORD WXDDFrontSurface::ColorToPixel(COLORREF col)
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.ColorToPixel(col);
	}
DWORD WXDDFrontSurface::PixelPerByte()
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.PixelPerByte();
	}
DWORD WXDDFrontSurface::BytePerPixel()
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.BytePerPixel();
	}
DWORD WXDDFrontSurface::LineLen()
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.LineLen();
	}
BYTE* WXDDFrontSurface::Line(DWORD y)
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.Line(y);
	}
DWORD WXDDFrontSurface::NLine()
	{
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	return pWXDD->surfPrimary.NLine();
	}
WXDDSurfaceDesc& WXDDFrontSurface::Desc(){
	return pWXDD->surfPrimary.desc; 
}
//----------------------------------------------------------------------------
//	WXDDSInterfaces
//
WXDDSInterfaces::WXDDSInterfaces()
	{
	cur = 0;
	}
void WXDDSInterfaces::Cur(int c)
	{
	if (c >= (int)size()) c = 0;
	cur = c;
	}
}	//	namespace Spr
