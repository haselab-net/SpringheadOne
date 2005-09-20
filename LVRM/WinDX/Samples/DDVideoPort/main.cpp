#include <LVRM/DDraw/WXDDVideoPort.h>


HRESULT WINAPI EnumSurfacesCallback(IDirectDrawSurface* lpDDSurface, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext){
	IDirectDrawSurface** ppS = (IDirectDrawSurface**)lpContext;
	*ppS = lpDDSurface;
	return DDENUMRET_OK;
}

int main(){	
	WXDDraws ddraws;
	WXDDVideoPorts videoPorts;
	videoPorts.Create();
	videoPorts.ports[0].Create(0);
	WXDDSurface surf;
	SIZE sz = {320, 240};
	surf.Size(sz);
	
	surf.desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT| DDSD_PIXELFORMAT;
	surf.Caps(DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY | DDSCAPS_VIDEOPORT);
	surf.desc.ddpfPixelFormat = videoPorts.ports[0].formats[0];
	surf.desc.dwBackBufferCount = 1;
	surf.Create();
	surf.Clear();
	videoPorts.ports[0].SetSurface(surf);
	videoPorts.ports[0].Start();

	ddraws.Primary().surfPrimary.Create();
	IDirectDrawSurface* pSurf = surf.pDDS;
	pSurf->AddRef();
	for(int i=0; i<100; i++){
		HRESULT hr;
		IDirectDrawSurface* pNextSurf;
		pSurf->EnumAttachedSurfaces(&pNextSurf, EnumSurfacesCallback);
		hr = videoPorts.ports[0]->Flip(pNextSurf, DDVPFLIP_VIDEO);
		WXDDSurface surfOld(pSurf);
		pSurf->Release();
		pSurf = pNextSurf;
		WXDDSurface& ps = ddraws.Primary().surfPrimary;
		surfOld.Lock();
		ps.Lock();
		for(int y=0; y<240; y++){
			memcpy(ps.Line(y), surfOld.Line(y), 320 * 2);
		}
		surfOld.Unlock();
		ps.Unlock();
		Sleep(100);
	}
	return 0;
}