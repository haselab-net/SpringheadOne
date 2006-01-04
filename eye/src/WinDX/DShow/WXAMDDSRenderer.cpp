#include "WXAMDDSRenderer.h"
#include "WXAMBaseClasses.h"

namespace Spr {;

//-----------------------------------------------------------------------------
//	WXAMDDSRendererImp
//
static const GUID CLSID_AMDDSurfRenderer = { 0x79A3ADEF, 0x639B, 0x4331, { 0xA7, 0x4D, 0xF8, 0x0E, 0xED, 0xFF, 0xA2, 0xBC } };

class WXAMDDSRendererImp: public CBaseVideoRenderer{
protected:
	GUID* pAcceptMediaType;
	WXDDSurfaceBase* pSurf;							//
	WXAMDDSRendererImp();							//
public:
	virtual ~WXAMDDSRendererImp();
	void Surf(WXDDSurfaceBase& surf);
	void AcceptMediaType(GUID t){ pAcceptMediaType = new GUID(t); }

private:
	//	CBaseVideoRendererメンバのオーバーライド
	HRESULT CheckMediaType(const CMediaType* mt);	// 入力が許容できるフォーマットか検査する
	HRESULT SetMediaType(const CMediaType* mt);		// フォーマットを指定する
	HRESULT DoRenderSample(IMediaSample* ms);		// ビデオの新しい画像が送られてくる

	CMediaType  mediaType;							//	入力のメディアタイプ
	friend class WXAMDDSRenderer;
};
WXAMDDSRendererImp::WXAMDDSRendererImp():
 CBaseVideoRenderer(CLSID_AMDDSurfRenderer, NAME("DirectDraw Surface Renderer"), NULL, NULL){
	pSurf = NULL;
	pAcceptMediaType = NULL;
}
WXAMDDSRendererImp::~WXAMDDSRendererImp(){
	delete pAcceptMediaType;
}
void WXAMDDSRendererImp::Surf(WXDDSurfaceBase& surf){
	pSurf = &surf;
}
inline const DWORD FourCC(const char* s){
	return *(DWORD*)s;
}
HRESULT WXAMDDSRendererImp::CheckMediaType(const CMediaType* mt){
	if (*mt->FormatType() != FORMAT_VideoInfo) return E_INVALIDARG;
	VIDEOINFO* pVi = (VIDEOINFO *)mt->Format();
	if (!IsEqualGUID(*mt->Type(), MEDIATYPE_Video)) return E_FAIL;
	if (pAcceptMediaType && IsEqualGUID(*mt->Subtype(), *pAcceptMediaType)) return NOERROR;

	
	//	Direct Draw Surface に合ったフォーマットを受け付ける
	if (pSurf){
		DDPIXELFORMAT& ddfmt = pSurf->Desc().ddpfPixelFormat;
#define FMTCHECK(cc)	(ddfmt.dwFourCC == FourCC(#cc) && IsEqualGUID(*mt->Subtype(), MEDIASUBTYPE_##cc))
		if (FMTCHECK(YVU9)) return NOERROR;
		if (FMTCHECK(Y411)) return NOERROR;
		if (FMTCHECK(Y41P)) return NOERROR;
		if (FMTCHECK(YUY2)) return NOERROR;
		if (FMTCHECK(YVYU)) return NOERROR;
		if (FMTCHECK(UYVY)) return NOERROR;
		if (FMTCHECK(Y211)) return NOERROR;
		if (FMTCHECK(CLJR)) return NOERROR;
		if (FMTCHECK(IF09)) return NOERROR;
		if (FMTCHECK(CPLA)) return NOERROR;

		if (pSurf->Desc().bitPerPixel==16 && pVi->bmiHeader.biBitCount == 16){
			if (pSurf->Desc().ddpfPixelFormat.dwGBitMask == 0x07E0 
				&& IsEqualGUID(*mt->Subtype(), MEDIASUBTYPE_RGB565)) return NOERROR;
			else if (IsEqualGUID(*mt->Subtype(), MEDIASUBTYPE_RGB555)) return NOERROR;
		}else if (pSurf->Desc().bitPerPixel==24 && pSurf->Desc().bitPerPixel==24){
			if(IsEqualGUID(*mt->Subtype(), MEDIASUBTYPE_RGB24)) return NOERROR;
		}else if (pSurf->Desc().bitPerPixel==32 && pSurf->Desc().bitPerPixel==32){
			if(IsEqualGUID(*mt->Subtype(), MEDIASUBTYPE_RGB32)) return NOERROR;
		}
	}
	return E_FAIL;
}
HRESULT WXAMDDSRendererImp::SetMediaType(const CMediaType* mt){
	mediaType = *mt;
	return NOERROR;
}
HRESULT WXAMDDSRendererImp::DoRenderSample(IMediaSample* sample){
	// Get source and dest details
	BYTE* bmpPtr;
	sample->GetPointer(&bmpPtr);
	VIDEOINFO* viBmp = (VIDEOINFO*) mediaType.Format();
	long bmpWidth  = viBmp->bmiHeader.biWidth;
	long bmpHeight = abs(viBmp->bmiHeader.biHeight);
	int bmpLinePitch = (bmpWidth*viBmp->bmiHeader.biBitCount/8 + 3) & 0xFFFFFFFC;
	if (viBmp->bmiHeader.biHeight > 0){
		bmpPtr += (bmpHeight-1) * bmpLinePitch;
		bmpLinePitch = -bmpLinePitch;
	}
	int copyLen = std::min(int(pSurf->Size().cx * pSurf->BytePerPixel()), abs(bmpLinePitch));
	int copyH = std::min(pSurf->Size().cy, bmpHeight);
	if (pSurf->Lock()){
		for(int y=0; y<copyH; y++){
			memcpy(pSurf->Line(y), bmpPtr, copyLen);
			bmpPtr += bmpLinePitch;
		}
		pSurf->Unlock();
	}
	return NOERROR;
}

//-----------------------------------------------------------------------------
//	WXAMDDSRenderer
//
WXAMDDSRenderer::WXAMDDSRenderer(){
	wcscpy(name, L"WXAMDDSRenderer");
	imp = new WXAMDDSRendererImp;
	imp->NonDelegatingQueryInterface(IID_IBaseFilter, (void**)&Intf());
};
WXAMDDSRenderer::~WXAMDDSRenderer(){
	imp = NULL;
}
void WXAMDDSRenderer::Surf(WXDDSurfaceBase& surf){
	if (imp) imp->Surf(surf);
}
void WXAMDDSRenderer::MediaType(const GUID& t){
	if (imp) imp->AcceptMediaType(t);
}

}	//	namespace Spr;
