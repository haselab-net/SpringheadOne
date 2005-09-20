#include "WXAMStream.h"
#include <WinDX/DDraw/WXDDraw.h>
namespace LVRM {;

bool WXAMDDStream::CreateDDStream(){
	if (ddStream) return true;
	HRESULT hr = Intf()->QueryInterface(IID_IDirectDrawMediaStream, (void **)&ddStream.Intf());
	return hr == NOERROR && GetSurfaceDesc();
}
bool WXAMDDStream::Surf(IDirectDrawSurface* pSurf){
	if (!pSurf) return false;
	if (!CreateDDStream()) return false;
	DDSURFACEDESC ddsd;
    ddsd.dwSize = sizeof(ddsd);
    HRESULT hr = ddStream->GetFormat(&ddsd, NULL, NULL, NULL);
	if (hr != NOERROR) {
		HRESULT hr = pSurf->GetSurfaceDesc(&ddsd);
		if (hr != NOERROR) return false;
	}
    RECT rect;
    rect.top = rect.left = 0;
    rect.bottom = ddsd.dwHeight;
    rect.right = ddsd.dwWidth;
    sample.Release();
	hr = ddStream->CreateSample(pSurf, &rect, 0, &sample.Intf());
	return hr == NOERROR;
}
bool WXAMDDStream::GetSurfaceDesc(){
	HRESULT hr = ddStream->GetFormat(&desc, NULL, NULL, NULL);
	return  hr == NOERROR;
}
bool WXAMDDStream::Update(DWORD flags){
    if (sample){
		HRESULT hr = sample->Update(flags, NULL, NULL, NULL);
		return hr == NOERROR;
	}
	return false;
}

//----------------------------------------------------------------------------
WXAMMultiStream::WXAMMultiStream(DWORD flags){
	CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER, IID_IAMMultiMediaStream, (void **)&Intf());
    Intf()->Initialize(STREAMTYPE_READ, flags, graph.Intf());
}
bool WXAMMultiStream::AddVideo(){
    HRESULT hr = Intf()->AddMediaStream(WXDDraws::GetLastCreate()->Primary().pDDraw, &MSPID_PrimaryVideo, 0, NULL);
	return hr == NOERROR;
}
bool WXAMMultiStream::OpenMoniker(CMoniker m){
	IBindCtx* ctx;
	CreateBindCtx(0, &ctx);
	HRESULT hr = Intf()->OpenMoniker(ctx, m.Intf(), AMMSF_RENDERTOEXISTING);
	ctx->Release();
	return hr == NOERROR;
}
bool WXAMMultiStream::OpenFile(const char* fname){
	wchar_t buf[1024];
	mbstowcs(buf, fname, sizeof(buf));
	HRESULT hr = Intf()->OpenFile(buf, 0);
	return hr == NOERROR;
}
WXAMDDStream WXAMMultiStream::Video(){
	WXAMDDStream mstr;
	Intf()->GetMediaStream(MSPID_PrimaryVideo, &mstr.Intf());
	mstr.CreateDDStream();
	return mstr;
}
bool WXAMMultiStream::Run(){
	HRESULT hr = Intf()->SetState(STREAMSTATE_RUN);
	return hr == NOERROR;
}
bool WXAMMultiStream::Stop(){
	HRESULT hr = Intf()->SetState(STREAMSTATE_STOP);
	return hr == NOERROR;
}

}	//	namespace LVRM;
