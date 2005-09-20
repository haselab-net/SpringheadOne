#include "..\MidiLib.h"
#include <windows.h>
#include <stdio.h>

int	main()
{
	int	i;
	MidiLib	*pMidiLib = NULL;

	pMidiLib = new MidiLib;

	pMidiLib->Open();	// MIDI 初期化 (MIDI マッパー)

	pMidiLib->TrackProgramChange( 1, 49 );	// 楽器をStrings1にする
	pMidiLib->TrackPitchBendSensitivity( 1, 12 );	// ピッチベンドの最大を1オクターブに設定

	// ピッチベンドの機能は、GMにも搭載されているので、GMモードでもGSモードでも使えるはず。

	pMidiLib->NoteOn( 1, 60, 127 );		// ノートオン (音を出す)

	Sleep( 1000 );

	// オクターブ上まで変化させてみる
	for( i = 0; i < 83; i++ ){
		pMidiLib->TrackPitchBend( 1, i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// もとの高さまで下げる
	for( i = 82; i>= 0; i-- ){
		pMidiLib->TrackPitchBend( 1, i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// 今度はオクターブ下まで下げる
	for( i = 0; i < 83; i++ ){
		pMidiLib->TrackPitchBend( 1, - i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	// もとの高さまで上げる
	for( i = 82; i>= 0; i-- ){
		pMidiLib->TrackPitchBend( 1, - i * 100 );
		Sleep( 100 );
	}

	Sleep( 1000 );

	pMidiLib->NoteOff( 1, 60 );			// ノートオフ (音を止める)
	pMidiLib->TrackPitchBend( 1, 0 );	// ピッチ変化を戻しておく

	delete pMidiLib;
	return 0;
}
