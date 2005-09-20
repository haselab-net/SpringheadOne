#include "WXAMCallbackRenderer.h"
#include "WXAMBaseClasses.h"
#include "WXAMGuidInfo.h"

namespace LVRM {;

//-----------------------------------------------------------------------------
//	WXAMCallbackRendererImp
//
static const GUID CLSID_AMCallbackRenderer = { 0x9AEC363B, 0x8AC6, 0x4A6D, { 0xB7, 0x2D, 0x89, 0x94, 0xC5, 0x37, 0xBA, 0xE5 } };
class WXAMCallbackRendererImp: public CBaseVideoRenderer{
protected:
	GUID acceptMediaType;
	void (*callback)(SIZE bmpSize, BYTE* bmpPtr, int bmpLinePitch);
	WXAMCallbackRendererImp();
public:
    virtual ~WXAMCallbackRendererImp();
	void AcceptMediaType(GUID t){
		acceptMediaType = t;
	}
	void SetCallback(void (*f)(SIZE bmpSize, BYTE* bmpPtr, int bmpLinePitch)){
		callback = f;
	}
private:
	//	CBaseVideoRendererメンバのオーバーライド
	HRESULT CheckMediaType(const CMediaType* mt);	// 入力が許容できるフォーマットか検査する
	HRESULT SetMediaType(const CMediaType* mt);		// フォーマットを指定する
	HRESULT DoRenderSample(IMediaSample* ms);		// ビデオの新しい画像が送られてくる

	CMediaType  mediaType;							//	入力のメディアタイプ
	friend class WXAMCallbackRenderer;
};
WXAMCallbackRendererImp::WXAMCallbackRendererImp():
CBaseVideoRenderer(CLSID_AMCallbackRenderer, NAME("Callback Renderer"), NULL, NULL){
	callback = NULL;
}
WXAMCallbackRendererImp::~WXAMCallbackRendererImp(){
}

HRESULT WXAMCallbackRendererImp::CheckMediaType(const CMediaType* mt){
	if (*mt->FormatType() != FORMAT_VideoInfo) return E_INVALIDARG;
	VIDEOINFO* pVi = (VIDEOINFO *)mt->Format();
	if (!IsEqualGUID(*mt->Type(), MEDIATYPE_Video)) return E_FAIL;
	if (IsEqualGUID(*mt->Subtype(), acceptMediaType)) return NOERROR;
	return E_FAIL;
}
HRESULT WXAMCallbackRendererImp::SetMediaType(const CMediaType* mt){
	mediaType = *mt;
	return NOERROR;
}
HRESULT WXAMCallbackRendererImp::DoRenderSample(IMediaSample* sample){
	// Get source and dest details
	BYTE* bmpPtr;
	sample->GetPointer(&bmpPtr);
	VIDEOINFO* viBmp = (VIDEOINFO*) mediaType.Format();
	SIZE bmpSize;
	bmpSize.cx  = viBmp->bmiHeader.biWidth;
	bmpSize.cy = abs(viBmp->bmiHeader.biHeight);
	int bmpLinePitch = (bmpSize.cx * viBmp->bmiHeader.biBitCount/8 + 3) & ~3;
	if (viBmp->bmiHeader.biHeight < 0){
		bmpPtr += (bmpSize.cy-1) * bmpLinePitch;
		bmpLinePitch = -bmpLinePitch;
	}
	if (callback) callback(bmpSize, bmpPtr, bmpLinePitch);
	return NOERROR;
}

//-----------------------------------------------------------------------------
//	WXAMCallbackRenderer
//
WXAMCallbackRenderer::WXAMCallbackRenderer(){
	wcscpy(name, L"WXAMCallbackRenderer");
	imp = new WXAMCallbackRendererImp;
	imp->NonDelegatingQueryInterface(IID_IBaseFilter, (void**)&Intf());
};
WXAMCallbackRenderer::~WXAMCallbackRenderer(){
	imp = NULL;
}
void WXAMCallbackRenderer::MediaType(const GUID& t){
	if (imp) imp->AcceptMediaType(t);
}
void WXAMCallbackRenderer::Callback(void (*f)(SIZE bmpSize, BYTE* bmpPtr, int bmpLinePitch)){
	if (imp) imp->SetCallback(f);
}

}	//	namespace LVRM;
