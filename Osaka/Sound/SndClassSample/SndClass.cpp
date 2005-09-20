#include "SndClass.h"

//-----------------------------------------------------------------------------
// Name: CreateFail() 
// Desc: DirectDraw�I�u�W�F�N�g�̐������s�̏���
//-----------------------------------------------------------------------------
bool SndSystem::CreateFail( LPCTSTR szError )
{
	MessageBox( hWnd, szError, "ERROR", MB_OK );
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
	char	*szWaveFiles[ NUMSNDFILES ]
		= { "Mya", "Pashi", "Bullet", "Shibuki", "WinOpen" };
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
		if( !(pSnd[i]->SetWavFile( lpDS, szWaveFiles[i], WAV_CNT)) ){
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

