#include <stdio.h>
#include "SndClass.h"
#define xSND_DEBUG	//コンソールメッセージ表示

//-----------------------------------------------------------------------------
// Name: CreateFail() 
// Desc: DirectDrawオブジェクトの生成失敗の処理
//-----------------------------------------------------------------------------
bool SndSystem::CreateFail( LPCTSTR szError )
{
//	MessageBox( hWnd, szError, "ERROR", MB_OK );
	printf("%s\n",szError);
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
	//debugYI こんなとこにファイル名記述しない方がいいような
	char	*szWaveFiles[ NUMSNDFILES ]
		= { "KINZOKU", "BULLET", "PYUU", "MYA", "G_BSH", "MYA", "MYA" }; 
	
	int		nWaveCnt[ NUMSNDFILES ]
		= { WAV_CNT, WAV_CNT, WAV_CNT, 1, 1, 1, 1 };
	bool	bWaveVol[ NUMSNDFILES ]
		= { true, true, false, false, false, false, false };
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
		if( !(pSnd[i]->SetWavFile( lpDS, szWaveFiles[i], nWaveCnt[i], bWaveVol[i])) ){
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

extern int SndPuckPuck;
extern int SndPenPuck;
extern int SndPuckGoal;
extern int SndRacPen;
extern int SndRacPuck;
extern int SndRacOppPen1;
extern int SndRacOppPen2;
#define	INDEXSNDPUCKPUCK	0
#define	INDEXSNDPENPUCK		1
#define	INDEXSNDPUCKGOAL	2
#define	INDEXSNDRACPEN		3
#define	INDEXSNDRACPUCK		4
#define	INDEXSNDRACOPPPEN1	5
#define	INDEXSNDRACOPPPEN2	6
//#define	MINSNDPARAM			0.0		// 音を出す最小のパラメータ数
#define	MAXSNDPARAM			4.0	// グローバル変数の最大値を指定(正規化に使用)
//-----------------------------------------------------------------------------
// Name: CheckMakeSound()
// Desc: グローバルをチェックし、Play()を呼び出し音を出す。
//-----------------------------------------------------------------------------
bool GameSndSystem::CheckMakeSound()
{
	CheckMakeEach( INDEXSNDPUCKPUCK, &SndPuckPuck );
//	CheckMakeEach( INDEXSNDPENPUCK , &SndPenPuck  );
	CheckMakeEach( INDEXSNDPUCKGOAL, &SndPuckGoal );
	CheckMakeEach( INDEXSNDRACPEN  , &SndRacPen   );
	CheckMakeEach( INDEXSNDRACPUCK , &SndRacPuck );
	CheckMakeEach( INDEXSNDRACOPPPEN1  , &SndRacOppPen1 );
	CheckMakeEach( INDEXSNDRACOPPPEN2  , &SndRacOppPen2 );

	return true;
}

//-----------------------------------------------------------------------------
// Name: CheckMakeEach( int suffix, int *pSndConunt )
// Desc: 個々のサウンドの処理
//-----------------------------------------------------------------------------
bool GameSndSystem::CheckMakeEach( int suffix, int *pSndConunt )
{
	bool bRet = true;

	if( *pSndConunt > 0 ){
		// volは 最小 -10000 から 最大 0 に範囲変換
		double dSeikiVol = ( (double) *pSndConunt) / MAXSNDPARAM;
		LONG vol = (LONG) ( dSeikiVol * 10000 - 10000 );
#ifdef SND_DEBUG
		printf( "SND_DEBUG: %02d  %d   %d\n", suffix, *pSndConunt, vol);
#endif
		// 各イベントののサウンドを再生する。
		SndFiles *sfTarg = GetSndFile( suffix );
		if( sfTarg != NULL ){
			bRet = sfTarg->Play( vol );	// サウンドがちゃんと読めたときは音を出す。
		} else {
			bRet = false;			// ファイルが読めなかったら、false返す。
		}

		// 各イベントに対応するグローバル変数のクリア
		*pSndConunt = 0;
	}

	return bRet;
}
