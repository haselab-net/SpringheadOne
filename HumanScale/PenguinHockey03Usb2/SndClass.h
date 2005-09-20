#include <windowsx.h>
#include <dsound.h>
#include "dsutil.h"

#define	NUMSNDFILES	7
#define	WAV_CNT	30

extern	HWND	GetHWnd( const char *windowname );

class SndFiles
{
	HSNDOBJ		hSo;
	bool		bEnableVol;

	public:
	SndFiles()
	{
		hSo = NULL;
		bEnableVol = true;
	}
	// 後処理(SndSystem::~SndSystemより呼ばれる)
	~SndFiles()
	{
		if( hSo != NULL )	SndObjDestroy( hSo );
	}
	// WAVEファイルのロード等の初期化(SndSystem::Initより呼ばれる)
	bool	SetWavFile( IDirectSound *pDS, LPCTSTR lpName, int iConcurrent, bool bVol )
	{
		hSo = SndObjCreate( pDS, lpName, iConcurrent );
		bEnableVol = bVol;
		if( hSo == NULL ) return false;
		return true;
	}
	// サウンドの再生 vol は、-10000(DSBVOLUME_MIN)～0(DSBVOLUME_MAX)
	bool	Play( LONG vol )
	{
		if( hSo == NULL ) return false;
		if( !bEnableVol ) vol = DSBVOLUME_MAX;	// bEableVolがfalseなら音量は最大値
		if( SndObjPlay( hSo, vol, 0 ) ) return true;
		return false;
	}
	// 再生中のサウンドの中止
	bool	Stop()
	{
		if( hSo == NULL ) return false;
		if( SndObjStop( hSo ) ) return true;
		return false;
	}
};

class SndSystem
{
	HWND	hWnd;			// アプリケーションのWindow Handle
	LPGUID	lpDSGUID;		// デバイスGUID
	LPDIRECTSOUND	lpDS;		// DirectSound object用ポインタ
	SndFiles	*pSnd[NUMSNDFILES];		// サウンドの構造体

	bool CreateFail( LPCTSTR szError );	// エラーメッセージの表示

	public:

	SndSystem();
	// DirectSoundの後処理等
	~SndSystem();
	// 初期化処理(HWNDがわかっている場合)
	bool	Init( HWND hWnd );
	// 初期化処理(glut等、HWNDがわからない場合)
	// Windowの名称よりHWNDを検索する処理追加型
	bool	Init( const char *windowname );
	// suffix番目のサウンドを呼び出す。(Play()やStop()用)
	SndFiles	*GetSndFile( int suffix );
};

class GameSndSystem : public SndSystem
{
	public:
	// グローバルをチェックし、Play()を呼び出し音を出す。
	bool	CheckMakeSound();
	// 個々のサウンドの処理
	bool	CheckMakeEach( int suffix, int *pSndConunt );
};

