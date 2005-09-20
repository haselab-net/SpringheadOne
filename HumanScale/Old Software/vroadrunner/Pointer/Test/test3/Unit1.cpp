//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	pGraph = NULL;
	pMediaControl = NULL;
	pVidWin = NULL;
}
//---------------------------------------------------------------------------

//Comインターフェイスを開放する
void TForm1::CleanUp()
{
	KT_RELEASE( pBasicVideo );
	KT_RELEASE( pVidWin );
	KT_RELEASE( pMediaControl );
	KT_RELEASE( pCapture );
	RemoveFromRot( dwRegister ); // GraphEditで見るためのおまじない
	KT_RELEASE( pGraph );
}

void TForm1::ResultMBox( HRESULT hr,AnsiString header )
{
	AnsiString msg;
	if( hr == S_OK )
		msg = "S_OK";
	else if( hr == S_FALSE )
		msg = "S_FALSE";
//	else if( hr == S_TRUE )
//		msg = "S_TRUE";
	else if( hr == E_FAIL )
		msg = "E_FAIL";
	else if( hr == E_INVALIDARG )
		msg = "E_INVALIDARG";
	else if( hr == E_NOTIMPL )
		msg = "E_NOTIMPL";
	else if( hr == E_OUTOFMEMORY )
		msg = "E_OUTOFMEMORY";
	else if( hr == E_POINTER )
		msg = "E_POINTER";
	else if( hr == E_UNEXPECTED )
		msg = "E_UNEXPECTED";
	else if( hr == E_NOINTERFACE )
		msg = "E_NOINTERFACE";
	else if( hr == VFW_E_NOT_CONNECTED )
		msg = "VFW_E_NOT_CONNECTED";
	else
		msg = AnsiString("Unknown Error:")+AnsiString(hr);

	header += ":";
	Application->MessageBox( (header+msg).c_str(),"Result",MB_OK );
}

void TForm1::PlayStart()
{
	wchar_t filename[256];
	HRESULT hr;

//	if( OpenDialog1->Execute() ){
//		OpenDialog1->FileName.WideChar( filename,256 );

		// フィルタグラフマネージャの作成
		//   IGraphBuilderには、フィルタをすべて削除という関数はないようなので、
		//   すべて消す代わりに新規に作る。
		//   そうしないと過去にRenderFileした映像も同時に別ウィンドウで流れる。
		CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
			IID_IGraphBuilder,(void**)&pGraph );
		if( !pGraph ) Application->MessageBox( "pGraph is NULL ","",MB_OK );

		// GraphEditアプリで見るためのおまじない
		AddToRot( pGraph,&dwRegister );

		// IMediaControlは再生・停止に利用。
		pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
		// IVideoWindowはビデオ表示画面の設定に利用。
		pGraph->QueryInterface( IID_IVideoWindow,(void**)&pVidWin );
		// IBasicVideoはビデオの縦横の大きさを得るのに利用。
		pGraph->QueryInterface( IID_IBasicVideo,(void**)&pBasicVideo );

		// キャプチャグラフビルダの作成
		CoCreateInstance( CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC,
			IID_ICaptureGraphBuilder2,(void**)&pCapture );
		pCapture->SetFiltergraph( pGraph );

		// システムデバイス列挙子の作成
		ICreateDevEnum *pDevEnum = NULL;
		CoCreateInstance( CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,
			IID_ICreateDevEnum,(void**)&pDevEnum );
		// ビデオキャプチャデバイスの列挙子の作成
		//   どうやらビデオキャプチャデバイスが一個もつながっていないと
		//   列挙子の作成自体が失敗するっぽい。
		IEnumMoniker *pClassEnum = NULL;
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,&pClassEnum,0 );
		if( S_FALSE == hr ){
			Application->MessageBox(
				"VideoInputDeviceCategoryが存在しません。","error",MB_OK );
			CleanUp();
			return;
		}

		// ビデオキャプチャデバイスを列挙
		ULONG cFetched;
		IMoniker *pMoniker = NULL;
		IBaseFilter *pCaptureFilter = NULL;
		if( pClassEnum->Next( 1,&pMoniker,&cFetched ) == S_OK ){
			// フレンドリ名を得る
			IPropertyBag *pPropBag = NULL;
			pMoniker->BindToStorage( 0,0,IID_IPropertyBag,(void**)&pPropBag );
			VARIANT varName;
			varName.vt = VT_BSTR;
			varName.bstrVal = NULL;
			pPropBag->Read( L"FriendlyName",&varName,0 );
			Application->MessageBox( AnsiString(varName.bstrVal).c_str(),"FriendlyName",MB_OK );
			SysFreeString( varName.bstrVal );
			KT_RELEASE(pPropBag);

			// 最初のモニカをフィルタオブジェクトにバインドする
//if( S_OK != (hr =
			pMoniker->BindToObject( 0,0,IID_IBaseFilter,(void**)&pCaptureFilter );
//))	ResultMBox( hr,"POINT" );
			pMoniker->Release();
		}
		pClassEnum->Release();
		pDevEnum->Release();

		if( !pCaptureFilter ){
			Application->MessageBox(
				"ビデオキャプチャデバイスが列挙されませんでした。","error",MB_OK );
		}

		// キャプチャフィルタをグラフに追加
		pGraph->AddFilter( pCaptureFilter,L"Video Capture" );

		// ファイル書き込みフィルタの設定
		IBaseFilter *ppf = NULL; //マルチプレクサ
		IFileSinkFilter *pSink = NULL; // ファイルライタ
		//pCapture->SetOutputFileName( &kt_MEDIASUBTYPE_Avi,
		//	L"D:\\test.avi",&ppf,&pSink );

		// グラフを構築する
//		pGraph->RenderFile( filename,NULL );
//		pCapture->RenderStream( &kt_PIN_CATEGORY_CAPTURE,&kt_MEDIATYPE_Video,
//			pCaptureFilter,NULL,ppf );
		pCapture->RenderStream( &kt_PIN_CATEGORY_PREVIEW,&kt_MEDIATYPE_Video,
			pCaptureFilter,NULL,NULL );
		REFERENCE_TIME rtStart=0, rtStop=100000000;
//		pCapture->ControlStream( &kt_PIN_CATEGORY_CAPTURE,NULL,NULL,
//			&rtStart,&rtStop,0,0 );

		// オーディオのみかを判断
		if( !IsAudioOnly() ){
			// ビデオウィンドウを構築する
			if( S_OK != pVidWin->put_Owner( (OAHWND)Handle ) ){
				Application->MessageBox("put_Owner failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS ) )
			{
				Application->MessageBox( "put_WindowStyle failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_Visible( OATRUE ) ){
				Application->MessageBox( "put_Visible failed","",MB_OK );
			}

			// ビデオの大きさを得て、ウィンドウサイズを変更
			long width,height;
			pBasicVideo->get_SourceWidth( &width );
			pBasicVideo->get_SourceHeight( &height );
			Visible = false;
			ClientWidth = width;
			if( Width > Screen->Width ) Width = Screen->Width;
			ClientHeight = height;
			if( Height > Screen->Height ) Height = Screen->Height;
			Left = ( Screen->Width - Width ) / 2;
			Top = ( Screen->Height - Height ) / 2;
			Visible = true;

			// ポジションをセット
			pVidWin->SetWindowPosition( 0,0,ClientWidth,ClientHeight );
		}

		// グラフを実行する
		pMediaControl->Run();

		// キャプチャデバイスのプロパティページを開く
		ISpecifyPropertyPages *pSpecify;
		hr = pCaptureFilter->QueryInterface( IID_ISpecifyPropertyPages,(void**)&pSpecify );
		ResultMBox( hr,"SpecifyPropertyPages(Filter)" );
		if( pSpecify ){
			CAUUID caGUID;
			pSpecify->GetPages( &caGUID );
			KT_RELEASE( pSpecify );
			OleCreatePropertyFrame( NULL,0,0,L"setting",1,(IUnknown**)&pCaptureFilter,
				caGUID.cElems,caGUID.pElems,0,0,NULL );
			CoTaskMemFree( caGUID.pElems );
			KT_RELEASE( pSpecify );
		}

		// キャプチャデバイスから出ているピンを列挙してプロパティページを開く
		IEnumPins *pEnumPins;
		IPin *pPin;
		pCaptureFilter->EnumPins( &pEnumPins );
		while( pEnumPins->Next(1,&pPin,0) == S_OK ){
			hr = pPin->QueryInterface( IID_ISpecifyPropertyPages,(void**)&pSpecify );
			ResultMBox( hr,"SpecifyPropertyPages(Pin)" );
			if( pSpecify ){
				CAUUID caGUID;
				pSpecify->GetPages( &caGUID );
				KT_RELEASE( pSpecify );
				OleCreatePropertyFrame( NULL,0,0,L"setting",1,(IUnknown**)&pCaptureFilter,
					caGUID.cElems,caGUID.pElems,0,0,NULL );
				CoTaskMemFree( caGUID.pElems );
				KT_RELEASE( pSpecify );
			}
			KT_RELEASE( pPin );
		}
		KT_RELEASE( pEnumPins );

		/*		IAMVfwCaptureDialogs *pCaptureDialogs = NULL;
hr=		pCapture->FindInterface( &kt_PIN_CATEGORY_CAPTURE,&kt_MEDIATYPE_Video,
			pCaptureFilter,IID_IAMVfwCaptureDialogs,(void**)&pCaptureDialogs );
ResultMBox(hr,"CaptureDialog");
		if( pCaptureDialogs ){
			Application->MessageBox("Success pCaptureDialogs","",MB_OK);
			KT_RELEASE( pCaptureDialogs );
		}
*/
//	}
}

void TForm1::PlayStop()
{
	// 再生の停止
	if( pMediaControl ){
		pMediaControl->Stop();
	}
	if( pVidWin ){
		pVidWin->put_Visible(OAFALSE); //忘れると画面描画がおかしくなる
		pVidWin->put_Owner(NULL); //忘れるとウィンドウにメッセージが送り続けられる
	}

	// インターフェイスを開放
	CleanUp();
}

// ファイルがオーディオのみかをチェック
// playwndサンプルのvoid CheckVisibility()を参考にした。
bool TForm1::IsAudioOnly()
{
	// オーディオのみのファイルを読み込んだときは
	// VideoWindowインターフェイスが存在しないことがあるっぽい。
	if( !pVidWin ) return true;

	// VideoWindowインターフェイスが存在するときでも
	// オーディオのみのファイルはビデオレンダラが接続されないので
	// メンバ関数を呼んでも VFW_E_NOT_CONNECTED を返す。
	// (追記)E_NOINTERFACE を返すこともあるようだ。
	// ここでは適当にget_Width関数を呼んでみる。
	long temp;
	HRESULT hr = pVidWin->get_Width( &temp );
	return hr == VFW_E_NOT_CONNECTED || hr == E_NOINTERFACE;

/*	// 本当はこのようにエラー内容をチェックした方が良いと思われ。
	hr = pVidWin->get_Visible( &temp );
	if( hr == S_OK )
		Application->MessageBox( "S_OK","",MB_OK );
	else if( hr == VFW_E_NOT_CONNECTED )
		Application->MessageBox( "VFW_E_NOT_CONNECTED","",MB_OK );
	else if( hr == E_NOINTERFACE )
		Application->MessageBox( "E_NOINTERFACE","",MB_OK );
	else
		Application->MessageBox(
			(AnsiString("Unknown Error:")+AnsiString(hr)).c_str(),"",MB_OK );

	//return VFW_E_NOT_CONNECTED == pVidWin->get_Width( &temp );
	return S_OK != pVidWin->get_Width( &temp );
*/
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	CoInitialize( NULL );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenO1Click(TObject *Sender)
{
	PlayStop();
	PlayStart();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopS1Click(TObject *Sender)
{
	PlayStop();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	PlayStop();
}
//---------------------------------------------------------------------------

