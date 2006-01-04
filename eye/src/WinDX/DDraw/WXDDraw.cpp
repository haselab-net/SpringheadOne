// DDraw.cpp: WXDDraws クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "WXDDraw.h"
#include <d3d.h>
#if DIRECT3D_VERSION >= 0x800
 #include <d3dx8core.h>
#elif DIRECTDRAW_VERSION >= 0x700
 #include <d3dxcore.h>
#endif
namespace Spr {
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
	assert(last);
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
	assert(primary);
	return *primary;
}
void WXDDraws::HWnd(HWND wnd){
	hWnd = wnd;
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
		"EnumDisplayModes failed.")){
		std::sort(modes.begin(), modes.end());
	}
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
void WXDDraw::HWnd(HWND wnd){
	if (pWXDDraws && !pWXDDraws->hWnd) pWXDDraws->HWnd(wnd);

	HWND hWndParent = wnd;
	//	get mainframe window
	while(hWndParent){
		hWnd = hWndParent;
		hWndParent = GetParent(hWnd);
	}
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
WXDDError& WXDDraw::Error(){
	static WXDDError error;
	return error;
}

//----------------------------------------------------------------------------
//	WXDDError
//
bool WXDDError::Check(DWORD n, const char* msg, const char* fn, int ln)
	{
	if (n==DD_OK) return false;
	if (n==D3D_OK) return false;
	if (n==0) return false;
	if (!DDErrorToString(err, desc, n)) D3DErrorToString(err, desc, n);
	WXDXError::Check(n, msg, fn, ln);
	return true;
	}
bool WXDDError::DDErrorToString(char*& err, char*& desc, HRESULT error)
	{
#ifndef __BORLANDC__
#if DIRECTDRAW_VERSION >= 0x700
	static char buf[4096];
	buf[0]='\0';
	D3DXGetErrorString(error, sizeof(buf), buf);
	if(strlen(buf)){
		err = buf;
		desc = "";
		return true;
	}
#endif
#endif
	err = "";
	desc = "";
    switch(error) 
    	{
        case DDERR_ALREADYINITIALIZED:
            err = "DDERR_ALREADYINITIALIZED";
			desc = "This object is already initialized.";
			return true;
        case DDERR_BLTFASTCANTCLIP:
            err = "DDERR_BLTFASTCANTCLIP";
			desc = "Return if a clipper object is attached to the source surface passed into a BltFast call.";
			return true;
        case DDERR_CANNOTATTACHSURFACE:
            err = "DDERR_CANNOTATTACHSURFACE";
			desc = "This surface can not be attached to the requested surface.";
			return true;
        case DDERR_CANNOTDETACHSURFACE:
            err = "DDERR_CANNOTDETACHSURFACE";
			desc = "This surface can not be detached from the requested surface.";
			return true;
        case DDERR_CANTCREATEDC:
            err = "DDERR_CANTCREATEDC";
			desc = "Windows can not create any more DCs.";
			return true; 
        case DDERR_CANTDUPLICATE:
            err = "DDERR_CANTDUPLICATE";
			desc = "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.";
			return true;
        case DDERR_CLIPPERISUSINGHWND:
            err = "DDERR_CLIPPERISUSINGHWND";
			desc = "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.";
			return true;
        case DDERR_COLORKEYNOTSET:
            err = "DDERR_COLORKEYNOTSET";
			desc = "No src color key specified for this operation.";
			return true;
        case DDERR_CURRENTLYNOTAVAIL:
            err = "DDERR_CURRENTLYNOTAVAIL";
			desc = "Support is currently not available.";
			return true;
        case DDERR_DIRECTDRAWALREADYCREATED:
            err = "DDERR_DIRECTDRAWALREADYCREATED";
			desc = "A DirectDraw object representing this driver has already been created for this process.";
			return true;
        case DDERR_EXCEPTION:
            err = "DDERR_EXCEPTION";
			desc = "An exception was encountered while performing the requested operation.";
			return true;
        case DDERR_EXCLUSIVEMODEALREADYSET:
            err = "DDERR_EXCLUSIVEMODEALREADYSET";
			desc = "An attempt was made to set the cooperative level when it was already set to exclusive.";
			return true;
        case DDERR_GENERIC:
            err = "DDERR_GENERIC";
			desc = "Generic failure.";
			return true;
        case DDERR_HEIGHTALIGN:
            err = "DDERR_HEIGHTALIGN";
			desc = "Height of rectangle provided is not a multiple of reqd alignment.";
			return true;
        case DDERR_HWNDALREADYSET:
            err = "DDERR_HWNDALREADYSET";
			desc = "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.";
			return true;
        case DDERR_HWNDSUBCLASSED:
            err = "DDERR_HWNDSUBCLASSED";
			desc = "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.";
			return true;
        case DDERR_IMPLICITLYCREATED:
            err = "DDERR_IMPLICITLYCREATED";
			desc = "This surface can not be restored because it is an implicitly created surface.";
			return true;
        case DDERR_INCOMPATIBLEPRIMARY:
            err = "DDERR_INCOMPATIBLEPRIMARY";
			desc = "Unable to match primary surface creation request with existing primary surface.";
			return true;
        case DDERR_INVALIDCAPS:
            err = "DDERR_INVALIDCAPS";
			desc = "One or more of the caps bits passed to the callback are incorrect.";
			return true;
        case DDERR_INVALIDCLIPLIST:
            err = "DDERR_INVALIDCLIPLIST";
			desc = "DirectDraw does not support the provided cliplist.";
			return true;
        case DDERR_INVALIDDIRECTDRAWGUID:
            err = "DDERR_INVALIDDIRECTDRAWGUID";
			desc = "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.";
			return true;
        case DDERR_INVALIDMODE:
            err = "DDERR_INVALIDMODE";
			desc = "DirectDraw does not support the requested mode.";
			return true;
        case DDERR_INVALIDOBJECT:
            err = "DDERR_INVALIDOBJECT";
			desc = "DirectDraw received a pointer that was an invalid DIRECTDRAW object.";
			return true;
        case DDERR_INVALIDPARAMS:
            err = "DDERR_INVALIDPARAMS";
			desc = "One or more of the parameters passed to the function are incorrect.";
			return true;
        case DDERR_INVALIDPIXELFORMAT:
            err = "DDERR_INVALIDPIXELFORMAT";
			desc = "The pixel format was invalid as specified.";
			return true;
        case DDERR_INVALIDPOSITION:
            err = "DDERR_INVALIDPOSITION";
			desc = "Returned when the position of the overlay on the destination is no longer legal for that destination.";
			return true;
        case DDERR_INVALIDRECT:
            err = "DDERR_INVALIDRECT";
			desc = "Rectangle provided was invalid.";
			return true;
        case DDERR_LOCKEDSURFACES:
            err = "DDERR_LOCKEDSURFACES";
			desc = "Operation could not be carried out because one or more surfaces are locked.";
			return true;
        case DDERR_NO3D:
            err = "DDERR_NO3D";
			desc = "There is no 3D present.";
			return true;
        case DDERR_NOALPHAHW:
            err = "DDERR_NOALPHAHW";
			desc = "Operation could not be carried out because there is no alpha accleration hardware present or available.";
			return true;
        case DDERR_NOBLTHW:
            err = "DDERR_NOBLTHW";
			desc = "No blitter hardware present.";
			return true;
        case DDERR_NOCLIPLIST:
            err = "DDERR_NOCLIPLIST";
			desc = "No cliplist available.";
			return true;
        case DDERR_NOCLIPPERATTACHED:
            err = "DDERR_NOCLIPPERATTACHED";
			desc = "No clipper object attached to surface object.";
			return true;
        case DDERR_NOCOLORCONVHW:
            err = "DDERR_NOCOLORCONVHW";
			desc = "Operation could not be carried out because there is no color conversion hardware present or available.";
			return true;
        case DDERR_NOCOLORKEY:
            err = "DDERR_NOCOLORKEY";
			desc = "Surface doesn't currently have a color key";
			return true;
        case DDERR_NOCOLORKEYHW:
            err = "DDERR_NOCOLORKEYHW";
			desc = "Operation could not be carried out because there is no hardware support of the destination color key.";
			return true;
        case DDERR_NOCOOPERATIVELEVELSET:
            err = "DDERR_NOCOOPERATIVELEVELSET";
			desc = "Create function called without DirectDraw object method SetCooperativeLevel being called.";
			return true;
        case DDERR_NODC:
            err = "DDERR_NODC";
			desc = "No DC was ever created for this surface.";
			return true;
        case DDERR_NODDROPSHW:
            err = "DDERR_NODDROPSHW";
			desc = "No DirectDraw ROP hardware.";
			return true;
        case DDERR_NODIRECTDRAWHW:
            err = "DDERR_NODIRECTDRAWHW";
			desc = "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.";
			return true;
        case DDERR_NOEMULATION:
            err = "DDERR_NOEMULATION";
			desc = "Software emulation not available.";
			return true;
        case DDERR_NOEXCLUSIVEMODE:
            err = "DDERR_NOEXCLUSIVEMODE";
			desc = "Operation requires the application to have exclusive mode but the application does not have exclusive mode.";
			return true;
        case DDERR_NOFLIPHW:
            err = "DDERR_NOFLIPHW";
			desc = "Flipping visible surfaces is not supported.";
			return true;
        case DDERR_NOGDI:
            err = "DDERR_NOGDI";
			desc = "There is no GDI present.";
			return true;
        case DDERR_NOHWND:
            err = "DDERR_NOHWND";
			desc = "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.";
			return true;
        case DDERR_NOMIRRORHW:
            err = "DDERR_NOMIRRORHW";
			desc = "Operation could not be carried out because there is no hardware present or available.";
			return true;
        case DDERR_NOOVERLAYDEST:
            err = "DDERR_NOOVERLAYDEST";
			desc = "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.";
			return true;
        case DDERR_NOOVERLAYHW:
            err = "DDERR_NOOVERLAYHW";
			desc = "Operation could not be carried out because there is no overlay hardware present or available.";
			return true;
        case DDERR_NOPALETTEATTACHED:
            err = "DDERR_NOPALETTEATTACHED";
			desc = "No palette object attached to this surface.";
			return true;
        case DDERR_NOPALETTEHW:
            err = "DDERR_NOPALETTEHW";
			desc = "No hardware support for 16 or 256 color palettes.";
			return true;
        case DDERR_NORASTEROPHW:
            err = "DDERR_NORASTEROPHW";
			desc = "Operation could not be carried out because there is no appropriate raster op hardware present or available.";
			return true;
        case DDERR_NOROTATIONHW:
            err = "DDERR_NOROTATIONHW";
			desc = "Operation could not be carried out because there is no rotation hardware present or available.";
			return true;
        case DDERR_NOSTRETCHHW:
            err = "DDERR_NOSTRETCHHW";
			desc = "Operation could not be carried out because there is no hardware support for stretching.";
			return true;
        case DDERR_NOT4BITCOLOR:
            err = "DDERR_NOT4BITCOLOR";
			desc = "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.";
			return true;
        case DDERR_NOT4BITCOLORINDEX:
            err = "DDERR_NOT4BITCOLORINDEX";
			desc = "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.";
			return true;
        case DDERR_NOT8BITCOLOR:
            err = "DDERR_NOT8BITCOLOR";
			desc = "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.";
			return true;
        case DDERR_NOTAOVERLAYSURFACE:
            err = "DDERR_NOTAOVERLAYSURFACE";
			desc = "Returned when an overlay member is called for a non-overlay surface.";
			return true;
        case DDERR_NOTEXTUREHW:
            err = "DDERR_NOTEXTUREHW";
			desc = "Operation could not be carried out because there is no texture mapping hardware present or available.";
			return true;
        case DDERR_NOTFLIPPABLE:
            err = "DDERR_NOTFLIPPABLE";
			desc = "An attempt has been made to flip a surface that is not flippable.";
			return true;
        case DDERR_NOTFOUND:
            err = "DDERR_NOTFOUND";
			desc = "Requested item was not found.";
			return true;
        case DDERR_NOTLOCKED:
            err = "DDERR_NOTLOCKED";
			desc = "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.";
			return true;
        case DDERR_NOTPALETTIZED:
            err = "DDERR_NOTPALETTIZED";
			desc = "The surface being used is not a palette-based surface.";
			return true;
        case DDERR_NOVSYNCHW:
            err = "DDERR_NOVSYNCHW";
			desc = "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.";
			return true;
        case DDERR_NOZBUFFERHW:
            err = "DDERR_NOZBUFFERHW";
			desc = "Operation could not be carried out because there is no hardware support for zbuffer blitting.";
			return true;
        case DDERR_NOZOVERLAYHW:
            err = "DDERR_NOZOVERLAYHW";
			desc = "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.";
			return true;
        case DDERR_OUTOFCAPS:
            err = "DDERR_OUTOFCAPS";
			desc = "The hardware needed for the requested operation has already been allocated.";
			return true;
        case DDERR_OUTOFMEMORY:
            err = "DDERR_OUTOFMEMORY";
			desc = "DirectDraw does not have enough memory to perform the operation.";
			return true;
        case DDERR_OUTOFVIDEOMEMORY:
            err = "DDERR_OUTOFVIDEOMEMORY";
			desc = "DirectDraw does not have enough memory to perform the operation.";
			return true;
        case DDERR_OVERLAYCANTCLIP:
            err = "DDERR_OVERLAYCANTCLIP";
			desc = "The hardware does not support clipped overlays.";
			return true;
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            err = "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
			desc = "Can only have ony color key active at one time for overlays.";
			return true;
        case DDERR_OVERLAYNOTVISIBLE:
            err = "DDERR_OVERLAYNOTVISIBLE";
			desc = "Returned when GetOverlayPosition is called on a hidden overlay.";
			return true;
        case DDERR_PALETTEBUSY:
            err = "DDERR_PALETTEBUSY";
			desc = "Access to this palette is being refused because the palette is already locked by another thread.";
			return true;
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            err = "DDERR_PRIMARYSURFACEALREADYEXISTS";
			desc = "This process already has created a primary surface.";
			return true;
        case DDERR_REGIONTOOSMALL:
            err = "DDERR_REGIONTOOSMALL";
			desc = "Region passed to Clipper::GetClipList is too small.";
			return true;
        case DDERR_SURFACEALREADYATTACHED:
            err = "DDERR_SURFACEALREADYATTACHED";
			desc = "This surface is already attached to the surface it is being attached to.";
			return true;
        case DDERR_SURFACEALREADYDEPENDENT:
            err = "DDERR_SURFACEALREADYDEPENDENT";
			desc = "This surface is already a dependency of the surface it is being made a dependency of.";
			return true;
        case DDERR_SURFACEBUSY:
            err = "DDERR_SURFACEBUSY";
			desc = "Access to this surface is being refused because the surface is already locked by another thread.";
			return true;
        case DDERR_SURFACEISOBSCURED:
            err = "DDERR_SURFACEISOBSCURED";
			desc = "Access to surface refused because the surface is obscured.";
			return true;
        case DDERR_SURFACELOST:
            err = "DDERR_SURFACELOST";
			desc = "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.";
			return true;
        case DDERR_SURFACENOTATTACHED:
            err = "DDERR_SURFACENOTATTACHED";
			desc = "The requested surface is not attached.";
			return true;
        case DDERR_TOOBIGHEIGHT:
            err = "DDERR_TOOBIGHEIGHT";
			desc = "Height requested by DirectDraw is too large.";
			return true;
        case DDERR_TOOBIGSIZE:
            err = "DDERR_TOOBIGSIZE";
			desc = "Size requested by DirectDraw is too large, but the individual height and width are OK.";
			return true;
        case DDERR_TOOBIGWIDTH:
            err = "DDERR_TOOBIGWIDTH";
			desc = "Width requested by DirectDraw is too large.";
			return true;
        case DDERR_UNSUPPORTED:
            err = "DDERR_UNSUPPORTED";
			desc = "Action not supported.";
			return true;
        case DDERR_UNSUPPORTEDFORMAT:
            err = "DDERR_UNSUPPORTEDFORMAT";
			desc = "FOURCC format requested is unsupported by DirectDraw.";
			return true;
        case DDERR_UNSUPPORTEDMASK:
            err = "DDERR_UNSUPPORTEDMASK";
			desc = "Bitmask in the pixel format requested is unsupported by DirectDraw.";
			return true;
		case DDERR_UNSUPPORTEDMODE:
			err = "DDERR_UNSUPPORTEDMODE";
			desc = "The display is currently in an unsupported mode.";
			return true;
        case DDERR_VERTICALBLANKINPROGRESS:
            err = "DDERR_VERTICALBLANKINPROGRESS";
			desc = "Vertical blank is in progress.";
			return true;
        case DDERR_WASSTILLDRAWING:
            err = "DDERR_WASSTILLDRAWING";
			desc = "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.";
			return true;
        case DDERR_WRONGMODE:
            err = "DDERR_WRONGMODE";
			desc = "This surface can not be restored because it was created in a different mode.";
			return true;
        case DDERR_XALIGN:
            err = "DDERR_XALIGN";
			desc = "Rectangle provided was not horizontally aligned on required boundary.";
			return true;
        default:
			desc = "dd unknown.";
            return false;
    	}
	}
bool WXDDError::D3DErrorToString(char*& err, char*& desc, HRESULT error)
	{
	err = "";
	desc = "";
    switch(error) 
    	{
        case D3DERR_BADMAJORVERSION:
            err = "D3DERR_BADMAJORVERSION";
			return true;
        case D3DERR_BADMINORVERSION:
			err = "D3DERR_BADMINORVERSION";
			return true;
        case D3DERR_EXECUTE_LOCKED:
			err = "D3DERR_EXECUTE_LOCKED";
			return true;
        case D3DERR_EXECUTE_NOT_LOCKED:
			err = "D3DERR_EXECUTE_NOT_LOCKED";
			return true;
        case D3DERR_EXECUTE_CREATE_FAILED:
			err = "D3DERR_EXECUTE_CREATE_FAILED";
			return true;
        case D3DERR_EXECUTE_DESTROY_FAILED:
			err = "D3DERR_EXECUTE_DESTROY_FAILED";
			return true;
        case D3DERR_EXECUTE_LOCK_FAILED:
			err = "D3DERR_EXECUTE_LOCK_FAILED";
			return true;
        case D3DERR_EXECUTE_UNLOCK_FAILED:
			err = "D3DERR_EXECUTE_UNLOCK_FAILED";
			return true;
        case D3DERR_EXECUTE_FAILED:
			err = "D3DERR_EXECUTE_FAILED";
			return true;
        case D3DERR_EXECUTE_CLIPPED_FAILED:
			err = "D3DERR_EXECUTE_CLIPPED_FAILED";
			return true;
        case D3DERR_TEXTURE_NO_SUPPORT:
			err = "D3DERR_TEXTURE_NO_SUPPORT";
			return true;
        case D3DERR_TEXTURE_NOT_LOCKED:
			err = "D3DERR_TEXTURE_NOT_LOCKED";
			return true;
        case D3DERR_TEXTURE_LOCKED:
			err = "D3DERR_TEXTURELOCKED";
			return true;
        case D3DERR_TEXTURE_CREATE_FAILED:
			err = "D3DERR_TEXTURE_CREATE_FAILED";
			return true;
        case D3DERR_TEXTURE_DESTROY_FAILED:
			err = "D3DERR_TEXTURE_DESTROY_FAILED";
			return true;
        case D3DERR_TEXTURE_LOCK_FAILED:
			err = "D3DERR_TEXTURE_LOCK_FAILED";
			return true;
        case D3DERR_TEXTURE_UNLOCK_FAILED:
			err = "D3DERR_TEXTURE_UNLOCK_FAILED";
			return true;
        case D3DERR_TEXTURE_LOAD_FAILED:
			err = "D3DERR_TEXTURE_LOAD_FAILED";
			return true;
        case D3DERR_MATRIX_CREATE_FAILED:
			err = "D3DERR_MATRIX_CREATE_FAILED";
			return true;
        case D3DERR_MATRIX_DESTROY_FAILED:
			err = "D3DERR_MATRIX_DESTROY_FAILED";
			return true;
        case D3DERR_MATRIX_SETDATA_FAILED:
			err = "D3DERR_MATRIX_SETDATA_FAILED";
			return true;
        case D3DERR_SETVIEWPORTDATA_FAILED:
			err = "D3DERR_SETVIEWPORTDATA_FAILED";
			return true;
        case D3DERR_MATERIAL_CREATE_FAILED:
			err = "D3DERR_MATERIAL_CREATE_FAILED";
			return true;
        case D3DERR_MATERIAL_DESTROY_FAILED:
			err = "D3DERR_MATERIAL_DESTROY_FAILED";
			return true;
        case D3DERR_MATERIAL_SETDATA_FAILED:
			err = "D3DERR_MATERIAL_SETDATA_FAILED";
			return true;
        case D3DERR_LIGHT_SET_FAILED:
			err = "D3DERR_LIGHT_SET_FAILED";
			return true;
        default:
			desc = "d3d unknown.";
            return false;
		}
	}

}	//	namespace Spr
