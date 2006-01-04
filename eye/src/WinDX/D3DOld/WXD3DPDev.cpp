#include "WXD3DPDev.h"
#include "../DDraw/WXDDMode.h"

namespace Spr {
using namespace std;

#if DIRECTDRAW_VERSION >= 0x700
//----------------------------------------------------------------------------
//	WXD3Light
//
WXD3Light::WXD3Light(){
    dltType = D3DLIGHT_DIRECTIONAL;
    dcvDiffuse.r = dcvDiffuse.g = dcvDiffuse.b = dcvDiffuse.a = 1;
    dcvSpecular.r = dcvSpecular.g = dcvSpecular.b = dcvSpecular.a = 1;
    dcvAmbient.r = dcvAmbient.g = dcvAmbient.b = dcvAmbient.a = 0;
    dvPosition.x = dvPosition.y = dvPosition.z = 0;
    dvDirection.x = dvDirection.y = dvDirection.z = 1;
    dvRange = 10e10f;
    dvFalloff = 1;
    dvAttenuation0 = 1;
	dvAttenuation1 = 1;
	dvAttenuation2 = 1;
	dvTheta = float(M_PI/8);
	dvPhi	= float(M_PI/4);
}
//----------------------------------------------------------------------------
//	WXD3Viewport
//
WXD3Viewport::WXD3Viewport(WXD3Vec3 be, WXD3Vec3 e, WXD3Vec3 c, D3DVALUE f, D3DVALUE bk){
	begin = be;
	end = e;
	screen = c;
	front = f;
	back = bk;
}
WXD3Viewport::WXD3Viewport(WXD3Vec3 c, D3DVALUE f, D3DVALUE bk){
	end = WXD3Vec3(1,1,1);
	screen = c;
	front = f;
	back = bk;
}
WXD3Viewport::WXD3Viewport(D3DVALUE f, D3DVALUE bk){
	end = WXD3Vec3(1,1,1);
	front = f;
	back = bk;
}
WXD3Viewport::WXD3Viewport(const RECT& rc, D3DVALUE minZ, D3DVALUE maxZ){
	dwX = rc.left;					dwY = rc.top;
	dwWidth = rc.right - rc.left;	dwHeight = rc.bottom - rc.top;
	dvMinZ = minZ;					dvMaxZ = maxZ;
	front = 1.0f;
	back = dwWidth * 20;
}
WXD3Viewport::WXD3Viewport(const D3DVIEWPORT7& vp):D3DVIEWPORT7(vp){
	front = 1.0;
	back = dwWidth * 20;
}
void WXD3Viewport::TargetSize(SIZE sz){
	begin.clip(Vec3f(0,0,0), Vec3f(1,1,1));
	end.clip(Vec3f(0,0,0), Vec3f(1,1,1));
	WXD3Vec3 size = end - begin;
	size.clip(Vec3f(0,0,0), Vec3f(1,1,1));
	if (end != Vec3f()){
		dwWidth = size.X() * sz.cx;
		dwHeight = size.Y() * sz.cy;
		dwX = begin.X() * sz.cx;
		dwY = begin.Y() * sz.cy;
		dvMinZ = begin.Z();
		dvMaxZ = end.Z();
	}
}
WXD3Affine WXD3Viewport::Projection(){
	Affinef afProj;
	Vec2f sz(dwWidth, dwHeight);
    afProj = Affinef::ProjectionGL(screen, sz, front, back);
	return (WXD3Affine&)afProj;
}

//----------------------------------------------------------------------------
//	WXD3DPDevice
//
bool WXD3DPDevice::EnumDrivers(){
	AUTOLOCK;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->Create3D()) return false;
	drivers.clear();
	bool rv = !WXCHECK(pWXDD->pD3D7->EnumDevices(drivers.EnumDevicesCallback, &drivers));
	return rv;
}
bool WXD3DPDevice::CZSurface::Create(DWORD depth, bool bHAL, bool bCallCallback){
	AUTOLOCK;
	if (pDDS) return true;
	Caps(DDSCAPS_ZBUFFER);
	desc.dwFlags |= DDSD_ZBUFFERBITDEPTH;
	desc.dwZBufferBitDepth = depth;
    if (bHAL){
		desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
		desc.ddsCaps.dwCaps &= ~DDSCAPS_SYSTEMMEMORY;
		if (WXDDSurface::Create(bCallCallback)){
			return true;
		}
	}
	desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	desc.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
	bool rtv = WXDDSurface::Create(bCallCallback);
	return rtv;
}
WXD3DPDevice::WXD3DPDevice(){
	pD3DDev = NULL;
}
WXD3DPDevice::~WXD3DPDevice(){
	Release();
	RELEASE(pD3DDev);
	if (bCallDeleteCallbacks){
		bCallDeleteCallbacks = false;
		deleteCallbacks.Call(this);
	}
}
void WXD3DPDevice::DDraw(WXDDraw& dd){
	Release();
	WXDDFlipSurface::DDraw(dd);
	zBuffer.DDraw(dd);
	EnumDrivers();
}
bool WXD3DPDevice::Create(bool bCallCallback, int nBackBuffer){
	AUTOLOCK;
	if (pDDS) return true;
	if (!pWXDD) pWXDD = &(WXDDraws::GetLastCreate()->Primary());
	if (!pWXDD->Create()) return false;
	if (!zBuffer.pWXDD) zBuffer.pWXDD = pWXDD;
	TRACEALL("WXD3DPDevice::Create\n");
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
		TRACEALL("WXD3DPDevice::Create zBuf%d: (%d,%d) %d\n", zBuffer.Size().cx, zBuffer.Size().cy, zBuffer.desc.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY);
	}
	//	Attach z buffer to the back buffer
	if (zBuffer.pDDS && WXCHECK(pDDS->AddAttachedSurface(zBuffer.pDDS))) goto releaseReturn;
	//	create d3ddevice
	if (pD3DDev){
		if (WXCHECK( pD3DDev->SetRenderTarget(pDDS7, 0) )) goto releaseReturn;
	}else{
		WXCHECK( pWXDD->pD3D7->CreateDevice(drivers.CurDriver().guid, pDDS7, &pD3DDev) );
		if (!pD3DDev) goto releaseReturn;
	}
	Viewport(viewport);
	if (bCallCallback) createCallbacks.Call(this);
	return true;
releaseReturn:
	Release(false);
	return false;
}
void WXD3DPDevice::Release(bool bCallCallback){
	AUTOLOCK;
	if (!pDDS) return;
	TRACEALL("WXD3DPDevice::Release\n");
	if (bCallCallback) releaseCallbacks.Call(this);
	RELEASE(pD3DDev);
	zBuffer.Release(bCallCallback);
	WXDDFlipSurface::Release(false);
}
bool WXD3DPDevice::Restore(){
	AUTOLOCK;
	bool rtv = WXDDFlipSurface::Restore();
	if (rtv && zBuffer.pDDS && zBuffer.pDDS->IsLost() == DDERR_SURFACELOST){
		HRESULT hr = zBuffer.pDDS->Restore();
		if (hr!=DDERR_OUTOFMEMORY && hr!=DDERR_OUTOFVIDEOMEMORY){
			Release();
			return Create();
		}
		if (WXCHECKMSG(hr, "restore surface failed.")) return false;
	}
	return rtv;
}
bool WXD3DPDevice::ClearImp(POINT dest, SIZE sz, DWORD color, DWORD flags){
	AUTOLOCK;
	if (IsLost() && !Restore()) return false;
	if (sz.cx == -1) sz = Size();
	RECT rcDest;
	rcDest.left = dest.x;
	rcDest.top = dest.y;
	rcDest.right = dest.x + sz.cx;
	rcDest.bottom = dest.y + sz.cy;
	if (!pD3DDev) return false;
	return !WXCHECK(pD3DDev->Clear(1, (D3DRECT*)&rcDest, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, color, 1.0f, 0));
}
void WXD3DPDevice::Viewport(const WXD3Viewport& vp){
	viewport = vp;
	viewport.TargetSize(Size());
	WXCHECK(pD3DDev->SetViewport(&viewport));
	WXCHECK(pD3DDev->SetTransform(D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX*)&viewport.Projection()));
}
#endif

}	//	namespace Spr
