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
	pBasicVideo = NULL;

	pCapture = NULL;
	pGrabberFilter = NULL;
	pGrabber = NULL;

	dwRegister = NULL;
}
//---------------------------------------------------------------------------

//Com�C���^�[�t�F�C�X���J������
void TForm1::CleanUp()
{
	KT_RELEASE( pGrabber );
	KT_RELEASE( pGrabberFilter );
	KT_RELEASE( pBasicVideo );
	KT_RELEASE( pVidWin );
	KT_RELEASE( pMediaControl );
	KT_RELEASE( pCapture );
	RemoveFromRot( dwRegister ); // GraphEdit�Ō��邽�߂̂��܂��Ȃ�
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

IPin* TForm1::GetPin( IBaseFilter *pFilter,PIN_DIRECTION PinDir )
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

void TForm1::PlayStart()
{
	wchar_t filename[256];
	HRESULT hr;

//	if( OpenDialog1->Execute() ){
//		OpenDialog1->FileName.WideChar( filename,256 );

		// �t�B���^�O���t�}�l�[�W���̍쐬
		//   IGraphBuilder�ɂ́A�t�B���^�����ׂč폜�Ƃ����֐��͂Ȃ��悤�Ȃ̂ŁA
		//   ���ׂď�������ɐV�K�ɍ��B
		//   �������Ȃ��Ɖߋ���RenderFile�����f���������ɕʃE�B���h�E�ŗ����B
		CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
			IID_IGraphBuilder,(void**)&pGraph );
		if( !pGraph ) Application->MessageBox( "pGraph is NULL ","",MB_OK );

		// GraphEdit�A�v���Ō��邽�߂̂��܂��Ȃ�
		AddToRot( pGraph,&dwRegister );

		// IMediaControl�͍Đ��E��~�ɗ��p�B
		pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
		// IVideoWindow�̓r�f�I�\����ʂ̐ݒ�ɗ��p�B
		pGraph->QueryInterface( IID_IVideoWindow,(void**)&pVidWin );
		// IBasicVideo�̓r�f�I�̏c���̑傫���𓾂�̂ɗ��p�B
		pGraph->QueryInterface( IID_IBasicVideo,(void**)&pBasicVideo );

		// �L���v�`���O���t�r���_�̍쐬
		CoCreateInstance( CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC,
			IID_ICaptureGraphBuilder2,(void**)&pCapture );
		pCapture->SetFiltergraph( pGraph );

		// �V�X�e���f�o�C�X�񋓎q�̍쐬
		ICreateDevEnum *pDevEnum = NULL;
		CoCreateInstance( CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,
			IID_ICreateDevEnum,(void**)&pDevEnum );
		// �r�f�I�L���v�`���f�o�C�X�̗񋓎q�̍쐬
		//   �ǂ����r�f�I�L���v�`���f�o�C�X������Ȃ����Ă��Ȃ���
		//   �񋓎q�̍쐬���̂����s������ۂ��B
		IEnumMoniker *pClassEnum = NULL;
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,&pClassEnum,0 );
		if( S_FALSE == hr ){
			Application->MessageBox(
				"VideoInputDeviceCategory�����݂��܂���B","error",MB_OK );
			CleanUp();
			return;
		}

		// �r�f�I�L���v�`���f�o�C�X���
		ULONG cFetched;
		IMoniker *pMoniker = NULL;
		IBaseFilter *pCaptureFilter = NULL;
		if( pClassEnum->Next( 1,&pMoniker,&cFetched ) == S_OK ){
			// �t�����h�����𓾂�
			IPropertyBag *pPropBag = NULL;
			pMoniker->BindToStorage( 0,0,IID_IPropertyBag,(void**)&pPropBag );
			VARIANT varName;
			varName.vt = VT_BSTR;
			varName.bstrVal = NULL;
			pPropBag->Read( L"FriendlyName",&varName,0 );
			Application->MessageBox( AnsiString(varName.bstrVal).c_str(),"FriendlyName",MB_OK );
			SysFreeString( varName.bstrVal );
			KT_RELEASE(pPropBag);

			// �ŏ��̃��j�J���t�B���^�I�u�W�F�N�g�Ƀo�C���h����
//if( S_OK != (hr =
			pMoniker->BindToObject( 0,0,IID_IBaseFilter,(void**)&pCaptureFilter );
//))	ResultMBox( hr,"POINT" );
			KT_RELEASE( pMoniker );
		}
		KT_RELEASE( pClassEnum );
		KT_RELEASE( pDevEnum );

		if( !pCaptureFilter ){
			Application->MessageBox(
				"�r�f�I�L���v�`���f�o�C�X���񋓂���܂���ł����B","error",MB_OK );
		}

		// �L���v�`���t�B���^���O���t�ɒǉ�
		pGraph->AddFilter( pCaptureFilter,L"Video Capture" );

		// �t�@�C���������݃t�B���^�̐ݒ�
		IBaseFilter *ppf = NULL; //�}���`�v���N�T
		IFileSinkFilter *pSink = NULL; // �t�@�C�����C�^
		pCapture->SetOutputFileName( &kt_MEDIASUBTYPE_Avi,
			L"D:\\test.avi",&ppf,&pSink );
//    	pGraph->AddFilter( ppf,L"FileSinkFilter" );

/*
		//�r�f�I�����_�����O�t�B���^�̍쐬
		IBaseFilter *pVideoRenderingFilter;
hr=		CoCreateInstance( CLSID_VideoRenderer,NULL,CLSCTX_INPROC,
			IID_IBaseFilter,(void**)&pVideoRenderingFilter );
ResultMBox( hr,"VideoRenderer" );
		pGraph->AddFilter( pVideoRenderingFilter,L"VideoRenderingFilter" );
		KT_RELEASE( pVideoRenderingFilter );
*/

		// SampleGrabber�t�B���^�̍쐬
		AM_MEDIA_TYPE mt;
		CoCreateInstance( kt_CLSID_SampleGrabber,NULL,CLSCTX_INPROC_SERVER,
			IID_IBaseFilter,(void**)&pGrabberFilter );
		pGrabberFilter->QueryInterface( kt_IID_ISampleGrabber,(void**)&pGrabber );
		ZeroMemory( &mt,sizeof(AM_MEDIA_TYPE) );
		mt.majortype = kt_MEDIATYPE_Video;
		mt.subtype = kt_MEDIASUBTYPE_RGB24;
		mt.formattype = kt_FORMAT_VideoInfo;
		pGrabber->SetMediaType( &mt );
		// �O���t�ɒǉ�
		pGraph->AddFilter( pGrabberFilter,L"SampleGrabber" );
		// �T���v�����o�b�t�@�ɃR�s�[����悤�ɐݒ�
		pGrabber->SetBufferSamples(TRUE);

		// SmartTee�t�B���^�̍쐬
		IBaseFilter *pSmartTee;
		CoCreateInstance( CLSID_SmartTee,NULL,CLSCTX_INPROC,
			IID_IBaseFilter,(void**)&pSmartTee );
		pGraph->AddFilter( pSmartTee,L"SmartTee" );

		// �O���t���\�z����

		// USB�J������SampleGrabber��ڑ�����B
		IPin *pCaptureOut = GetPin( pCaptureFilter,PINDIR_OUTPUT );
if(!pCaptureOut) ResultMBox( E_FAIL,"pCaptureOut" );
		IPin *pGrabberIn = GetPin( pGrabberFilter,PINDIR_INPUT );
if(!pGrabberIn) ResultMBox( E_FAIL,"pGrabberIn" );
		IPin *pGrabberOut = GetPin( pGrabberFilter,PINDIR_OUTPUT );
if(!pGrabberOut) ResultMBox( E_FAIL,"pGrabberFilter" );
		IPin *pSmartTeeIn = GetPin( pSmartTee,PINDIR_INPUT );
if(!pSmartTeeIn) ResultMBox( E_FAIL,"pSmartTeeIn" );
		pGraph->Connect( pCaptureOut,pGrabberIn );
		pGraph->Connect( pGrabberOut,pSmartTeeIn );
		// Render
//		pGraph->Render( pGrabberOut );
		KT_RELEASE( pCaptureOut );
		KT_RELEASE( pGrabberIn );
		KT_RELEASE( pGrabberOut );
		KT_RELEASE( pSmartTeeIn );
		KT_RELEASE( pGrabberFilter );
		KT_RELEASE( pSmartTee );

//		pGraph->RenderFile( filename,NULL );
/*
		pCapture->RenderStream( &kt_PIN_CATEGORY_CAPTURE,&kt_MEDIATYPE_Video,
			pCaptureFilter,NULL,ppf );
		pCapture->RenderStream( &kt_PIN_CATEGORY_PREVIEW,&kt_MEDIATYPE_Video,
			pCaptureFilter,NULL,NULL );
*/
//		pCapture->RenderStream( NULL,NULL,
//			pCaptureFilter,NULL,ppf );
		pCapture->RenderStream( NULL,NULL,
			pCaptureFilter,NULL,NULL );

		KT_RELEASE( ppf );
		KT_RELEASE( pSink );

		REFERENCE_TIME rtStart=0, rtStop=100000000;
//		pCapture->ControlStream( &kt_PIN_CATEGORY_CAPTURE,NULL,NULL,
//			&rtStart,&rtStop,0,0 );

		// �I�[�f�B�I�݂̂��𔻒f
		if( !IsAudioOnly() ){
/*
			// �r�f�I�E�B���h�E���\�z����
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
*/
			// �r�f�I�̑傫���𓾂āA�E�B���h�E�T�C�Y��ύX
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


			// �|�W�V�������Z�b�g
			pVidWin->SetWindowPosition( 0,0,ClientWidth,ClientHeight );
		}

		// �O���t�����s����
		pMediaControl->Run();

/*
		// �L���v�`���f�o�C�X�̃v���p�e�B�y�[�W���J��
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

		// �L���v�`���f�o�C�X����o�Ă���s����񋓂��ăv���p�e�B�y�[�W���J��
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
*/

		/*		IAMVfwCaptureDialogs *pCaptureDialogs = NULL;
hr=		pCapture->FindInterface( &kt_PIN_CATEGORY_CAPTURE,&kt_MEDIATYPE_Video,
			pCaptureFilter,IID_IAMVfwCaptureDialogs,(void**)&pCaptureDialogs );
ResultMBox(hr,"CaptureDialog");
		if( pCaptureDialogs ){
			Application->MessageBox("Success pCaptureDialogs","",MB_OK);
			KT_RELEASE( pCaptureDialogs );
		}
*/
		KT_RELEASE( pCaptureFilter );
//	}
}

void TForm1::PlayStop()
{
	// �Đ��̒�~
	if( pMediaControl ){
		pMediaControl->Stop();
	}
	if( pVidWin ){
		pVidWin->put_Visible(OAFALSE); //�Y���Ɖ�ʕ`�悪���������Ȃ�
		pVidWin->put_Owner(NULL); //�Y���ƃE�B���h�E�Ƀ��b�Z�[�W�����葱������
	}

	// �C���^�[�t�F�C�X���J��
	CleanUp();
}

// �t�@�C�����I�[�f�B�I�݂̂����`�F�b�N
// playwnd�T���v����void CheckVisibility()���Q�l�ɂ����B
bool TForm1::IsAudioOnly()
{
	// �I�[�f�B�I�݂̂̃t�@�C����ǂݍ��񂾂Ƃ���
	// VideoWindow�C���^�[�t�F�C�X�����݂��Ȃ����Ƃ�������ۂ��B
	if( !pVidWin ) return true;

	// VideoWindow�C���^�[�t�F�C�X�����݂���Ƃ��ł�
	// �I�[�f�B�I�݂̂̃t�@�C���̓r�f�I�����_�����ڑ�����Ȃ��̂�
	// �����o�֐����Ă�ł� VFW_E_NOT_CONNECTED ��Ԃ��B
	// (�ǋL)E_NOINTERFACE ��Ԃ����Ƃ�����悤���B
	// �����ł͓K����get_Width�֐����Ă�ł݂�B
	long temp;
	HRESULT hr = pVidWin->get_Width( &temp );
	return hr == VFW_E_NOT_CONNECTED || hr == E_NOINTERFACE;

/*	// �{���͂��̂悤�ɃG���[���e���`�F�b�N���������ǂ��Ǝv���B
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


void __fastcall TForm1::CaptureC1Click(TObject *Sender)
{
	if( !pGrabber ) return;

	AM_MEDIA_TYPE MediaType;
	pGrabber->GetConnectedMediaType( &MediaType );

	// �r�f�I�w�b�_�[�ւ̃|�C���^���擾����
	VIDEOINFOHEADER *pVideoHeader = (VIDEOINFOHEADER*)MediaType.pbFormat;

	// �r�f�I�w�b�_�[�ɂ̓r�b�g�}�b�v��񂪊܂܂��B
	// �r�b�g�}�b�v���� BITMAPINFO �\���̂ɃR�s�[����B
	BITMAPINFO BitmapInfo;
	ZeroMemory( &BitmapInfo,sizeof(BitmapInfo) );
	CopyMemory( &BitmapInfo.bmiHeader,&(pVideoHeader->bmiHeader),
		sizeof(BITMAPINFOHEADER) );

	// �r�b�g�}�b�v�w�b�_�[����DIB���쐬���A�o�b�t�@�ւ̃|�C���^�𓾂�B
	void *buffer = NULL;
	HBITMAP hBitmap = CreateDIBSection( 0,&BitmapInfo,DIB_RGB_COLORS,
		&buffer,NULL,0 );

	if( !hBitmap ) ResultMBox( E_FAIL,"CreateDIBSection" );
	if( !buffer ) ResultMBox( E_FAIL,"buffer" );

	// �C���[�W���o�b�t�@�ɃR�s�[����
	long bufferSize = 0;
	HRESULT hr = pGrabber->GetCurrentBuffer( &bufferSize,(long*)buffer );
    if( hr != S_OK ) ResultMBox( hr,"GetCurrentBuffer" );

	// �T�C�Y�𓾂�
	long width = pVideoHeader->bmiHeader.biWidth;
	long height = pVideoHeader->bmiHeader.biHeight;

	// BitBuilt�ŕ`��
	//HDC hdcSrc = CreateCompatibleDC( Form1->Image1->Canvas->Handle );
	HDC hdcSrc = CreateCompatibleDC( NULL );
	HGDIOBJ hobj;
	hobj = SelectObject( hdcSrc,hBitmap );
	BitBlt( Form1->Image1->Canvas->Handle,0,0,width,height,hdcSrc,0,0,SRCCOPY );
	Image1->Repaint();

	// �r�b�g�}�b�v�����
	SelectObject( hdcSrc,hobj );
	DeleteDC( hdcSrc );
	DeleteObject( hBitmap );

//	Application->MessageBox( "Capture!","",MB_OK );
}
//---------------------------------------------------------------------------

