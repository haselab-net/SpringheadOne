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
	// �㏈��(SndSystem::~SndSystem���Ă΂��)
	~SndFiles()
	{
		if( hSo != NULL )	SndObjDestroy( hSo );
	}
	// WAVE�t�@�C���̃��[�h���̏�����(SndSystem::Init���Ă΂��)
	bool	SetWavFile( IDirectSound *pDS, LPCTSTR lpName, int iConcurrent, bool bVol )
	{
		hSo = SndObjCreate( pDS, lpName, iConcurrent );
		bEnableVol = bVol;
		if( hSo == NULL ) return false;
		return true;
	}
	// �T�E���h�̍Đ� vol �́A-10000(DSBVOLUME_MIN)�`0(DSBVOLUME_MAX)
	bool	Play( LONG vol )
	{
		if( hSo == NULL ) return false;
		if( !bEnableVol ) vol = DSBVOLUME_MAX;	// bEableVol��false�Ȃ特�ʂ͍ő�l
		if( SndObjPlay( hSo, vol, 0 ) ) return true;
		return false;
	}
	// �Đ����̃T�E���h�̒��~
	bool	Stop()
	{
		if( hSo == NULL ) return false;
		if( SndObjStop( hSo ) ) return true;
		return false;
	}
};

class SndSystem
{
	HWND	hWnd;			// �A�v���P�[�V������Window Handle
	LPGUID	lpDSGUID;		// �f�o�C�XGUID
	LPDIRECTSOUND	lpDS;		// DirectSound object�p�|�C���^
	SndFiles	*pSnd[NUMSNDFILES];		// �T�E���h�̍\����

	bool CreateFail( LPCTSTR szError );	// �G���[���b�Z�[�W�̕\��

	public:

	SndSystem();
	// DirectSound�̌㏈����
	~SndSystem();
	// ����������(HWND���킩���Ă���ꍇ)
	bool	Init( HWND hWnd );
	// ����������(glut���AHWND���킩��Ȃ��ꍇ)
	// Window�̖��̂��HWND���������鏈���ǉ��^
	bool	Init( const char *windowname );
	// suffix�Ԗڂ̃T�E���h���Ăяo���B(Play()��Stop()�p)
	SndFiles	*GetSndFile( int suffix );
};

class GameSndSystem : public SndSystem
{
	public:
	// �O���[�o�����`�F�b�N���APlay()���Ăяo�������o���B
	bool	CheckMakeSound();
	// �X�̃T�E���h�̏���
	bool	CheckMakeEach( int suffix, int *pSndConunt );
};

