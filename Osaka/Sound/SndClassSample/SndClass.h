#include <windowsx.h>
#include <dsound.h>
#include "dsutil.h"

#define	NUMSNDFILES	5
#define	WAV_CNT	10

extern	HWND	GetHWnd( const char *windowname );

class SndFiles
{
	HSNDOBJ		hSo;

	public:
	SndFiles()
	{
		hSo = NULL;
	}
	// 後処理(SndSystem::~SndSystemより呼ばれる)
	~SndFiles()
	{
		if( hSo != NULL )	SndObjDestroy( hSo );
	}
	// WAVEファイルのロード等の初期化(SndSystem::Initより呼ばれる)
	bool	SetWavFile( IDirectSound *pDS, LPCTSTR lpName, int iConcurrent )
	{
		hSo = SndObjCreate( pDS, lpName, iConcurrent );
		if( hSo == NULL ) return false;
		return true;
	}
	// サウンドの再生 vol は、-10000(DSBVOLUME_MIN)～0(DSBVOLUME_MAX)
	bool	Play( LONG vol )
	{
		if( SndObjPlay( hSo, vol, 0 ) ) return true;
		return false;
	}
	// 再生中のサウンドの中止
	bool	Stop()
	{
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

