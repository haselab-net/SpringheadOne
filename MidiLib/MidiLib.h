#ifndef	_MIDILIB_H_
#define	_MIDILIB_H_

#include <windows.h>
#include <mmsystem.h>

class MidiLib {
	HMIDIOUT	hMidiOut;	// MIDI�o�͂̃n���h��
	UINT		nMidiDevice;	// MIDI Device �ԍ�
	bool		bMidiOff;	// MIDI���g��Ȃ�

public:
	void SetDevice( const char *pszDeviceName );	// MIDI�f�o�C�X���̌������f�o�C�X�Z�b�g
	bool Open();			// ������
	bool Open( const char *pszDeviceName )			// ������(MIDI�f�o�C�X���w��)
	{
		SetDevice( pszDeviceName );		// MIDI�f�o�C�X���̌������f�o�C�X�Z�b�g
		bool bRet = Open();				// MIDI�I�[�v��
		return bRet;
	}
	bool Close();			// �㏈��
	bool Send( int ac, int mes1, int mes2 );	// MIDI�֑��M
	bool SendLongMsg( const BYTE *pbyData, int iDataSize );	// MIDI�֑��M

	void ErrorDisp( UINT nError );		// �G���[�\��
	virtual void DispString( const char *szDisp )	// ������̕\��
	{
		::OutputDebugString( szDisp );	// �f�o�b�O�E�B���h�E�֕\��
	}

	//	MIDI �V���[�g���b�Z�[�W�n
	bool NoteOn( int track, int scale, int vel );	// �m�[�g�I��
	bool NoteOff( int track, int scale );		// �m�[�g�I�t
	bool TrackProgramChange( int track, int instrument );	// �y��ύX
	bool TrackProgramBankChange( int track, int instrument, int bank );	// �y��ύX
	bool TrackPanChange( int track, int pan );	// �p���ύX
	bool TrackVolumeChange( int track, int volume );	// �{�����[���ύX
	bool TrackPitchBendSensitivity( int track, int maxpitchwidth );	// �s�b�`�x���h���ݒ�
	bool TrackPitchBend( int track, int pitch );	// �s�b�`�x���h

	//	MIDI �����O���b�Z�[�W�n
	bool GM_System_On();				// GM�V�X�e���I��
	bool GS_Reset( int devid = 0x10 );	// GS���Z�b�g
	bool GS_TrackDrumModeSet( int track, int mode, int devid = 0x10 );	// �h�������[�h�̃Z�b�g

	MidiLib::MidiLib(){		// �R���X�g���N�^
		hMidiOut = NULL;	// �n���h���̃k��������
		nMidiDevice = MIDI_MAPPER;	// �I�[�v�����鎞�̃f�t�H���g��MIDI�}�b�p�[
		bMidiOff = false;	// MIDI�͎g�p�ɃZ�b�g
	}
	MidiLib::~MidiLib(){		// �f�X�g���N�^
		Close();
	}
};

int	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat = 0 );	// �������m�[�g�ԍ��֕ϊ�

#endif	// _MIDILIB_H_
