#include <stdio.h>
#include "SndClass.h"
#define xSND_DEBUG	//�R���\�[�����b�Z�[�W�\��

//-----------------------------------------------------------------------------
// Name: CreateFail() 
// Desc: DirectDraw�I�u�W�F�N�g�̐������s�̏���
//-----------------------------------------------------------------------------
bool SndSystem::CreateFail( LPCTSTR szError )
{
//	MessageBox( hWnd, szError, "ERROR", MB_OK );
	printf("%s\n",szError);
	return false;
}

//-----------------------------------------------------------------------------
// Name: Init( const char *windowname )
// Desc: Direct Sound �I�u�W�F�N�g�̐�����
//-----------------------------------------------------------------------------
bool SndSystem::Init( const char *windowname )
{
	HWND hWnd = GetHWnd( windowname );
	return Init( hWnd );
}

//-----------------------------------------------------------------------------
// Name: Init( HWND hInWnd )
// Desc: Direct Sound �I�u�W�F�N�g�̐�����
//-----------------------------------------------------------------------------
bool SndSystem::Init( HWND hInWnd )
{
	//debugYI ����ȂƂ��Ƀt�@�C�����L�q���Ȃ����������悤��
	char	*szWaveFiles[ NUMSNDFILES ]
		= { "KINZOKU", "BULLET", "PYUU", "MYA", "G_BSH", "MYA", "MYA" }; 
	
	int		nWaveCnt[ NUMSNDFILES ]
		= { WAV_CNT, WAV_CNT, WAV_CNT, 1, 1, 1, 1 };
	bool	bWaveVol[ NUMSNDFILES ]
		= { true, true, false, false, false, false, false };
	HRESULT	dsrval;

	hWnd = hInWnd;

	// DirectSound�I�u�W�F�N�g�̍쐬
	dsrval = DirectSoundCreate(lpDSGUID, &lpDS, NULL);
	if(dsrval != DS_OK)	return CreateFail("DirectSoundCreate Failed");
	OutputDebugString("DirectSoundCreate OK\n");

	// DirectSound�������x���̐ݒ�
	dsrval = lpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(dsrval != DS_OK){
//		DSRelease();
		return CreateFail("DirectSound SetCooperativeLevel Failed");
	}
	OutputDebugString( "DirectSound SetCooperativeLevel OK\n" );

	//�Q�[���Ŏg�p����v�`�u�t�@�C����ǂݍ���
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
// Desc: WAV�t�@�C���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
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
// Desc: SndSystem�̏����l�̑��
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
// Desc: Direct Sound �I�u�W�F�N�g�̍폜
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
//#define	MINSNDPARAM			0.0		// �����o���ŏ��̃p�����[�^��
#define	MAXSNDPARAM			4.0	// �O���[�o���ϐ��̍ő�l���w��(���K���Ɏg�p)
//-----------------------------------------------------------------------------
// Name: CheckMakeSound()
// Desc: �O���[�o�����`�F�b�N���APlay()���Ăяo�������o���B
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
// Desc: �X�̃T�E���h�̏���
//-----------------------------------------------------------------------------
bool GameSndSystem::CheckMakeEach( int suffix, int *pSndConunt )
{
	bool bRet = true;

	if( *pSndConunt > 0 ){
		// vol�� �ŏ� -10000 ���� �ő� 0 �ɔ͈͕ϊ�
		double dSeikiVol = ( (double) *pSndConunt) / MAXSNDPARAM;
		LONG vol = (LONG) ( dSeikiVol * 10000 - 10000 );
#ifdef SND_DEBUG
		printf( "SND_DEBUG: %02d  %d   %d\n", suffix, *pSndConunt, vol);
#endif
		// �e�C�x���g�̂̃T�E���h���Đ�����B
		SndFiles *sfTarg = GetSndFile( suffix );
		if( sfTarg != NULL ){
			bRet = sfTarg->Play( vol );	// �T�E���h�������Ɠǂ߂��Ƃ��͉����o���B
		} else {
			bRet = false;			// �t�@�C�����ǂ߂Ȃ�������Afalse�Ԃ��B
		}

		// �e�C�x���g�ɑΉ�����O���[�o���ϐ��̃N���A
		*pSndConunt = 0;
	}

	return bRet;
}
