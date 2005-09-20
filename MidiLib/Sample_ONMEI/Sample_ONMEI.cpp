#include <stdio.h>
#include <windows.h>
#include "..\MidiLib.h"

int	main()
{
	MidiLib *pMidiLib = NULL;

	pMidiLib = new MidiLib;

	pMidiLib->Open();

	int	iKey1 = OnmeiConv( 'C', 3 );		// C3
	int	iKey2 = OnmeiConv( 'F', 3, 1 );		// F#3
	printf( "Key1 = %d\tKey2 = %d\n", iKey1, iKey2 );
	pMidiLib->NoteOn( 1, iKey1, 127 );
	pMidiLib->NoteOn( 1, iKey2, 127 );

	Sleep( 1000 );

	pMidiLib->NoteOff( 1, iKey1 );
	pMidiLib->NoteOff( 1, iKey2 );

	delete pMidiLib;

	return 0;
}
