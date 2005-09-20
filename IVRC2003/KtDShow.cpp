#include <string.h>
#include "KtDShow.h"
// 必要最小限のエラーチェックしかしないバージョン

//----------------------------------------------------------
// KtDShowBasic
// DirectShowのファイル再生を行うクラス
KtDShowBasic::KtDShowBasic()
{
	m_pGraph = NULL;
	m_pMediaControl = NULL;
	m_pVidWin = NULL;
	m_pBasicVideo = NULL;
	m_dwRegister = 0;
	m_inputFilename[0] = '\0';
	m_hOutputWindow = NULL;
	m_errorMsg = "";
	m_bAddToRot = false;
}

KtDShowBasic::~KtDShowBasic()
{
	Close();
}

bool KtDShowBasic::Open()
{
	// すでにフィルタグラフマネージャが存在していたら、Closeする。
	if( m_pGraph ){
		Close();
	}

	// フィルタグラフマネージャの作成
	//   IGraphBuilderには、フィルタをすべて削除という関数はないようなので、
	//   すべて消す代わりに新規に作る。
	//   そうしないと過去にRenderFileした映像も同時に別ウィンドウで流れる。
	CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
		IID_IGraphBuilder,(void**)&m_pGraph );
	if( !m_pGraph ){
		m_errorMsg = "フィルタグラフマネージャを作成できませんでした。";
		return false;
	}

	if( m_bAddToRot ){
		AddToRot();
	}

	// IMediaControlは再生・停止に利用。
	m_pGraph->QueryInterface( IID_IMediaControl,(void**)&m_pMediaControl );
	// IVideoWindowはビデオ表示画面の設定に利用。
	m_pGraph->QueryInterface( IID_IVideoWindow,(void**)&m_pVidWin );
	// IBasicVideoはビデオの縦横の大きさを得るのに利用。
	m_pGraph->QueryInterface( IID_IBasicVideo,(void**)&m_pBasicVideo );

	return true;
}

void KtDShowBasic::Close()
{
	Stop();
	RemoveFromRot();
	KT_RELEASE( m_pGraph );
	KT_RELEASE( m_pMediaControl );
	KT_RELEASE( m_pVidWin );
	KT_RELEASE( m_pBasicVideo );
}

void KtDShowBasic::SetInputFile( char *filename )
{
	MultiByteToWideChar( CP_ACP,NULL,filename,strlen(filename)+1,
		m_inputFilename,sizeof(m_inputFilename) );
}

void KtDShowBasic::SetOutputWindow( HWND hwnd )
{
	m_hOutputWindow = hwnd;
}

void KtDShowBasic::SetDrawPosition()
{
	if( !HaveVideoData() || !m_pVidWin ){
		return;
	}

	if( m_hOutputWindow ){
		// 表示する位置をクライアント領域に設定
		RECT rect;
		GetClientRect( m_hOutputWindow,&rect );
		m_pVidWin->SetWindowPosition( 0,0,rect.right,rect.bottom );
	}
}

void KtDShowBasic::SetDrawPosition( int left,int top,int right,int bottom )
{
	if( !HaveVideoData() || !m_pVidWin ){
		return;
	}

	m_pVidWin->SetWindowPosition( left,top,right,bottom );
}

bool KtDShowBasic::Render()
{
	HRESULT hr;

	// 前準備をする
	if( !Open() ){
		return false;
	}

//	if( !m_pGraph || !m_pVidWin ){
	if( !m_pGraph ){
		return false;
	}

	// フィルタグラフを構築
	hr = m_pGraph->RenderFile( m_inputFilename,NULL );
	if( hr != S_OK ){
		m_errorMsg = GetResultErrorString( hr );
		return false;
	}

	return true;
}

bool KtDShowBasic::HaveVideoData()
{
	// オーディオのみのファイルを読み込んだときは
	// VideoWindowインターフェイスが存在しないことがあるっぽい。
	if( !m_pVidWin ){
		return false;
	}

	// VideoWindowインターフェイスが存在するときでも
	// オーディオのみのファイルはビデオレンダラが接続されないので
	// メンバ関数を呼んでも VFW_E_NOT_CONNECTED を返す。
	// (追記)E_NOINTERFACE を返すこともあるようだ。
	// ここでは適当にget_Width関数を呼んでみる。
	long temp;
	HRESULT hr = m_pVidWin->get_Width( &temp );
	return hr == S_OK;
}

long KtDShowBasic::GetVideoWidth()
{
	if( !m_pBasicVideo ){
		return -1;
	}

	long width;
	HRESULT hr;
	hr = m_pBasicVideo->get_SourceWidth( &width );
	return ( hr == S_OK ) ? width : 0;
}

long KtDShowBasic::GetVideoHeight()
{
	if( !m_pBasicVideo ){
		return -1;
	}

	long height;
	HRESULT hr;
	hr = m_pBasicVideo->get_SourceHeight( &height );
	return ( hr == S_OK ) ? height : 0;
}

bool KtDShowBasic::Run()
{
	HRESULT hr;

	if( !m_pGraph ){
		m_errorMsg = "Openされていない状態でRunが実行されました。";
		return false;
	}

	// 映像データを持っている場合
	if( HaveVideoData() ){
		// オーナーウィンドウを設定
		hr = m_pVidWin->put_Owner( (OAHWND)m_hOutputWindow );
		if( hr != S_OK ){
			m_errorMsg = GetResultErrorString( hr );
			return false;
		}

		// OutputWindowが指定された場合はウィンドウスタイルを設定
		if( m_hOutputWindow ){
			m_pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
			m_pVidWin->put_Visible( OATRUE );
//			SetDrawPosition();
		}
	}

	m_pMediaControl->Run();

	return true;
}

bool KtDShowBasic::Stop()
{
	if( !m_pGraph ){
		return false;
	}

	 m_pMediaControl->Stop();

	 if( HaveVideoData() && m_pVidWin ){
		m_pVidWin->put_Visible( OAFALSE );
		m_pVidWin->put_Owner( NULL );
	 }

	 return true;
}

bool KtDShowBasic::AddToRot()
{
	if( !m_pGraph || !m_bAddToRot){
		return false;
	}

	// すでに登録済みのとき
	if( m_dwRegister ){
		return true;
	}

	IMoniker *pMoniker;
	IRunningObjectTable *pROT;
	if( FAILED(GetRunningObjectTable(0,&pROT)) ){
		m_errorMsg = "ROTの取得に失敗しました。";
		return false;
	}
	WCHAR wsz[256];
	wsprintfW( wsz,L"FilterGraph %08p pid %08x",(DWORD_PTR)((IUnknown*)m_pGraph),
		GetCurrentProcessId() );
	HRESULT hr = CreateItemMoniker( L"!",wsz,&pMoniker );
	if( SUCCEEDED(hr) ){
		hr = pROT->Register( 0,(IUnknown*)m_pGraph,pMoniker,&m_dwRegister );
		pMoniker->Release();
	}
	pROT->Release();
	return SUCCEEDED( hr );
}

void KtDShowBasic::RemoveFromRot()
{
	if( !m_dwRegister ){
		return;
	}

	IRunningObjectTable *pROT;
	if( SUCCEEDED( GetRunningObjectTable(0,&pROT) ) ){
		pROT->Revoke( m_dwRegister );
		pROT->Release();
	}

	m_dwRegister = 0;
}

IPin* KtDShowBasic::GetPin( IBaseFilter *pFilter,PIN_DIRECTION PinDir )
{
	bool bFound = false;
	IEnumPins *pEnum;
	IPin *pPin;

	pFilter->EnumPins( &pEnum );
	while( pEnum->Next(1,&pPin,0) == S_OK ){
		PIN_DIRECTION PinDirThis;
		pPin->QueryDirection( &PinDirThis );
		if( bFound = ( PinDir == PinDirThis ) ){
			break;
		}
		pPin->Release();
	}
	pEnum->Release();
	return bFound ? pPin : NULL;
}

IPin* KtDShowBasic::GetPinByName( IBaseFilter *pFilter,WCHAR *pinname )
{
	bool bFound = false;
	IEnumPins *pEnum;
	IPin *pPin;

	pFilter->EnumPins( &pEnum );
	while( pEnum->Next(1,&pPin,0) == S_OK ){
		PIN_INFO PinInfo;
		pPin->QueryPinInfo( &PinInfo );
		if( wcscmp(pinname,PinInfo.achName) == 0 ){
            bFound = true;
			break;
		}
		pPin->Release();
	}
	pEnum->Release();
	return bFound ? pPin : NULL;
}

char* KtDShowBasic::GetResultErrorString( HRESULT hr )
{
	switch( hr ){
		case VFW_S_AUDIO_NOT_RENDERED:
			return "オーディオ用のレンダラが見つかりませんでした。";

		case VFW_S_VIDEO_NOT_RENDERED:
			return "映像用のレンダラが見つかりませんでした。";

		case E_FAIL:
			return "(内部エラー)失敗しました。";
		
		case E_POINTER:
			return "(内部エラー)NULLポインタ引数";
		
		case VFW_E_CANNOT_CONNECT:
			return "接続を確立する中間フィルタの組み合わせが見つかりませんでした。";
		
		case VFW_E_CANNOT_LOAD_SOURCE_FILTER:
			return "ファイルのソースフィルタをロードできませんでした。";
		
		case VFW_E_INVALID_FILE_FORMAT:
			return "ファイルフォーマットが無効です。";
		
		case VFW_E_UNSUPPORTED_STREAM:
			return "ファイルのフォーマットがサポートされていません。";

		default:
			return "登録されていないエラーです。";
	}
}

//----------------------------------------------------------
// KtDShowCapture
// DirectShowでカメラからキャプチャを行うクラス
KtDShowCapture::KtDShowCapture()
{
	m_pCapture = NULL;
	m_pGrabber = NULL;
	m_captureDeviceIndex = -1;
	m_outputFilename[0] = '\0';
	m_sampleBuffer = NULL;
	m_sampleBufferSize = 0;
	m_hSampleBitmap = NULL;

    m_bGetSample = true;
	// カメラを列挙
	FindCaptureDevices();
}

KtDShowCapture::~KtDShowCapture()
{
	// キャプチャデバイス配列を削除
	while( !m_captureDevices.empty() ){
		delete m_captureDevices.back();
		m_captureDevices.pop_back();
	}

	Close();
}

bool KtDShowCapture::Open()
{
	if( !KtDShowBasic::Open() ){
		return false;
	}

	// キャプチャグラフビルダの作成
	CoCreateInstance( CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&m_pCapture );
	m_pCapture->SetFiltergraph( GetGraphBuilder() );
	
	if( !m_pCapture ){
		SetErrorMsg( "キャプチャグラフマネージャを作成できませんでした。" );
		Close();
		return false;
	}

	return true;
}

void KtDShowCapture::Close()
{
	KT_RELEASE( m_pGrabber );
	KT_RELEASE( m_pCapture );
	KtDShowBasic::Close();
}

void KtDShowCapture::FindCaptureDevices()
{
	// キャプチャデバイス配列を削除
	while( !m_captureDevices.empty() ){
		delete m_captureDevices.back();
		m_captureDevices.pop_back();
	}
	m_captureDeviceIndex = -1;

	// システムデバイス列挙子の作成
	ICreateDevEnum *pDevEnum = NULL;
	CoCreateInstance( CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,
		IID_ICreateDevEnum,(void**)&pDevEnum );

	// ビデオキャプチャデバイスの列挙子の作成
	//   どうやらビデオキャプチャデバイスが一個もつながっていないと
	//   列挙子の作成自体が失敗するっぽい。
	HRESULT hr;
	IEnumMoniker *pClassEnum = NULL;
	hr = pDevEnum->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,&pClassEnum,0 );
	if( S_FALSE == hr ){
		KT_RELEASE( pDevEnum );
		return;
	}

	// ビデオキャプチャデバイスを列挙
	ULONG cFetched;
	IMoniker *pMoniker = NULL;
	KtCaptureDevice *device;
	while( pClassEnum->Next( 1,&pMoniker,&cFetched ) == S_OK ){
		// KtDShowCaptureDevideを作成して配列に追加
		device = new KtCaptureDevice( pMoniker );
		m_captureDevices.push_back( device );
		KT_RELEASE( pMoniker );
	}
	KT_RELEASE( pClassEnum );
	KT_RELEASE( pDevEnum );

	// 最初のキャプチャデバイスを選択
	m_captureDeviceIndex = 0;
}

void KtDShowCapture::SetOutputFile( char *filename )
{
	strcpy( filename,m_outputFilename );
}

int KtDShowCapture::GetNumOfCaptureDevices()
{
	return m_captureDevices.size();
}

bool KtDShowCapture::SetCaptureDeviceIndex(int index)
{
	if( index < 0 || index > GetNumOfCaptureDevices()-1 ){
		return false;
	}

	m_captureDeviceIndex = index;

	return true;
}

KtCaptureDevice* KtDShowCapture::GetCaptureDevice(int index)
{
	if( index < 0 || index > GetNumOfCaptureDevices()-1 ){
		return NULL;
	}

	return m_captureDevices[index];
}

BYTE* KtDShowCapture::GetCurrentBuffer()
{
	if( !m_pGrabber ) return NULL;
//	m_sampleBufferSize = 0;
	m_pGrabber->GetCurrentBuffer( &m_sampleBufferSize,(long*)m_sampleBuffer );
	return m_sampleBuffer;
}

HBITMAP KtDShowCapture::GetCurrentBitmap()
{
	if( !m_pGrabber ) return NULL;

	// すでにビットマップオブジェクトが存在する場合は削除
	if( m_hSampleBitmap ){
		DeleteObject( m_hSampleBitmap );
		m_hSampleBitmap = NULL;
	}

	AM_MEDIA_TYPE MediaType;
	m_pGrabber->GetConnectedMediaType( &MediaType );

	// ビデオヘッダーへのポインタを取得する
	VIDEOINFOHEADER *pVideoHeader = (VIDEOINFOHEADER*)MediaType.pbFormat;

	// ビデオヘッダーにはビットマップ情報が含まれる。
	// ビットマップ情報を BITMAPINFO 構造体にコピーする。
	BITMAPINFO BitmapInfo;
	ZeroMemory( &BitmapInfo,sizeof(BitmapInfo) );
	CopyMemory( &BitmapInfo.bmiHeader,&(pVideoHeader->bmiHeader),
		sizeof(BITMAPINFOHEADER) );

	// ビットマップヘッダーからDIBを作成し、バッファへのポインタを得る。
	void *buffer = NULL;
	m_hSampleBitmap = CreateDIBSection( 0,&BitmapInfo,DIB_RGB_COLORS,
		&buffer,NULL,0 );

	// イメージをバッファにコピーする
	long bufferSize = 0;
	m_pGrabber->GetCurrentBuffer( &bufferSize,(long*)buffer );

/*
	// サイズを得る
	long width = GetVideoWidth();
	long height = GetVideoHeight();

	// BitBuiltで描画
	//HDC hdcSrc = CreateCompatibleDC( Form1->Image1->Canvas->Handle );
	HDC hdcSrc = CreateCompatibleDC( NULL );
	HGDIOBJ hobj;
	hobj = SelectObject( hdcSrc,hBitmap );
	BitBlt( Form1->Image1->Canvas->Handle,0,0,width,height,hdcSrc,0,0,SRCCOPY );
	Image1->Repaint();
*/
	return m_hSampleBitmap;
}


bool KtDShowCapture::Render()
{
	// すでにpCaptureがある場合はCloseする。
	if( m_pCapture ){
		Close();
	}

	if( !Open() ){
		return false;
	}

	if( m_captureDeviceIndex < 0 ||
		m_captureDeviceIndex > GetNumOfCaptureDevices()-1 ){
		return false;
	}

	// キャプチャフィルタをグラフに追加
	IBaseFilter *pCaptureFilter =
		m_captureDevices[m_captureDeviceIndex]->GetIBaseFilter();
	GetGraphBuilder()->AddFilter( pCaptureFilter,L"Video Capture" );

	// 書き込みファイルが指定してある場合
	IBaseFilter *ppf = NULL; //マルチプレクサ
	IFileSinkFilter *pSink = NULL; // ファイルライタ
	if( m_outputFilename[0] ){
		// ファイル書き込みフィルタの設定
//		m_pCapture->SetOutputFileName( &MEDIASUBTYPE_Avi,
//			L"D:\\test.avi",&ppf,&pSink );
	}

	// SmartTeeフィルタの作成
	IBaseFilter *pSmartTee;
	CoCreateInstance( CLSID_SmartTee,NULL,CLSCTX_INPROC,
		IID_IBaseFilter,(void**)&pSmartTee );
	GetGraphBuilder()->AddFilter( pSmartTee,L"SmartTee" );

	// SmartTeeフィルタのPreviewピンにつないで
	// レンダラーにつなぐ
	if( m_bGetSample ){
		//サンプルグラバーをセットする場合
		// SampleGrabberフィルタの作成
		AM_MEDIA_TYPE mt;
		IBaseFilter *pGrabberFilter = NULL;
		CoCreateInstance( CLSID_SampleGrabber,NULL,CLSCTX_INPROC_SERVER,
			IID_IBaseFilter,(void**)&pGrabberFilter );
		pGrabberFilter->QueryInterface( IID_ISampleGrabber,(void**)&m_pGrabber );
		ZeroMemory( &mt,sizeof(AM_MEDIA_TYPE) );
		mt.majortype = MEDIATYPE_Video;
		mt.subtype = MEDIASUBTYPE_RGB24;
		mt.formattype = FORMAT_VideoInfo;
		m_pGrabber->SetMediaType( &mt );
		// グラフに追加
		GetGraphBuilder()->AddFilter( pGrabberFilter,L"SampleGrabber" );
		// サンプルをバッファにコピーするように設定
		m_pGrabber->SetBufferSamples(TRUE);

		// USBカメラ→SmartTee
		// SmartTee.Preview→SampleGrabber
		// SampleGrabber→VideoRendererを接続する。
		IPin *pCaptureOut = GetPin( pCaptureFilter,PINDIR_OUTPUT );
		IPin *pSmartTeeIn = GetPin( pSmartTee,PINDIR_INPUT );
        IPin *pSmartTeePreview = GetPinByName( pSmartTee,L"Preview" );
		IPin *pGrabberIn = GetPin( pGrabberFilter,PINDIR_INPUT );
		IPin *pGrabberOut = GetPin( pGrabberFilter,PINDIR_OUTPUT );

		GetGraphBuilder()->Connect( pCaptureOut,pSmartTeeIn );
		GetGraphBuilder()->Connect( pSmartTeePreview,pGrabberIn );
		m_pCapture->RenderStream( NULL,NULL,pGrabberOut,NULL,NULL );

		ShowCaptureOutputProperty();

		KT_RELEASE( pCaptureOut );
		KT_RELEASE( pGrabberIn );
		KT_RELEASE( pGrabberOut );
		KT_RELEASE( pSmartTeeIn );
        KT_RELEASE( pSmartTeePreview );
		KT_RELEASE( pGrabberFilter );
	}else{
		// USBカメラ→SmartTee を接続する。
		IPin *pCaptureOut = GetPin( pCaptureFilter,PINDIR_OUTPUT );
		IPin *pSmartTeeIn = GetPin( pSmartTee,PINDIR_INPUT );
        IPin *pSmartTeePreview = GetPinByName( pSmartTee,L"Preview" );

		GetGraphBuilder()->Connect( pCaptureOut,pSmartTeeIn );
		m_pCapture->RenderStream( NULL,NULL,pSmartTeePreview,NULL,NULL );

		KT_RELEASE( pCaptureOut );
		KT_RELEASE( pSmartTeeIn );
        KT_RELEASE( pSmartTeePreview );
	}
	KT_RELEASE( pSmartTee );

	// 残りのフィルタグラフを構築
	if( m_outputFilename[0] ){
		// ファイル書き込みはファイル名が指定されているときのみ
		m_pCapture->RenderStream( NULL,NULL,pCaptureFilter,NULL,ppf );
	}

	return true;
}

bool KtDShowCapture::Run()
{
	if( !KtDShowBasic::Run() ){
		return false;
	}

	if( HaveVideoData() ){
		// サンプルを得るときはバッファを確保
		if( m_bGetSample ){
			AM_MEDIA_TYPE MediaType;
			m_pGrabber->GetConnectedMediaType( &MediaType );
			m_sampleBufferSize = MediaType.lSampleSize;

			if( m_sampleBufferSize != 0 ){
				m_sampleBuffer = new BYTE[m_sampleBufferSize];
			}else{
				m_bGetSample = false;
			}
		}
	}

	return true;
}

bool KtDShowCapture::Stop()
{
	KtDShowBasic::Stop();

	if( m_sampleBuffer ){
		delete[] m_sampleBuffer;
		m_sampleBuffer = NULL;
		m_sampleBufferSize = 0;
	}

	if( m_hSampleBitmap ){
		DeleteObject( m_hSampleBitmap );
		m_hSampleBitmap = NULL;
	}

	return true;
}

bool KtDShowCapture::ShowCaptureDeviceProperty()
{
	if( m_captureDeviceIndex < 0 ||
		m_captureDeviceIndex > GetNumOfCaptureDevices()-1 ){
		return false;
	}

	IBaseFilter *pCaptureFilter =
		m_captureDevices[m_captureDeviceIndex]->GetIBaseFilter();
	ISpecifyPropertyPages *pSpecify;
	HRESULT hr = pCaptureFilter->QueryInterface(
		IID_ISpecifyPropertyPages,(void**)&pSpecify );
	if( !SUCCEEDED(hr) ){
		SetErrorMsg( "キャプチャデバイスのプロパティはありません。" );
		return false;
	}

	CAUUID caGUID;
	pSpecify->GetPages( &caGUID );
	OleCreatePropertyFrame( NULL,0,0,L"setting",1,(IUnknown**)&pCaptureFilter,
		caGUID.cElems,caGUID.pElems,0,0,NULL );
	CoTaskMemFree( caGUID.pElems );
	KT_RELEASE( pSpecify );

	return true;
}

bool KtDShowCapture::ShowCaptureOutputProperty()
{
	if( m_captureDeviceIndex < 0 ||
		m_captureDeviceIndex > GetNumOfCaptureDevices()-1 ){
		return false;
	}

	IBaseFilter *pCaptureFilter =
		m_captureDevices[m_captureDeviceIndex]->GetIBaseFilter();
	IPin *pCaptureOut = GetPin( pCaptureFilter,PINDIR_OUTPUT );

	ISpecifyPropertyPages *pSpecify;
	HRESULT hr = pCaptureOut->QueryInterface(
		IID_ISpecifyPropertyPages,(void**)&pSpecify );
	if( !SUCCEEDED(hr) ){
		SetErrorMsg( "キャプチャデバイスのプロパティはありません。" );
		return false;
	}

	CAUUID caGUID;
	pSpecify->GetPages( &caGUID );
	OleCreatePropertyFrame( NULL,0,0,L"setting",1,(IUnknown**)&pCaptureOut,
		caGUID.cElems,caGUID.pElems,0,0,NULL );
	CoTaskMemFree( caGUID.pElems );
	KT_RELEASE( pSpecify );

	KT_RELEASE( pCaptureOut );

	return true;
}

//----------------------------------------------------------
// KtCaptureDevice
// DirectShowのキャプチャデバイスを表すクラス
KtCaptureDevice::KtCaptureDevice(IMoniker *pMoniker)
{
	m_pBaseFilter = NULL;
	m_name[0] = '\0';

	if( pMoniker ){
		// フレンドリ名を得る
		IPropertyBag *pPropBag = NULL;
		pMoniker->BindToStorage( 0,0,IID_IPropertyBag,(void**)&pPropBag );
		VARIANT varName;
		varName.vt = VT_BSTR;
		varName.bstrVal = NULL;
		pPropBag->Read( L"FriendlyName",&varName,0 );
		wcstombs( m_name,varName.bstrVal,sizeof(m_name) );
//		strcpy( varName.bstrVal,m_name );
		SysFreeString( varName.bstrVal );
		KT_RELEASE(pPropBag);

		// モニカをフィルタオブジェクトにバインドする
		pMoniker->BindToObject( 0,0,IID_IBaseFilter,(void**)&m_pBaseFilter );
	}
}

KtCaptureDevice::~KtCaptureDevice()
{
	KT_RELEASE( m_pBaseFilter );
}

