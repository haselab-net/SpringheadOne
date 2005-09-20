//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>

//#include <mmsystem.h>
#include <uuids.h>
#include <dshow.h>
#include <qedit.h>
#include <windows.h>
#include <ExtCtrls.hpp>
#ifndef KT_RELEASE
#define KT_RELEASE(x) if((x)){ (x)->Release(); (x)=NULL; }
#endif // KT_RELEASE
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TOpenDialog *OpenDialog1;
	TMainMenu *MainMenu1;
	TMenuItem *OpenO1;
	TMenuItem *StopS1;
	TMenuItem *CaptureC1;
	TImage *Image1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OpenO1Click(TObject *Sender);
	void __fastcall StopS1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CaptureC1Click(TObject *Sender);
private:	// ユーザー宣言
	IGraphBuilder *pGraph;
	IMediaControl *pMediaControl;
	IVideoWindow *pVidWin;
	IBasicVideo *pBasicVideo;

	ICaptureGraphBuilder2 *pCapture;

	IBaseFilter *pGrabberFilter;
	ISampleGrabber *pGrabber;

	DWORD dwRegister; //GraphEditで見るためのおまじないで使用

	void CleanUp(); //Comインターフェイスを開放する
	bool IsAudioOnly();
	void ResultMBox( HRESULT hr,AnsiString header );
	IPin *GetPin( IBaseFilter *pFilter,PIN_DIRECTION PinDir ); // From Help
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
	void PlayStart();
	void PlayStop();
};

#define kt_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	const GUID name \
		= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

kt_DEFINE_GUID(
	kt_PIN_CATEGORY_CAPTURE,
	0xfb6c4281, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba
);

kt_DEFINE_GUID(
	kt_PIN_CATEGORY_PREVIEW,
	0xfb6c4282, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba
);

kt_DEFINE_GUID(
	kt_MEDIATYPE_Video,
	0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
);

kt_DEFINE_GUID(
	kt_MEDIATYPE_Interleaved,
	0x73766169, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
);

kt_DEFINE_GUID(
	kt_MEDIASUBTYPE_Avi,
0xe436eb88, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70
);

kt_DEFINE_GUID(
	kt_MEDIASUBTYPE_RGB24,
	0xe436eb7d, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70
);

kt_DEFINE_GUID(
	kt_IID_ISampleGrabber,
	0x6B652FFF, 0x11FE, 0x4fce, 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F
//	0x0579154A, 0x2B53, 0x4994, 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85
);

kt_DEFINE_GUID(
	kt_CLSID_SampleGrabber,
	0xC1F400A0, 0x3F08, 0x11d3, 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37
);

kt_DEFINE_GUID(
	kt_FORMAT_VideoInfo,
	0x05589f80, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a
);


/*
GUID kt_PIN_CATEGORY_CAPTURE = { 0xfb6c4281, 0x0353, 0x11d1,
	{ 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba } };

GUID kt_

GUID kt_MEDIATYPE_Video = { 0x73646976, 0x0000, 0x0010,
	{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
 */


// 以下、Helpから得た有用な関数
HRESULT AddToRot( IUnknown *pUnkGraph,DWORD *pdwRegister )
{
	IMoniker *pMoniker;
	IRunningObjectTable *pROT;
	if( FAILED(GetRunningObjectTable(0,&pROT)) ){
		return E_FAIL;
	}
	WCHAR wsz[256];
	wsprintfW( wsz,L"FilterGraph %08p pid %08x",(DWORD_PTR)pUnkGraph,GetCurrentProcessId() );
	HRESULT hr = CreateItemMoniker( L"!",wsz,&pMoniker );
	if( SUCCEEDED(hr) ){
		hr = pROT->Register( 0,pUnkGraph,pMoniker,pdwRegister );
		pMoniker->Release();
	}
	pROT->Release();
	return hr;
}

void RemoveFromRot( DWORD pdwRegister )
{
	IRunningObjectTable *pROT;
	if( SUCCEEDED( GetRunningObjectTable(0,&pROT) ) ){
		pROT->Revoke( pdwRegister );
		pROT->Release();
	}
}



//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

