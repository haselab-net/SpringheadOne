#pragma warning (disable: 4251 4275 4267 4018)
#pragma comment(lib, "Dxguid.lib")

#include "GRSound.h"
#include <Base/BaseDebug.h>

using namespace std;
using namespace Spr;

GRSound GRSound::inst;

GRSound::GRSound()
{
	pDS = NULL;
	pDsbPrimary = NULL;
}


GRSound::~GRSound()
{
	freeDirectSound();
}


HRESULT GRSound::initialize( HWND hwnd )
{
	HRESULT hr;

	if (hwnd==NULL){
		char pszTitle[1024];
		GetConsoleTitle(pszTitle, 1024);
		DSTR << __FILE__ << "(" << __LINE__ << ") : " << pszTitle <<"\n";
		hwnd = FindWindow(NULL, pszTitle);
	}

    SAFE_RELEASE( pDS );

	if( FAILED( hr = DirectSoundCreate8( NULL, &pDS, NULL ) ) ) {
		DSTR << __FILE__ << "(" << __LINE__ << ") : " << "えうぅ、DirectSoundが初期化できません～\n";
		return E_FAIL;
	}

    if( FAILED( hr = pDS->SetCooperativeLevel( hwnd, DSSCL_PRIORITY ) ) ) { //念のため優先協調レベルに
	    if( FAILED( hr = pDS->SetCooperativeLevel( hwnd, DSSCL_NORMAL ) ) ) { 
			DSTR << __FILE__ << "(" << __LINE__ << ") : " << "協調レベルが設定できません～(" << hr << ")\n";
			return E_FAIL;
		}
	}

    if( FAILED( hr = getListener( &pListener ) ) ) {
		DSTR << __FILE__ << "(" << __LINE__ << ") : " << "リ、リスナーがぁっっ(T^T)\n";
        return E_FAIL;
    }

    // Get listener parameters
    //dsListenerParams.dwSize = sizeof(DS3DLISTENER);
    //pListener->GetAllParameters( &g_dsListenerParams );

	WIN32_FIND_DATA finddata;
	char filter[MAX_PATH];
	string filename; 
	HANDLE hFind = NULL;
	int i;
	// string	soundset = IniFile::getValue<string>("Sound", "Set", string("SampleSoundSet"));
	string soundset = string("SoundSet");
	
	DSTR << __FILE__ << "(" << __LINE__ << ") : " << soundset <<"\n";

	for( i=0; i<SOUNDBUFFER_MAX; i++ ) {
		pDsb[i] = NULL;
		sprintf( filter, "%s\\%02d*.wav", soundset.c_str() , i);
		hFind = FindFirstFile( filter, &finddata );
		if (hFind==INVALID_HANDLE_VALUE) continue;

		FindClose( hFind );
		filename = soundset + "\\" + finddata.cFileName;
		if (FAILED( createSoundBuffer(&pDsb[i]) )) continue;
		
		if (FAILED(hr = loadWaveFile( (SOUNDID)i , (TCHAR*)filename.c_str() ) )){
			SAFE_RELEASE(pDsb[i]);
			continue;
		}

		DSTR << "GRSound '" << finddata.cFileName << "' を読み込みました。\n";
	}

	return hr;
}


HRESULT GRSound::play( SOUNDID id )
{
	HRESULT hr;
	if (pDsbPrimary==NULL) return CO_E_NOTINITIALIZED;
	if (pDsb[id]==NULL) return CO_E_NOTINITIALIZED;
	hr = pDsb[id]->SetCurrentPosition(0);
	hr = pDsb[id]->Play(0,0,0);
	return hr;
}

HRESULT GRSound::stop( SOUNDID id )
{
	HRESULT hr;
	if (pDsbPrimary==NULL) return CO_E_NOTINITIALIZED;
	if (pDsb[id]==NULL) return CO_E_NOTINITIALIZED;
	hr = pDsb[id]->Stop();
	return hr;
}

//-----------------------------------------------------------------------------
// セカンダリバッファを作成
//-----------------------------------------------------------------------------
HRESULT GRSound::createSoundBuffer( LPDIRECTSOUNDBUFFER8* ppDsb8 ) 
{ 
  WAVEFORMATEX wfx; 
  DSBUFFERDESC dsbdesc; 
  LPDIRECTSOUNDBUFFER pDSB = NULL;
  HRESULT hr; 
 
  // WAV フォーマットの構造体をセットアップする。
  memset( &wfx, 0, sizeof(WAVEFORMATEX) ); 
  wfx.wFormatTag = WAVE_FORMAT_PCM; 
  wfx.nChannels = 1;
  wfx.nSamplesPerSec = 22050; 
  wfx.nBlockAlign = 2; 
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 
  wfx.wBitsPerSample = 16; 
 
  // DSBUFFERDESC 構造体をセットアップする。
  memset( &dsbdesc, 0, sizeof(DSBUFFERDESC) ); 
  dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
  dsbdesc.dwFlags = DSBCAPS_CTRL3D; //DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; //
  dsbdesc.dwBufferBytes = 3 * wfx.nAvgBytesPerSec; 
  dsbdesc.lpwfxFormat = &wfx; 
 
  // バッファを作成する。
  hr = pDS->CreateSoundBuffer(&dsbdesc, &pDSB, NULL); 
  if (SUCCEEDED(hr)) { 
     hr = pDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) ppDsb8);
	 pDSB->Release();
  } 
  return hr;
} 

//-----------------------------------------------------------------------------
// プライマリバッファを作成、リスナーを取得
//-----------------------------------------------------------------------------
HRESULT GRSound::getListener( LPDIRECTSOUND3DLISTENER* ppListener )
{
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;

    if( ppListener == NULL ) return E_INVALIDARG;
    if( pDS == NULL ) return CO_E_NOTINITIALIZED;

    *ppListener = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

    if( FAILED( hr = pDS->CreateSoundBuffer( &dsbdesc, &pDsbPrimary, NULL ) ) ) return E_FAIL;

    if( FAILED( hr = pDsbPrimary->QueryInterface( IID_IDirectSound3DListener, (VOID**)ppListener ) ) ) {
        SAFE_RELEASE( pDsbPrimary );
        return E_FAIL;
    }

    return S_OK;
}

// 柔軟性にかける。エラー対策が皆無に等しい。
HRESULT GRSound::loadWaveFile( SOUNDID id ,TCHAR* strFileName )
{
    HRESULT hr;
    HMMIO	hmmio;
	MMCKINFO        ckRiff;           // chunk info. for general use.
    MMCKINFO		ck;

    if( pDS == NULL ) return CO_E_NOTINITIALIZED;

	if( strFileName == NULL ) return E_INVALIDARG;

	hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );
	if( hmmio == NULL ) {
		DSTR << __FILE__ << "(" << __LINE__ << ") : " << "Waveが開けなかったよぉ\n";
		return E_FAIL;
	}

	if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ckRiff, NULL, 0 ) ){
		mmioClose( hmmio, 0 );
		return E_FAIL;
	}

	// 'data' チャンクを探す
    ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( MMSYSERR_NOERROR != mmioDescend( hmmio, &ck, &ckRiff, MMIO_FINDCHUNK ) ){
		mmioClose( hmmio, 0 );
		return E_FAIL;
	}

	if( pDsb[id] == NULL ) return CO_E_NOTINITIALIZED;
	//DSTR << "pDsb:" << pDsb[id] << "\n";
	//DSTR << "cksize:" << ck.cksize << "\n";

	VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

	// Lock the buffer down
    if( FAILED( hr = pDsb[id]->Lock( 0, ck.cksize, 
                                 &pDSLockedBuffer, &dwDSLockedBufferSize, 
								 NULL, NULL, 0L ) ) )
	{
		mmioClose( hmmio, 0 );
		return E_FAIL;
	}

    if( ck.cksize <= dwDSLockedBufferSize ){
		dwWavDataRead = mmioRead( hmmio, (CHAR*)pDSLockedBuffer, ck.cksize);
        FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
					dwDSLockedBufferSize - dwWavDataRead, 
                    0);
	}else{
		mmioRead( hmmio, (CHAR*)pDSLockedBuffer, dwDSLockedBufferSize );
	}

    pDsb[id]->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    mmioAscend( hmmio, &ck, 0 );
	mmioClose( hmmio, 0 );
	return S_OK;
}


HRESULT GRSound::freeDirectSound()
{
	SAFE_RELEASE(pDS);
//	DSTR << __FILE__ << "(" << __LINE__ << ") : " << "DirectSound解放しましたっ\n";
	return S_OK;
}
