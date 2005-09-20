#include "SndClass.h"

//-----------------------------------------------------------------------------
// Name: CreateFail() 
// Desc: DirectDrawオブジェクトの生成失敗の処理
//-----------------------------------------------------------------------------
bool SndSystem::CreateFail( LPCTSTR szError )
{
	MessageBox( hWnd, szError, "ERROR", MB_OK );
	return false;
}

//-----------------------------------------------------------------------------
// Name: Init( const char *windowname )
// Desc: Direct Sound オブジェクトの生成等
//-----------------------------------------------------------------------------
bool SndSystem::Init( const char *windowname )
{
	HWND hWnd = GetHWnd( windowname );
	return Init( hWnd );
}

//-----------------------------------------------------------------------------
// Name: Init( HWND hInWnd )
// Desc: Direct Sound オブジェクトの生成等
//-----------------------------------------------------------------------------
bool SndSystem::Init( HWND hInWnd )
{
	char	*szWaveFiles[ NUMSNDFILES ]
		= { "Mya", "Pashi", "Bullet", "Shibuki", "WinOpen" };
	HRESULT	dsrval;

	hWnd = hInWnd;

	// DirectSoundオブジェクトの作成
	dsrval = DirectSoundCreate(lpDSGUID, &lpDS, NULL);
	if(dsrval != DS_OK)	return CreateFail("DirectSoundCreate Failed");
	OutputDebugString("DirectSoundCreate OK\n");

	// DirectSound協調レベルの設定
	dsrval = lpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(dsrval != DS_OK){
//		DSRelease();
		return CreateFail("DirectSound SetCooperativeLevel Failed");
	}
	OutputDebugString( "DirectSound SetCooperativeLevel OK\n" );

	//ゲームで使用するＷＡＶファイルを読み込む
	for( int i=0; i< NUMSNDFILES; i++ ){
		pSnd[i] = new SndFiles;
		if( !(pSnd[i]->SetWavFile( lpDS, szWaveFiles[i], WAV_CNT)) ){
			return CreateFail("Wave Resouce Load Error!!\n");
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Name: GetSndFile( int suffix )
// Desc: WAVファイルオブジェクトへのポインタを返す
//-----------------------------------------------------------------------------
SndFiles *SndSystem::GetSndFile( int suffix )
{
	if( suffix >= 0 && suffix < NUMSNDFILES ){
		return pSnd[ suffix ];
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Name: SndSystem()
// Desc: SndSystemの初期値の代入
//-----------------------------------------------------------------------------
SndSystem::SndSystem()
{
	hWnd = NULL;
	lpDSGUID = NULL;
	lpDS = NULL;
	for( int i=0; i< NUMSNDFILES; i++ ){
		pSnd[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: ~SndSystem()
// Desc: Direct Sound オブジェクトの削除
//-----------------------------------------------------------------------------
SndSystem::~SndSystem()
{
	for( int i=0; i< NUMSNDFILES; i++ ){
		if( pSnd[i] != NULL ) delete( pSnd[i] );
	}
	if( lpDS != NULL )	lpDS->Release();

	OutputDebugString( "Release Called!!\n" );
}

