#include <string.h>
#include "KtDShow.h"
// �K�v�ŏ����̃G���[�`�F�b�N�������Ȃ��o�[�W����

//----------------------------------------------------------
// KtDShowBasic
// DirectShow�̃t�@�C���Đ����s���N���X
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
	// ���łɃt�B���^�O���t�}�l�[�W�������݂��Ă�����AClose����B
	if( m_pGraph ){
		Close();
	}

	// �t�B���^�O���t�}�l�[�W���̍쐬
	//   IGraphBuilder�ɂ́A�t�B���^�����ׂč폜�Ƃ����֐��͂Ȃ��悤�Ȃ̂ŁA
	//   ���ׂď�������ɐV�K�ɍ��B
	//   �������Ȃ��Ɖߋ���RenderFile�����f���������ɕʃE�B���h�E�ŗ����B
	CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
		IID_IGraphBuilder,(void**)&m_pGraph );
	if( !m_pGraph ){
		m_errorMsg = "�t�B���^�O���t�}�l�[�W�����쐬�ł��܂���ł����B";
		return false;
	}

	if( m_bAddToRot ){
		AddToRot();
	}

	// IMediaControl�͍Đ��E��~�ɗ��p�B
	m_pGraph->QueryInterface( IID_IMediaControl,(void**)&m_pMediaControl );
	// IVideoWindow�̓r�f�I�\����ʂ̐ݒ�ɗ��p�B
	m_pGraph->QueryInterface( IID_IVideoWindow,(void**)&m_pVidWin );
	// IBasicVideo�̓r�f�I�̏c���̑傫���𓾂�̂ɗ��p�B
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
		// �\������ʒu���N���C�A���g�̈�ɐݒ�
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

	// �O����������
	if( !Open() ){
		return false;
	}

//	if( !m_pGraph || !m_pVidWin ){
	if( !m_pGraph ){
		return false;
	}

	// �t�B���^�O���t���\�z
	hr = m_pGraph->RenderFile( m_inputFilename,NULL );
	if( hr != S_OK ){
		m_errorMsg = GetResultErrorString( hr );
		return false;
	}

	return true;
}

bool KtDShowBasic::HaveVideoData()
{
	// �I�[�f�B�I�݂̂̃t�@�C����ǂݍ��񂾂Ƃ���
	// VideoWindow�C���^�[�t�F�C�X�����݂��Ȃ����Ƃ�������ۂ��B
	if( !m_pVidWin ){
		return false;
	}

	// VideoWindow�C���^�[�t�F�C�X�����݂���Ƃ��ł�
	// �I�[�f�B�I�݂̂̃t�@�C���̓r�f�I�����_�����ڑ�����Ȃ��̂�
	// �����o�֐����Ă�ł� VFW_E_NOT_CONNECTED ��Ԃ��B
	// (�ǋL)E_NOINTERFACE ��Ԃ����Ƃ�����悤���B
	// �����ł͓K����get_Width�֐����Ă�ł݂�B
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
		m_errorMsg = "Open����Ă��Ȃ���Ԃ�Run�����s����܂����B";
		return false;
	}

	// �f���f�[�^�������Ă���ꍇ
	if( HaveVideoData() ){
		// �I�[�i�[�E�B���h�E��ݒ�
		hr = m_pVidWin->put_Owner( (OAHWND)m_hOutputWindow );
		if( hr != S_OK ){
			m_errorMsg = GetResultErrorString( hr );
			return false;
		}

		// OutputWindow���w�肳�ꂽ�ꍇ�̓E�B���h�E�X�^�C����ݒ�
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

	// ���łɓo�^�ς݂̂Ƃ�
	if( m_dwRegister ){
		return true;
	}

	IMoniker *pMoniker;
	IRunningObjectTable *pROT;
	if( FAILED(GetRunningObjectTable(0,&pROT)) ){
		m_errorMsg = "ROT�̎擾�Ɏ��s���܂����B";
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
			return "�I�[�f�B�I�p�̃����_����������܂���ł����B";

		case VFW_S_VIDEO_NOT_RENDERED:
			return "�f���p�̃����_����������܂���ł����B";

		case E_FAIL:
			return "(�����G���[)���s���܂����B";
		
		case E_POINTER:
			return "(�����G���[)NULL�|�C���^����";
		
		case VFW_E_CANNOT_CONNECT:
			return "�ڑ����m�����钆�ԃt�B���^�̑g�ݍ��킹��������܂���ł����B";
		
		case VFW_E_CANNOT_LOAD_SOURCE_FILTER:
			return "�t�@�C���̃\�[�X�t�B���^�����[�h�ł��܂���ł����B";
		
		case VFW_E_INVALID_FILE_FORMAT:
			return "�t�@�C���t�H�[�}�b�g�������ł��B";
		
		case VFW_E_UNSUPPORTED_STREAM:
			return "�t�@�C���̃t�H�[�}�b�g���T�|�[�g����Ă��܂���B";

		default:
			return "�o�^����Ă��Ȃ��G���[�ł��B";
	}
}

//----------------------------------------------------------
// KtDShowCapture
// DirectShow�ŃJ��������L���v�`�����s���N���X
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
	// �J�������
	FindCaptureDevices();
}

KtDShowCapture::~KtDShowCapture()
{
	// �L���v�`���f�o�C�X�z����폜
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

	// �L���v�`���O���t�r���_�̍쐬
	CoCreateInstance( CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&m_pCapture );
	m_pCapture->SetFiltergraph( GetGraphBuilder() );
	
	if( !m_pCapture ){
		SetErrorMsg( "�L���v�`���O���t�}�l�[�W�����쐬�ł��܂���ł����B" );
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
	// �L���v�`���f�o�C�X�z����폜
	while( !m_captureDevices.empty() ){
		delete m_captureDevices.back();
		m_captureDevices.pop_back();
	}
	m_captureDeviceIndex = -1;

	// �V�X�e���f�o�C�X�񋓎q�̍쐬
	ICreateDevEnum *pDevEnum = NULL;
	CoCreateInstance( CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,
		IID_ICreateDevEnum,(void**)&pDevEnum );

	// �r�f�I�L���v�`���f�o�C�X�̗񋓎q�̍쐬
	//   �ǂ����r�f�I�L���v�`���f�o�C�X������Ȃ����Ă��Ȃ���
	//   �񋓎q�̍쐬���̂����s������ۂ��B
	HRESULT hr;
	IEnumMoniker *pClassEnum = NULL;
	hr = pDevEnum->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,&pClassEnum,0 );
	if( S_FALSE == hr ){
		KT_RELEASE( pDevEnum );
		return;
	}

	// �r�f�I�L���v�`���f�o�C�X���
	ULONG cFetched;
	IMoniker *pMoniker = NULL;
	KtCaptureDevice *device;
	while( pClassEnum->Next( 1,&pMoniker,&cFetched ) == S_OK ){
		// KtDShowCaptureDevide���쐬���Ĕz��ɒǉ�
		device = new KtCaptureDevice( pMoniker );
		m_captureDevices.push_back( device );
		KT_RELEASE( pMoniker );
	}
	KT_RELEASE( pClassEnum );
	KT_RELEASE( pDevEnum );

	// �ŏ��̃L���v�`���f�o�C�X��I��
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

	// ���łɃr�b�g�}�b�v�I�u�W�F�N�g�����݂���ꍇ�͍폜
	if( m_hSampleBitmap ){
		DeleteObject( m_hSampleBitmap );
		m_hSampleBitmap = NULL;
	}

	AM_MEDIA_TYPE MediaType;
	m_pGrabber->GetConnectedMediaType( &MediaType );

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
	m_hSampleBitmap = CreateDIBSection( 0,&BitmapInfo,DIB_RGB_COLORS,
		&buffer,NULL,0 );

	// �C���[�W���o�b�t�@�ɃR�s�[����
	long bufferSize = 0;
	m_pGrabber->GetCurrentBuffer( &bufferSize,(long*)buffer );

/*
	// �T�C�Y�𓾂�
	long width = GetVideoWidth();
	long height = GetVideoHeight();

	// BitBuilt�ŕ`��
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
	// ���ł�pCapture������ꍇ��Close����B
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

	// �L���v�`���t�B���^���O���t�ɒǉ�
	IBaseFilter *pCaptureFilter =
		m_captureDevices[m_captureDeviceIndex]->GetIBaseFilter();
	GetGraphBuilder()->AddFilter( pCaptureFilter,L"Video Capture" );

	// �������݃t�@�C�����w�肵�Ă���ꍇ
	IBaseFilter *ppf = NULL; //�}���`�v���N�T
	IFileSinkFilter *pSink = NULL; // �t�@�C�����C�^
	if( m_outputFilename[0] ){
		// �t�@�C���������݃t�B���^�̐ݒ�
//		m_pCapture->SetOutputFileName( &MEDIASUBTYPE_Avi,
//			L"D:\\test.avi",&ppf,&pSink );
	}

	// SmartTee�t�B���^�̍쐬
	IBaseFilter *pSmartTee;
	CoCreateInstance( CLSID_SmartTee,NULL,CLSCTX_INPROC,
		IID_IBaseFilter,(void**)&pSmartTee );
	GetGraphBuilder()->AddFilter( pSmartTee,L"SmartTee" );

	// SmartTee�t�B���^��Preview�s���ɂȂ���
	// �����_���[�ɂȂ�
	if( m_bGetSample ){
		//�T���v���O���o�[���Z�b�g����ꍇ
		// SampleGrabber�t�B���^�̍쐬
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
		// �O���t�ɒǉ�
		GetGraphBuilder()->AddFilter( pGrabberFilter,L"SampleGrabber" );
		// �T���v�����o�b�t�@�ɃR�s�[����悤�ɐݒ�
		m_pGrabber->SetBufferSamples(TRUE);

		// USB�J������SmartTee
		// SmartTee.Preview��SampleGrabber
		// SampleGrabber��VideoRenderer��ڑ�����B
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
		// USB�J������SmartTee ��ڑ�����B
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

	// �c��̃t�B���^�O���t���\�z
	if( m_outputFilename[0] ){
		// �t�@�C���������݂̓t�@�C�������w�肳��Ă���Ƃ��̂�
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
		// �T���v���𓾂�Ƃ��̓o�b�t�@���m��
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
		SetErrorMsg( "�L���v�`���f�o�C�X�̃v���p�e�B�͂���܂���B" );
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
		SetErrorMsg( "�L���v�`���f�o�C�X�̃v���p�e�B�͂���܂���B" );
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
// DirectShow�̃L���v�`���f�o�C�X��\���N���X
KtCaptureDevice::KtCaptureDevice(IMoniker *pMoniker)
{
	m_pBaseFilter = NULL;
	m_name[0] = '\0';

	if( pMoniker ){
		// �t�����h�����𓾂�
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

		// ���j�J���t�B���^�I�u�W�F�N�g�Ƀo�C���h����
		pMoniker->BindToObject( 0,0,IID_IBaseFilter,(void**)&m_pBaseFilter );
	}
}

KtCaptureDevice::~KtCaptureDevice()
{
	KT_RELEASE( m_pBaseFilter );
}

