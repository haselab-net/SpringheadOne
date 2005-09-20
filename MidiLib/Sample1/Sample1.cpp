#include "..\MidiLib.h"
#include <windows.h>
#include <stdio.h>

int main()
{
	MidiLib	*pMidiLib = NULL;

	pMidiLib = new MidiLib;

	//pMidiLib->Open();	// 初期化 (MIDIマッパーの場合)
	pMidiLib->Open( "Microsoft GS Wavetable SW Synth" );	// 初期化 (MIDIデバイス名指定)

	//pMidiLib->GM_System_On();
	pMidiLib->GS_Reset();	// GSリセット
/*
	pMidiLib->TrackProgramBankChange( 1, 124, 1 );	// Dogの音

	pMidiLib->NoteOn( 1, 60, 127 );

	MessageBox( NULL, "AAA", "BBB", MB_OK );

	pMidiLib->NoteOff( 1, 60 );
*/
	pMidiLib->GS_TrackDrumModeSet( 1, 2 );		// track 1 をドラムセット2にする

	pMidiLib->TrackProgramChange( 1, 57 );	// ドラムのSFX

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
