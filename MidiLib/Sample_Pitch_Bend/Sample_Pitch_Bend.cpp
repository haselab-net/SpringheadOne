#include "..\MidiLib.h"
#include <windows.h>
#include <stdio.h>

int	main()
{
	int	i;
	MidiLib	*pMidiLib = NULL;

	pMidiLib = new MidiLib;

	pMidiLib->Open();	// MIDI ������ (MIDI �}�b�p�[)

	pMidiLib->TrackProgramChange( 1, 49 );	// �y���Strings1�ɂ���
	pMidiLib->TrackPitchBendSensitivity( 1, 12 );	// �s�b�`�x���h�̍ő��1�I�N�^�[�u�ɐݒ�

	// �s�b�`�x���h�̋@�\�́AGM�ɂ����ڂ���Ă���̂ŁAGM���[�h�ł�GS���[�h�ł��g����͂��B

	pMidiLib->NoteOn( 1, 60, 127 );		// �m�[�g�I�� (�����o��)

	Sleep( 1000 );

	// �I�N�^�[�u��܂ŕω������Ă݂�
	for( i = 0; i < 83; i++ ){
		pMidiLib->TrackPitchBend( 1, i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// ���Ƃ̍����܂ŉ�����
	for( i = 82; i>= 0; i-- ){
		pMidiLib->TrackPitchBend( 1, i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// ���x�̓I�N�^�[�u���܂ŉ�����
	for( i = 0; i < 83; i++ ){
		pMidiLib->TrackPitchBend( 1, - i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// ���Ƃ̍����܂ŏグ��
	for( i = 82; i>= 0; i-- ){
		pMidiLib->TrackPitchBend( 1, - i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	pMidiLib->NoteOff( 1, 60 );			// �m�[�g�I�t (�����~�߂�)
	pMidiLib->TrackPitchBend( 1, 0 );	// �s�b�`�ω���߂��Ă���

	delete pMidiLib;
	return 0;
}
