#include "..\MidiLib.h"
#include <windows.h>
#include <stdio.h>

int main()
{
	MidiLib	*pMidiLib = NULL;

	pMidiLib = new MidiLib;

	//pMidiLib->Open();	// ������ (MIDI�}�b�p�[�̏ꍇ)
	pMidiLib->Open( "Microsoft GS Wavetable SW Synth" );	// ������ (MIDI�f�o�C�X���w��)

	//pMidiLib->GM_System_On();
	pMidiLib->GS_Reset();	// GS���Z�b�g
/*
	pMidiLib->TrackProgramBankChange( 1, 124, 1 );	// Dog�̉�

	pMidiLib->NoteOn( 1, 60, 127 );

	MessageBox( NULL, "AAA", "BBB", MB_OK );

	pMidiLib->NoteOff( 1, 60 );
*/
	pMidiLib->GS_TrackDrumModeSet( 1, 2 );		// track 1 ���h�����Z�b�g2�ɂ���

	pMidiLib->TrackProgramChange( 1, 57 );	// �h������SFX

	for( int i = 0; i < 10; i++ ){
		pMidiLib->NoteOn( 1, 56, 127 );	// Footstep 1
		Sleep( 250 );
		pMidiLib->NoteOn( 10, 78, 127 );	// Mute Cuica
		Sleep( 250 );
		pMidiLib->NoteOn( 1, 57, 127 );	// Footstep 2
		Sleep( 250 );
		pMidiLib->NoteOn( 10, 79, 127 );	// Open Cuica
		Sleep( 250 );
	}

	delete pMidiLib;

	return 0;
}
