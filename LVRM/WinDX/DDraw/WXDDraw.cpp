// DDraw.cpp: WXDDraws クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "WXDDraw.h"
#include <d3d.h>
#if DIRECTDRAW_VERSION >= 0x700
 #include <d3dxcore.h>
#endif
namespace LVRM {
//----------------------------------------------------------------------------
//		WXDDraws
//
WXDDraws* WXDDraws::last;
#ifdef USE_CRITICAL_SECTION
 static CCriticalSection* WXDDraws_critical;
 static int ddrawCriticalRefCount;
 static void DDrawCriticalAddRef(){
	ddrawCriticalRefCount ++;
	TRACEALL("WXDDraws's critical section ref count = ^%d.\n", ddrawCriticalRefCount);
 }
 static void DDrawCriticalRelease(){
	ddrawCriticalRefCount --;
	TRACEALL("WXDDraws's critical section ref count = _%d.\n", ddrawCriticalRefCount);
	if (ddrawCriticalRefCount == 0){
		delete WXDDraws_critical;
		WXDDraws_critical = NULL;
	}
 }
 CCriticalSection& WXDDraws::Critical(){
	if (!WXDDraws_critical){
		TRACEALL("Create critical section.\n");
		WXDDraws_critical = new CCriticalSection;
	}
	return *WXDDraws_critical;
 }
#else
 #define DDrawCriticalAddRef()
 #define DDrawCriticalRelease()
#endif

WXDDraws* WXDDraws::GetLastCreate(){
	VRASSERT(last);
	return last;
}
WXDDraws::WXDDraws(){
	hWnd = NULL;
	Create();
	DDrawCriticalAddRef();
}
WXDDraws::~WXDDraws(){	
	DDrawCriticalRelease();
}

#if DIRECTDRAW_VERSION >= 0x700
BOOL WINAPI WXDDraws::DDEnumCallbackEx(GUID* pGUID, char* pDesc, char* pName, void* pContext, HMONITOR hm){
#else
BOOL WINAPI WXDDraws::DDEnumCallbackEx(GUID* pGUID, char* pDesc, char* pName, void* pContext, HANDLE hm){
#endif
	WXDDraws* pWXDD = (WXDDraws*)pContext;
	pWXDD->push_back(WXDDraw());
	WXDDraw& newDev = pWXDD->end()[-1];
	if (pGUID){
		newDev.pGuid = new GUID;
		*newDev.pGuid = *pGUID;
	}
	newDev.name = pName;
	newDev.desc = pDesc;
	newDev.pWXDDraws = pWXDD;
	return DDENUMRET_OK;
}
BOOL WINAPI WXDDraws::DDEnumCallback(GUID* pGUID, char* pDesc, char* pName, void* pContext){
	return DDEnumCallbackEx(pGUID, pDesc, pName, pContext, NULL);
}
bool WXDDraws::Create(){
	//	following is to live together old version of direct draw library
#if DIRECTDRAW_VERSION >= 0x0600
	DirectDrawEnumerateEx(DDEnumCallbackEx, this, DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_NONDISPLAYDEVICES);
#else
	DirectDrawEnumerate(DDEnumCallback, this);
#endif
	for(int i=0; i<(int)size(); i++) {
		if (begin()[i].pGuid == NULL) primary = &begin()[i];
	}
	lastOld = last;
	last = this;
	return true;
}
WXDDraw& WXDDraws::Primary(){
	VRASSERT(primary);
	return *primary;
}
void WXDDraws::HWnd(HWND wnd){
	HWND hWndParent = wnd;
	//	get mainframe window
	while(hWndParent){
		hWnd = hWndParent;
		hWndParent = GetParent(hWnd);
	}
}

//----------------------------------------------------------------------------
//	WXDDraw
//
WXDDraw::WXDDraw(){
	DDrawCriticalAddRef();
	pWXDDraws = NULL;
	hWnd = NULL;
	pDDraw = NULL;
	pDDraw2 = NULL;
	pD3D = NULL;
	pPalette = NULL;
	pClipper = NULL;
	pGuid = NULL;
	bFullscreen = false;
	surfPrimary.DDraw(*this);
#if DIRECTDRAW_VERSION >= 0x0700
	pD3D7 = NULL;
	pDDraw7 = NULL;
#endif
}
WXDDraw::WXDDraw(const WXDDraw& dd){
	DDrawCriticalAddRef();
	pWXDDraws = dd.pWXDDraws;
	hWnd = dd.hWnd;
	bFullscreen = false;
	wplaceOld = dd.wplaceOld;
	pDDraw = dd.pDDraw;
	if (pDDraw) pDDraw->AddRef();
	pDDraw2 = dd.pDDraw2;
	if (pDDraw2) pDDraw2->AddRef();
	pPalette = dd.pPalette;
	if (pPalette) pPalette->AddRef();
	pClipper = dd.pClipper;
	if (pClipper) pClipper->AddRef();
	pD3D = dd.pD3D;
	if (pD3D) pD3D->AddRef();
	name = dd.name;
	desc = dd.desc;
	if (dd.pGuid){
		pGuid = new GUID;
		*pGuid = *dd.pGuid;
	}
	else pGuid = NULL;
	displayMode = dd.displayMode;
	surfPrimary.DDraw(*this);
#if DIRECTDRAW_VERSION >= 0x0700
	pD3D7 = dd.pD3D7;
	if (pD3D7) pD3D7->AddRef();
	pDDraw7 = dd.pDDraw7;
	if (pDDraw7) pDDraw7->AddRef();
#endif
}

WXDDraw::~WXDDraw(){
	surfPrimary.Release();
	Release();
	delete pGuid;
	pGuid = NULL;
	DDrawCriticalRelease();
}

static HRESULT CALLBACK EnumDisplayModesCallback(DDSURFACEDESC* pddsd, void* lpContext){
	WXDDraw* pWXDD = (WXDDraw*)lpContext;
	WXDDDisplayMode newMode;
    newMode.desc = *pddsd;
	newMode.desc.Calc();
	pWXDD->modes.push_back(newMode);
	return DDENUMRET_OK;
}
bool WXDDraw::Create3D(){
	if (!Create()) return false;
	bool rv = true;
#if DIRECTDRAW_VERSION >= 0x0700
	if (!pD3D7 && pDDraw7){
		rv &= WXCHECK(pDDraw7->QueryInterface(IID_IDirect3D7, (void**)&pD3D7));
	}
#endif
	if (!pD3D){
#if DIRECTDRAW_VERSION >= 0x500
		rv &= WXCHECK(pDDraw2->QueryInterface(IID_IDirect3D2, (void**)&pD3D));
#else
		rv &= WXCHECK(pDDraw2->QueryInterface(IID_IDirect3D, (void**)&pD3D));
#endif
	}
	return rv;
}
bool WXDDraw::Create()
	{
	AUTOLOCK;
	if (pDDraw2) return true;
	//	create direct draw
	pDDraw = NULL;
	if (!WXCHECK(DirectDrawCreate(pGuid, &pDDraw, NULL))) return false;
    pDDraw2 = NULL;
	WXCHECK(pDDraw->QueryInterface(IID_IDirectDraw2, (void**)&pDDraw2));
	WXCHECK(pDDraw2->SetCooperativeLevel(NULL, DDSCL_NORMAL));
#if DIRECTDRAW_VERSION >= 0x700
	if (!WXCHECK(DirectDrawCreateEx(pGuid, (void**)&pDDraw7, IID_IDirectDraw7, NULL))) return false;
	WXCHECK(pDDraw7->SetCooperativeLevel(NULL, DDSCL_NORMAL | DDSCL_FPUSETUP));
#endif

	//	enum display modes
	modes.clear();
	if (WXCHECKMSG(pDDraw2->EnumDisplayModes(/*DDEDM_REFRESHRATES*/ 0, NULL, this, EnumDisplayModesCallback),
		"EnumDisplayModes failed."))
		std::sort(modes.begin(), modes.end());
	//	get hWnd from ddraws
	if (!hWnd) HWnd(WXDDraws::GetLastCreate()->hWnd);

	//	set primary surface's caps
	surfPrimary.Caps(DDSCAPS_PRIMARYSURFACE);

	//	get current display mode
	GetDisplayMode();
	//	create clipper
	if (pDDraw2 && !pClipper)
		WXCHECK(pDDraw2->CreateClipper(0, &pClipper, NULL));
	return true;
	}
void WXDDraw::Release()
	{
	AUTOLOCK;
	ReleasePalette();
	RELEASE(pClipper);
	RELEASE(pD3D);
	RELEASE(pDDraw);
	RELEASE(pDDraw2);
#if DIRECTDRAW_VERSION >= 0x0700
	RELEASE(pD3D7);
	RELEASE(pDDraw7);
#endif
	}
void WXDDraw::HWnd(HWND wnd)
	{
	hWnd = wnd;
	}
bool WXDDraw::Fullscreen(int w, int h, int bitPerPixel, int refresh)
	{
	AUTOLOCK;
	if (!pDDraw2 && !Create()) goto leave;
	if (!hWnd && !pWXDDraws->HWnd())
		{
		WXERRMSG(2, "WXDDraw::Fullscreen() needs handle of the top level window. Call WXDDraws::HWnd(HWND).");
		goto leave;
		}
#ifndef DDSCL_SETFOCUSWINDOW			//	The version of the Direct Draw is old and it can't make device window.
	if (!hWnd) hWnd = pWXDDraws->HWnd();	//	Use the forcus window insted of the device window.
#endif
	if (!bFullscreen)
		{
		wplaceOld.length = sizeof (wplaceOld);
		if (hWnd) GetWindowPlacement(hWnd, &wplaceOld);
		}
	//	release primary surface and palette
	surfPrimary.Release();
	surfPrimary.desc.dwFlags = 0;
	surfPrimary.Caps(DDSCAPS_PRIMARYSURFACE);
	ReleasePalette();
	TRACEALL("SetCooperativeLevel\n");
#ifdef DDSCL_SETFOCUSWINDOW
	if (!WXCHECK(pDDraw2->SetCooperativeLevel(pWXDDraws->hWnd, DDSCL_SETFOCUSWINDOW))) goto leave;
	if (hWnd){
		if (!WXCHECK(pDDraw2->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_SETDEVICEWINDOW))) goto leave;
	}else{
		if (!WXCHECK(pDDraw2->SetCooperativeLevel(NULL, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_CREATEDEVICEWINDOW))) goto leave;
	}
#else
	if (!WXCHECK(pDDraw2->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN))) goto leave;
#endif
	if (!WXCHECK(pDDraw2->SetDisplayMode(w, h, bitPerPixel, refresh, 0))) goto coopleave;
 	GetDisplayMode();
	bFullscreen = true;
	return true;
coopleave:
	WXCHECK(pDDraw2->RestoreDisplayMode());
	WXCHECK(pDDraw2->SetCooperativeLevel(hWnd, DDSCL_NORMAL));
	if (hWnd) SetWindowPlacement(hWnd, &wplaceOld);
leave:
	return false;
}
bool WXDDraw::Fullscreen(const WXDDDisplayMode& m){
	return Fullscreen(m.desc.Size().cx, m.desc.Size().cy, m.desc.bitPerPixel);
}
bool WXDDraw::Window()
	{
	AUTOLOCK;
	if (!bFullscreen) return true;
	if (!pDDraw2 && !Create()) goto leave;
	//	release primary surface and palette
	surfPrimary.Release();
	surfPrimary.desc.dwFlags = 0;
	surfPrimary.Caps(DDSCAPS_PRIMARYSURFACE);
	ReleasePalette();
	if (!WXCHECK(pDDraw2->RestoreDisplayMode())) goto leave;
	if (!WXCHECK(pDDraw2->SetCooperativeLevel(hWnd, DDSCL_NORMAL))) goto leave;
 	GetDisplayMode();
	bFullscreen = false;
	if(hWnd) SetWindowPlacement(hWnd, &wplaceOld);
	return true;
leave:
	return false;
	}
DWORD WXDDraw::BPPToDDBD(int bpp)
	{
    switch(bpp) 
		{
        case 1:
            return DDBD_1;
        case 2:
            return DDBD_2;
        case 4:
            return DDBD_4;
        case 8:
            return DDBD_8;
        case 16:
            return DDBD_16;
        case 24:
            return DDBD_24;
        case 32:
            return DDBD_32;
        default:
            return 0;
		}
	}
DWORD WXDDraw::FindBPP(DWORD ddbd, int bpp)
	{
	int bppFound=0;
	if (ddbd & DDBD_32)
		{
		bppFound = 32;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_24)
		{
		bppFound = 24;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_16)
		{
		bppFound = 16;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_8)
		{
		bppFound = 8;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_4)
		{
		bppFound = 4;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_2)
		{
		bppFound = 2;
		if (bppFound <= bpp) return bppFound;
		}
	if (ddbd & DDBD_1)
		{
		bppFound = 1;
		if (bppFound <= bpp) return bppFound;
		}
	return bppFound;
	}
void WXDDraw::GetDisplayMode()
	{
	if (!pDDraw2 && !Create()) return;
	WXCHECK(pDDraw2->GetDisplayMode(&displayMode));
	SetClipper();
	}
void WXDDraw::SetClipper()
	{
	if (!pClipper) return;
	char buf[sizeof(RGNDATA) + sizeof(RECT)];
	RGNDATA* rgn = (RGNDATA*) buf;
	rgn->rdh.dwSize = sizeof(RGNDATAHEADER);
	rgn->rdh.iType = RDH_RECTANGLES;
	rgn->rdh.nCount = 1;
	rgn->rdh.nRgnSize = 1;
	rgn->rdh.rcBound.left = 0;
	rgn->rdh.rcBound.top = 0;
	SIZE sz = displayMode.Size();
	rgn->rdh.rcBound.right = sz.cx;
	rgn->rdh.rcBound.bottom = sz.cy;
	RECT* rgnRect = (RECT*) &rgn->Buffer;
	*rgnRect = rgn->rdh.rcBound;
	pClipper->SetClipList(rgn, 0);
	}
bool WXDDraw::CreatePalette()
	{
	if (pPalette) return true;
	surfPrimary.Create();
	//	Get system palette entries
	PALETTEENTRY ppe[256];
	HDC hdc = GetDC(NULL);
	GetSystemPaletteEntries(hdc, 0, 256, ppe);
	ReleaseDC(NULL, hdc);
	//	Set palette flag
	if (bFullscreen)
		{
		//	We can use all entris.
		ppe[0].peFlags = PC_EXPLICIT;
		for (int i = 1; i < 255; i++) ppe[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
		ppe[255].peFlags = PC_EXPLICIT;
		}
	else
		{
		//	We can use 10 to 245 entries.
		int i;
		for (i = 0; i < 10; i++) ppe[i].peFlags = PC_EXPLICIT;
		for (i = 10; i < 256 - 10; i++) ppe[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
		for (i = 256 - 10; i < 256; i++) ppe[i].peFlags = PC_EXPLICIT;
		}
	bool rtv = WXCHECK(pDDraw2->CreatePalette(DDPCAPS_8BIT|DDPCAPS_INITIALIZE, ppe, &pPalette, NULL));
	return rtv;
	}
void WXDDraw::ReleasePalette()
	{
	RELEASE(pPalette);
	}
}	//	namespace LVRM
