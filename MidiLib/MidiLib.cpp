#include "MidiLib.h"

//	define

#define LOC_MID_NOTEON			0x90
#define LOC_MID_NOTEOFF			0x80
#define LOC_MID_POLYKEYPRESSURE		0xA0
#define LOC_MID_CONTROLCHANGE		0xB0
#define LOC_MID_PROGRAMCHANGE		0xC0
#define LOC_MID_CHANNELPRESSUREE	0xD0
#define LOC_MID_PITCHBENDCHANGE		0xE0
#define LOC_MID_SYSTEMMESSAGE		0xF0

#define LOC_MID_MNOTEON(x)		(0x90+(x))
#define LOC_MID_MNOTEOFF(x)		(0x80+(x))
#define LOC_MID_MPOLYKEYPRESSURE(x)	(0xA0+(x))
#define LOC_MID_MCONTROLCHANGE(x)	(0xB0+(x))
#define LOC_MID_MPROGRAMCHANGE(x)	(0xC0+(x))
#define LOC_MID_MCHANNELPRESSUREE(x)	(0xD0+(x))
#define LOC_MID_MPITCHBENDCHANGE(x)	(0xE0+(x))
#define LOC_MID_MSYSTEMMESSAGE(x)	(0xF0+(x))

void MidiLib::SetDevice( const char *pszDeviceName )
{
	UINT	nRet;
	UINT	nDevNum;
	UINT	nLoop;
	MIDIOUTCAPS	moc;

	nDevNum = ::midiOutGetNumDevs();	// MIDI�f�o�C�X�̐��̎擾
	if( nDevNum == 0 ){
		bMidiOff = true;				// MIDI�͎g��Ȃ� (�g���Ȃ��͂�)
		return;
	}

	if( pszDeviceName == NULL ){	// �k���Ȃ�
		nMidiDevice = MIDI_MAPPER;	// MIDI MAPPER���g���B
		return;
	}

	if( *pszDeviceName == '\0' ){	// ����0�̕����� ������k��������Ȃ�
		nMidiDevice = MIDI_MAPPER;	// MIDI MAPPER���g���B
		return;
	}

	for( nLoop = 0; nLoop < nDevNum; nLoop++ ){
		nRet = ::midiOutGetDevCaps( nLoop, &moc, sizeof( MIDIOUTCAPS ));
		if( nRet != MMSYSERR_NOERROR ){	// �擾�G���[��
			ErrorDisp( nRet );
			continue;
		}
		if( !::lstrcmp( moc.szPname, pszDeviceName ) ){	// �w�肵��Device���Ɠ����ꍇ
			nMidiDevice = nLoop;			// MIDI Device�ԍ���nLoop�l������
			return;
		}
	}
	DispString( "�w��f�o�C�X��������܂��� MIDI MAPPER���g�p���܂�\n" );
	nMidiDevice = MIDI_MAPPER;
	return;
}

bool MidiLib::Open()
{
	UINT	uRet;

	if( bMidiOff )	return true;	// MIDI�f�o�C�X��������ΐ���I��(�������Ȃ�)
	if( hMidiOut != NULL ) return false;
		// MIDI�n���h����NULL�łȂ���΂��łɃI�[�v�����Ă���̂ŃG���[

	uRet = ::midiOutOpen( &hMidiOut, nMidiDevice,
		(unsigned long)NULL, (unsigned long)NULL, CALLBACK_NULL );
	if( uRet == MMSYSERR_NOERROR ) return true;		// �G���[�łȂ���ΐ���I��
	if( uRet == MIDIERR_NODEVICE ){		// MIDI�f�o�C�X�������ꍇ
		bMidiOff = true;				// MIDI�͎g��Ȃ� (�g���Ȃ��͂�)
		return true;					// �ꉞ����I�� (���o���Ȃ��œ���������)
	}
	ErrorDisp( uRet );
	return false;
}

bool MidiLib::Close()
{
	UINT	uRet;

	if( hMidiOut == NULL ) return true;

	uRet = ::midiOutClose( hMidiOut ) ;
	if( uRet == MMSYSERR_NOERROR ){
		hMidiOut = NULL;
		return true;
	}
	ErrorDisp( uRet );
	return false;
}

bool MidiLib::Send( int ac, int mes1, int mes2 )
{
	UINT	uRet;
	union {
		DWORD dwMes;
		BYTE  byMes[4];
	} objMidiMes;

	if( bMidiOff ) return true;
	if( hMidiOut == NULL ) return false;

	objMidiMes.byMes[0] = (BYTE)ac;
	objMidiMes.byMes[1] = (BYTE)mes1;
	objMidiMes.byMes[2] = (BYTE)mes2;
	objMidiMes.byMes[3] = 0;

	uRet = ::midiOutShortMsg( hMidiOut, objMidiMes.dwMes );
	if( uRet == MMSYSERR_NOERROR ) return true;
	ErrorDisp( uRet );
	return false;
}

bool MidiLib::SendLongMsg( const BYTE *pbyData, int iDataSize )
{
	UINT	uRet, uRet2;
	bool	bHeaderPrepared = false;
	BYTE	byBuffer[64000];	// �ő�T�C�Y��64KB
	MIDIHDR	moh;			// MIDI Header
	char	szMsgTemp[256];

	if( pbyData == NULL ) return false;	// NULL�`�F�b�N
	if( iDataSize > 64000 ) return false;	// �傫���`�F�b�N

	if( bMidiOff ) return true;
	if( hMidiOut == NULL ) return false;

	::memset( byBuffer, 0, 64000 );				// �ꉞ0�N���A
	::memcpy( byBuffer, pbyData, iDataSize );

	::memset( &moh, 0, sizeof( MIDIHDR ) );
	moh.lpData = (char *) byBuffer;
	moh.dwBufferLength = iDataSize;
	moh.dwBytesRecorded = iDataSize;
	moh.dwFlags = 0;

	// MIDI Header�̏���
	uRet = ::midiOutPrepareHeader( hMidiOut, &moh, sizeof( MIDIHDR ) );
	if( uRet != MMSYSERR_NOERROR ) goto SendLongMsg_Err;
	bHeaderPrepared = true;

	// Long Msg�̑��M
	uRet = ::midiOutLongMsg( hMidiOut, &moh, sizeof( MIDIHDR ) );
	if( uRet != MMSYSERR_NOERROR ) goto SendLongMsg_Err;
	::wsprintf( szMsgTemp, "midiOutLongMsg Sending.... dwFlag = %x\n", moh.dwFlags );
	DispString( szMsgTemp );

	// ��L���M�̏I������ ( ���������[�v�͂܂������ȁH )
	while( moh.dwFlags & MHDR_DONE == 0 ){ }	// MHDR_DONE�r�b�g�����܂Ŗ������[�v
	::wsprintf( szMsgTemp, "midiOutLongMsg Done. dwFlag = %x\n", moh.dwFlags );
	DispString( szMsgTemp );

SendLongMsg_Err :

	if( bHeaderPrepared ){	// MIDI Header����������Ă����Ȃ�
		// MIDI Header�̌㏈��
		uRet2 = ::midiOutUnprepareHeader( hMidiOut, &moh, sizeof( MIDIHDR ) );
	}

	bool bRet = true;
	if( uRet != MMSYSERR_NOERROR ){
		ErrorDisp( uRet );
		bRet = false;
	}
	if( uRet2 != MMSYSERR_NOERROR ){
		ErrorDisp( uRet2 );
		bRet = false;
	}
	return bRet;
}

void MidiLib::ErrorDisp( UINT nError )
{
	UINT	uRet;
	char	szErrorText[ MAXERRORLENGTH ];

	uRet = ::midiOutGetErrorText( nError, szErrorText, MAXERRORLENGTH );
	
	switch ( uRet ) {
	case MMSYSERR_NOERROR :
		DispString( szErrorText );	// �G���[�̕\��
		break;
	case MMSYSERR_BADERRNUM :
		DispString( "�w�肳�ꂽ�G���[�ԍ��͔͈͊O�ł�\n" );
		break;
	case MMSYSERR_INVALPARAM :
		DispString( "�w�肳�ꂽ�|�C���^�܂��͍\���͖̂����ł�\n" );
		break;
	default :
		::wsprintf( szErrorText, "���m�̃G���[�H �ԍ�: %d\n", uRet );
		DispString( szErrorText );
	}
}

bool MidiLib::NoteOn( int track, int scale, int vel )
{
	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( scale < 0 ) scale =0;	// �ŏ��X�P�[��
	if( scale > 0x7f ) scale = 0x7f;	// �ő�X�P�[��
	if( vel < 0 ) vel = 0;		// �ŏ��x���V�e�B
	if( vel > 0x7f ) vel = 0x7f;	// �ő�x���V�e�B

	return Send( LOC_MID_MNOTEON( track ), scale, vel );
}

bool MidiLib::NoteOff( int track, int scale )
{
	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( scale < 0 ) scale =0;	// �ŏ��X�P�[��
	if( scale > 0x7f ) scale = 0x7f;	// �ő�X�P�[��

	return Send( LOC_MID_MNOTEOFF( track ), scale, 0 );
}

bool MidiLib::TrackProgramChange( int track, int instrument )
{
	track--;					// �g���b�N�ԍ���1�傫��
	instrument--;				// �y��ԍ��͎w��l��1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( instrument < 0 ) instrument = 0;		// �ŏ��y��ԍ�
	if( instrument > 0x7f ) instrument = 0x7f;	// �ő�y��ԍ�

	return Send( LOC_MID_MPROGRAMCHANGE( track ), instrument, 0 );
}

bool MidiLib::TrackProgramBankChange( int track, int instrument, int bank )
{
	track--;					// �g���b�N�ԍ���1�傫��
	instrument--;				// �y��ԍ��͎w��l��1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( instrument < 0 ) instrument = 0;		// �ŏ��y��ԍ�
	if( instrument > 0x7f ) instrument = 0x7f;	// �ő�y��ԍ�
	if( bank < 0 ) bank = 0;	// �ŏ��o���N
	if( bank > 0x7f ) bank = 0x7f;	// �ő�o���N

	if( !Send( LOC_MID_MCONTROLCHANGE( track ), 0x00, bank ) ){
	// �g���b�N���̃o���N�Z���N�g�̓R���g���[���`�F���W0x00�̕��ł悢�͂�
		return false;
	}

	return Send( LOC_MID_MPROGRAMCHANGE( track ), instrument, 0 );
	// �o���N�Z���N�g�I����A�v���O�����`�F���W
}

bool MidiLib::TrackPanChange( int track, int pan )
{
	track--;					// �g���b�N�ԍ���1�傫��
	pan += 0x40;				// �p���̒����l��0x40�ƂȂ�̂ŁA0->0x40�֕��s�ړ�

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( pan < 0 ) pan = 0;		// �ŏ��p��(��)
	if( pan > 0x7f ) pan = 0x7f;	// �ő�p��(�E)

	return Send( LOC_MID_MCONTROLCHANGE( track ), 0x0a, pan );
	// �g���b�N���̃p���̓R���g���[���`�F���W��0x0a

}

bool MidiLib::TrackVolumeChange( int track, int volume )
{
	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( volume < 0 ) volume = 0;		// �ŏ��{�����[��
	if( volume > 0x7f ) volume = 0x7f;	// �ő�{�����[��

	return Send( LOC_MID_MCONTROLCHANGE( track ), 0x07, volume );
	// �g���b�N���̃{�����[���̓R���g���[���`�F���W��0x07
}

bool MidiLib::TrackPitchBendSensitivity( int track, int maxpitchwidth )
{
	bool	bRet = true;

	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( maxpitchwidth <  0 ) maxpitchwidth =  0;	// �ŏ��� 0 (�s�b�`�ω��Ȃ�?)
	if( maxpitchwidth > 12 ) maxpitchwidth = 12;	// �ő啝 12 (1�I�N�^�[�u)

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 100, 0x00 );
	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 101, 0x00 );
	// RPN�̋@�\�ŁAPitchBendSensitivity(RPN MSB = 0x00, RPN LSB = 0x00)��I��

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 0x06, maxpitchwidth );
	// RPN�̃f�[�^�G���g��(0x06)�ŁA�ݒ�l�𑗐M

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 100, 0x7f );
	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 101, 0x7f );
	// RPN�̋@�\�I������������ (RPN�k��)

	return bRet;
}

bool MidiLib::TrackPitchBend( int track, int pitch )
{
	WORD	wPitch;
	BYTE	byLSBPitch, byMSBPitch;

	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( pitch < -8192 ) pitch = -8192;	// �������ő�ω���
	if( pitch > 8191 ) pitch = 8191;	// ������ő�ω���

	wPitch = (WORD) ( pitch + 8192 );		// �ω��ʂ̐����l��
	byLSBPitch = wPitch % 0x80;				// ��7bit��
	byMSBPitch = wPitch / 0x80;				// ��7bit��

	return Send( LOC_MID_MPITCHBENDCHANGE( track ), byLSBPitch, byMSBPitch );
}

bool MidiLib::GM_System_On()
{
	bool	bRet;
	BYTE	GM_System_On_Data[8];

	GM_System_On_Data[0] = 0xf0;	// ex�J�n�X�e�[�^�X
	GM_System_On_Data[1] = 0x7e;
	GM_System_On_Data[2] = 0x7f;
	GM_System_On_Data[3] = 0x09;
	GM_System_On_Data[4] = 0x01;
	GM_System_On_Data[5] = 0xf7;	// ex�I���X�e�[�^�X

	bRet = SendLongMsg( GM_System_On_Data, 6 );

	::Sleep( 55 );	// ����MIDI���߂Ƃ̊ԂɍŒ�50ms�͊Ԃ��󂯂�K�v����
	return bRet;

}

bool MidiLib::GS_Reset( int devid )
{
	bool	bRet;
	BYTE	GS_Reset_Data[16];

	if( devid < 0 ) devid = 0;
	if( devid > 0x1f ) devid = 0x1f;

	GS_Reset_Data[ 0] = 0xf0;	// ex�J�n�X�e�[�^�X
	GS_Reset_Data[ 1] = 0x41;
	GS_Reset_Data[ 2] = (BYTE) devid;	// device ID (�ʏ��0x10)
	GS_Reset_Data[ 3] = 0x42;
	GS_Reset_Data[ 4] = 0x12;
	GS_Reset_Data[ 5] = 0x40;
	GS_Reset_Data[ 6] = 0x00;
	GS_Reset_Data[ 7] = 0x7f;
	GS_Reset_Data[ 8] = 0x00;
	GS_Reset_Data[ 9] = 0x41;
	GS_Reset_Data[10] = 0xf7;	// ex�I���X�e�[�^�X

	bRet = SendLongMsg( GS_Reset_Data, 11 );

	::Sleep( 55 );	// ����MIDI���߂Ƃ̊ԂɍŒ�50ms�͊Ԃ��󂯂�K�v����
	return bRet;
}

bool MidiLib::GS_TrackDrumModeSet( int track, int mode, int devid )
{
	bool	bRet;
	BYTE	byCmdData[16];

	track--;					// �g���b�N�ԍ���1�傫��

	if( track < 0 ) track = 0;	// �ŏ��g���b�N (�g���b�N1)
	if( track > 0xf ) track = 0xf;	// �ő�g���b�N (�g���b�N16)
	if( mode < 0 ) mode = 0;
	if( mode > 2 ) mode = 2;
	if( devid < 0 ) devid = 0;
	if( devid > 0x1f ) devid = 0x1f;

	byCmdData[ 0] = 0xf0;	// ex�J�n�X�e�[�^�X
	byCmdData[ 1] = 0x41;
	byCmdData[ 2] = (BYTE) devid;	// device ID (�ʏ��0x10)
	byCmdData[ 3] = 0x42;
	byCmdData[ 4] = 0x12;
	byCmdData[ 5] = 0x40;
	if( track == 9 )		byCmdData[ 6] = 0x10;			// track 10
	else if( track < 9 )	byCmdData[ 6] = 0x11 + track;	// track 1�`9
	else					byCmdData[ 6] = 0x10 + track;	// track 11�`16
	byCmdData[ 7] = 0x15;
	byCmdData[ 8] = (BYTE) mode;	// �h�������[�h�̎w��
	// mode = 0 �ŉ����y��(�h�����Z�b�g�ł͂Ȃ�)�Amode = 1 �Ńh����1�Amode = 2 �Ńh����2
	byCmdData[ 9] = 128 - ( ( byCmdData[5] + byCmdData[6] + byCmdData[7] + byCmdData[8] ) % 128 );
	// ���`�F�b�N�T��
	byCmdData[10] = 0xf7;	// ex�I���X�e�[�^�X

	bRet = SendLongMsg( byCmdData, 11 );

	::Sleep( 55 );	// ����MIDI���߂Ƃ̊ԂɍŒ�50ms�͊Ԃ��󂯂�K�v����
	return bRet;
}

/*******************************************
�������m�[�g�ԍ��֕ϊ�
����:
	char chOnmei
		����
		C, D, E, F, G, A, B�Ŏw��̂���	(�A���t�@�x�b�g���p�啶���̂�)
		����ȊO�͂��ׂ�C����
	int iOctave;
		�I�N�^�[�u�ԍ� -1, 0�`9�Ŏw��B
		����ȊO�͂��ׂ� 4 ����
	int iSharpFlat
		�V���[�v��t���b�g���w�肷��B
		�V���[�v�̏ꍇ 1, �t���b�g�̏ꍇ -1
		���w�� 0 �ŁA�����Ȃ�
�߂�l:
	�m�[�g�ԍ� ( 0�`127 )
********************************************/
int	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat )
{
	int	iZougenOnmei;

	// �����̔���
	switch ( chOnmei ){
	case 'C' :	iZougenOnmei =  0;	break;
	case 'D' :	iZougenOnmei =  2;	break;
	case 'E' :	iZougenOnmei =  4;	break;
	case 'F' :	iZougenOnmei =  5;	break;
	case 'G' :	iZougenOnmei =  7;	break;
	case 'A' :	iZougenOnmei =  9;	break;
	case 'B' :	iZougenOnmei = 11;	break;
	default	 :	iZougenOnmei =  0;	break;	// �w�肪���������ꍇ�AC
	}

	// �I�N�^�[�u�ԍ��͈̔͊O������
	if( iOctave < -1 || iOctave > 9 )	iOctave = 4;

	int	iNoteNo = ( iOctave + 1 ) * 12 + iZougenOnmei + iSharpFlat;

	if( iNoteNo < 0 )	iNoteNo = 0;
	if( iNoteNo > 127 )	iNoteNo = 127;

	return iNoteNo;
}
