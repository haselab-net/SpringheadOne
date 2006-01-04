#include "WXD3Dev.h"
#include "../DDraw/WXDDMode.h"

namespace Spr {
using namespace std;

//----------------------------------------------------------------------------
//	WXD3Driver
//
WXD3Driver::WXD3Driver(){
	bDesc7 = false;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
#if DIRECTDRAW_VERSION >= 0x700
	ZeroMemory(&desc7, sizeof(desc));
#endif
}
bool WXD3Driver::CheckRendDepth(int bpp){
	DWORD dwBpp = WXDDraw::BPPToDDBD(bpp);
#if DIRECTDRAW_VERSION >= 0x700
	if (bDesc7) return (dwBpp & desc7.dwDeviceRenderBitDepth) != 0;
#endif
	return (dwBpp & desc.dwDeviceRenderBitDepth) != 0;
}
void WXD3Driver::SetDesc(D3DDEVICEDESC& d){
	desc = d;
	bDesc7 = false;
}
#if DIRECTDRAW_VERSION >= 0x700
void WXD3Driver::SetDesc(D3DDEVICEDESC7& d7){
	desc7 = d7;
	bDesc7 = true;
}
#endif
DWORD WXD3Driver::ZBufferDepthMask(){
#if DIRECTDRAW_VERSION >= 0x700
	if (bDesc7) return desc7.dwDeviceZBufferBitDepth;
#endif
	return desc.dwDeviceZBufferBitDepth;
}
int WXD3Driver::ZBufferDepth(int max){
	return WXDDraw::FindBPP(ZBufferDepthMask(), max);
}

//----------------------------------------------------------------------------
//	WXD3Drivers
//
WXD3Drivers::WXD3Drivers(){
	cur = -1;
	prefer = 0;
}
WXD3Driver& WXD3Drivers::CurDriver(){
	if (cur < 0) cur = 0;
	if (cur > (int)size()-1) cur = size()-1;
	return *(begin()+cur);
}
bool WXD3Drivers::Select(int rendDepth, bool bHALOK){
	int i = prefer;
	if (i < 0) i = 0;
	if (i > (int)size()-1) i = size() - 1;
	if (bHALOK){
		for(UINT j=0; j<size(); j++,i++){
			if (i >= (int)size()) i = 0;
			if (begin()[i].bHAL && begin()[i].CheckRendDepth(rendDepth)){
				cur = i;
				return true;
			}
		}
	}
	for(UINT j=0; j<size(); j++,i++){
		if (i >= (int)size()) i = 0;
		if (!begin()[i].bHAL && begin()[i].CheckRendDepth(rendDepth)){
			cur = i;
			return true;
		}
	}
	return false;
}
bool WXD3Drivers::Select(){
	int i = prefer;
	if (i < 0) i = 0;
	if (i > (int)size()-1) i = size() - 1;
	for(UINT j=0; j<size(); j++,i++){
		if (i >= (int)size()) i = 0;
		if (begin()[i].bHAL){
			cur = i;
			return true;
		}
	}
	for(UINT j=0; j<size(); j++,i++){
		if (i >= (int)size()) i = 0;
		if (!begin()[i].bHAL){
			cur = i;
			return true;
		}
	}
	return false;
}
void WXD3Drivers::Set(int id){
	prefer = id;
}
#if DIRECTDRAW_VERSION >= 0x700
HRESULT PASCAL WXD3Drivers::EnumDevicesCallback(char* desc, char* name, D3DDEVICEDESC7* devDesc, void* pContext){
	WXD3Drivers* drivers = (WXD3Drivers*)pContext;
	WXD3Driver newDrv;
	newDrv.guid = devDesc->deviceGUID;
	newDrv.about = desc;
	newDrv.name = name;
	newDrv.bHAL = strstr(name, "HAL") != NULL;
	newDrv.SetDesc(*devDesc);
	drivers->push_back(newDrv);
	return D3DENUMRET_OK;
}
#endif
HRESULT PASCAL WXD3Drivers::EnumDevicesCallback(GUID* pGuid, char* a, char* n, D3DDEVICEDESC* pHAL, D3DDEVICEDESC* pHEL, void* pContext){
	WXD3Drivers* drivers = (WXD3Drivers*)pContext;
	WXD3Driver newDrv;
	newDrv.guid = *pGuid;
	newDrv.about = a;
	newDrv.name = n;
    if (pHAL->dcmColorModel){
        newDrv.bHAL = true;
		newDrv.SetDesc(*pHAL);
	}else{
        newDrv.bHAL = false;
        newDrv.SetDesc(*pHEL);
	}
	drivers->push_back(newDrv);
	return D3DENUMRET_OK;
}

//----------------------------------------------------------------------------
//	WXD3Device
//
bool WXD3Device::EnumDrivers(){
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->Create3D()) return false;
	drivers.clear();
	bool rv = true;
	rv &= !WXCHECK(pWXDD->pD3D->EnumDevices(drivers.EnumDevicesCallback, &drivers));
#if DIRECTDRAW_VERSION >= 0x700
	if (pWXDD->pD3D7) rv &= !WXCHECK(pWXDD->pD3D7->EnumDevices(drivers.EnumDevicesCallback, &drivers));
#endif
	return rv;
}
bool WXD3Device::CZSurface::Create(DWORD depth, bool bHAL, bool bCallCallback)
	{
	AUTOLOCK;
	if (pDDS) return true;
	Caps(DDSCAPS_ZBUFFER);
	desc.dwFlags |= DDSD_ZBUFFERBITDEPTH;
	desc.dwZBufferBitDepth = depth;
    if (bHAL) 
		{
		desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
		desc.ddsCaps.dwCaps &= ~DDSCAPS_SYSTEMMEMORY;
		if (WXDDSurface::Create(bCallCallback)) 
			{
			return true;
			}
		}
	desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	desc.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
	bool rtv = WXDDSurface::Create(bCallCallback);
	return rtv;
	}
WXD3Device::WXD3Device(){
	pD3DDev = NULL;
}
WXD3Device::~WXD3Device(){
	Release();
	if (bCallDeleteCallbacks){
		bCallDeleteCallbacks = false;
		deleteCallbacks.Call(this);
	}
}
void WXD3Device::DDraw(WXDDraw& dd)
	{
	Release();
	WXDDFlipSurface::DDraw(dd);
	zBuffer.DDraw(dd);
	EnumDrivers();
	}
bool WXD3Device::Create(bool bCallCallback, int nBackBuffer){
	AUTOLOCK;
	if (pDDS) return true;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->Create()) return false;
	if (!zBuffer.pWXDD) zBuffer.pWXDD = pWXDD;
	TRACEALL("WXD3Device::Create\n");
	if (drivers.size() == 0) EnumDrivers();
	if (drivers.size() == 0) return false;
	//	create flip 3D surface
	desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
	if (!WXDDFlipSurface::Create(false, nBackBuffer)) return false;
	//	init d3d
	if (!pWXDD->Create3D()) goto releaseReturn;
	//	select d3d driver that is good for this surface
	if (!drivers.Select(desc.bitPerPixel, (desc.ddsCaps.dwCaps&DDSCAPS_VIDEOMEMORY)!=0)){
		WXERRMSG(2, "Cannot find d3d driver that is good for this surface.");
		goto releaseReturn;
	}
	//	create zbuffer if it was requested.
	if (drivers.CurDriver().ZBufferDepth()){
		zBuffer.Release(bCallCallback);
		zBuffer.Size(Size());
		if (!zBuffer.Create(drivers.CurDriver().ZBufferDepth(), drivers.CurDriver().bHAL, bCallCallback)) goto releaseReturn;
		//	If z buffer is in system memory, we can't use HAL.
		if (drivers.CurDriver().bHAL && zBuffer.desc.ddsCaps.dwCaps&DDSCAPS_SYSTEMMEMORY){
			if (!drivers.Select(desc.bitPerPixel, false)){
				WXERRMSG(2, "Cannot find d3d driver that is good for this surface.");
				goto releaseReturn;
			}
			zBuffer.Release(bCallCallback);
			if (!zBuffer.Create(drivers.CurDriver().ZBufferDepth(), false, bCallCallback)) goto releaseReturn;
		}
		TRACEALL("WXD3Device::Create zBuf%d: (%d,%d) %d\n", zBuffer.Size().cx, zBuffer.Size().cy, zBuffer.desc.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY);
	}
	//	If d3ddriver is HEL, backbuffer should be in system memory.
	if (!drivers.CurDriver().bHAL && desc.ddsCaps.dwCaps&DDSCAPS_VIDEOMEMORY){
		WXDDFlipSurface::Release(false);
		desc.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
		desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
		WXDDFlipSurface::Create(false);
        if (!pDDS) return false;
		descOld.ddsCaps.dwCaps &= ~DDSCAPS_SYSTEMMEMORY;
	}
	//	Attach z buffer to the back buffer
	if (zBuffer.pDDS && WXCHECK(pDDS->AddAttachedSurface(zBuffer.pDDS))) goto releaseReturn;
	//	create d3ddevice
#if DIRECTDRAW_VERSION >= 0x500
	if (WXCHECK(pWXDD->pD3D->CreateDevice(drivers.CurDriver().guid, pDDS, &pD3DDev))) goto releaseReturn;
#else
	if (WXCHECK(pDDS2->QueryInterface(drivers.CurDriver().guid, (void**)&pD3DDev))) goto releaseReturn;
#endif
	if (bCallCallback) createCallbacks.Call(this);
	return true;
releaseReturn:
	Release(false);
	return false;
}
void WXD3Device::Release(bool bCallCallback){
	AUTOLOCK;
	if (!pDDS) return;
	TRACEALL("WXD3Device::Release\n");
	if (bCallCallback) releaseCallbacks.Call(this);
	RELEASE(pD3DDev);
	zBuffer.Release(bCallCallback);
	WXDDFlipSurface::Release(false);
}
bool WXD3Device::Restore()
	{
	AUTOLOCK;
	bool rtv = WXDDFlipSurface::Restore();
  	if (rtv && zBuffer.pDDS && zBuffer.pDDS->IsLost() == DDERR_SURFACELOST)
		{
		HRESULT hr = zBuffer.pDDS->Restore();
		if (hr!=DDERR_OUTOFMEMORY && hr!=DDERR_OUTOFVIDEOMEMORY)
			{
			Release();
			rtv = Create();
			goto leave;
			}
		if (WXCHECKMSG(hr, "restore surface failed."))
			{
			rtv = false;
			goto leave;
			}
		}
leave:
	return rtv;
	}

}	//	namespace Spr
