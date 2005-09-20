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

	nDevNum = ::midiOutGetNumDevs();	// MIDIデバイスの数の取得
	if( nDevNum == 0 ){
		bMidiOff = true;				// MIDIは使わない (使えないはず)
		return;
	}

	if( pszDeviceName == NULL ){	// ヌルなら
		nMidiDevice = MIDI_MAPPER;	// MIDI MAPPERを使う。
		return;
	}

	if( *pszDeviceName == '\0' ){	// 長さ0の文字列 いわゆるヌル文字列なら
		nMidiDevice = MIDI_MAPPER;	// MIDI MAPPERを使う。
		return;
	}

	for( nLoop = 0; nLoop < nDevNum; nLoop++ ){
		nRet = ::midiOutGetDevCaps( nLoop, &moc, sizeof( MIDIOUTCAPS ));
		if( nRet != MMSYSERR_NOERROR ){	// 取得エラー時
			ErrorDisp( nRet );
			continue;
		}
		if( !::lstrcmp( moc.szPname, pszDeviceName ) ){	// 指定したDevice名と同じ場合
			nMidiDevice = nLoop;			// MIDI Device番号にnLoop値を入れる
			return;
		}
	}
	DispString( "指定デバイスが見つかりません MIDI MAPPERを使用します\n" );
	nMidiDevice = MIDI_MAPPER;
	return;
}

bool MidiLib::Open()
{
	UINT	uRet;

	if( bMidiOff )	return true;	// MIDIデバイスが無ければ正常終了(何もしない)
	if( hMidiOut != NULL ) return false;
		// MIDIハンドルがNULLでなければすでにオープンしているのでエラー

	uRet = ::midiOutOpen( &hMidiOut, nMidiDevice,
		(unsigned long)NULL, (unsigned long)NULL, CALLBACK_NULL );
	if( uRet == MMSYSERR_NOERROR ) return true;		// エラーでなければ正常終了
	if( uRet == MIDIERR_NODEVICE ){		// MIDIデバイスが無い場合
		bMidiOff = true;				// MIDIは使わない (使えないはず)
		return true;					// 一応正常終了 (音出さないで動かすため)
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
	BYTE	byBuffer[64000];	// 最大サイズは64KB
	MIDIHDR	moh;			// MIDI Header
	char	szMsgTemp[256];

	if( pbyData == NULL ) return false;	// NULLチェック
	if( iDataSize > 64000 ) return false;	// 大きさチェック

	if( bMidiOff ) return true;
	if( hMidiOut == NULL ) return false;

	::memset( byBuffer, 0, 64000 );				// 一応0クリア
	::memcpy( byBuffer, pbyData, iDataSize );

	::memset( &moh, 0, sizeof( MIDIHDR ) );
	moh.lpData = (char *) byBuffer;
	moh.dwBufferLength = iDataSize;
	moh.dwBytesRecorded = iDataSize;
	moh.dwFlags = 0;

	// MIDI Headerの準備
	uRet = ::midiOutPrepareHeader( hMidiOut, &moh, sizeof( MIDIHDR ) );
	if( uRet != MMSYSERR_NOERROR ) goto SendLongMsg_Err;
	bHeaderPrepared = true;

	// Long Msgの送信
	uRet = ::midiOutLongMsg( hMidiOut, &moh, sizeof( MIDIHDR ) );
	if( uRet != MMSYSERR_NOERROR ) goto SendLongMsg_Err;
	::wsprintf( szMsgTemp, "midiOutLongMsg Sending.... dwFlag = %x\n", moh.dwFlags );
	DispString( szMsgTemp );

	// 上記送信の終了判定 ( 半無限ループはまずいかな？ )
	while( moh.dwFlags & MHDR_DONE == 0 ){ }	// MHDR_DONEビットが立つまで無限ループ
	::wsprintf( szMsgTemp, "midiOutLongMsg Done. dwFlag = %x\n", moh.dwFlags );
	DispString( szMsgTemp );

SendLongMsg_Err :

	if( bHeaderPrepared ){	// MIDI Headerが準備されていたなら
		// MIDI Headerの後処理
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
		DispString( szErrorText );	// エラーの表示
		break;
	case MMSYSERR_BADERRNUM :
		DispString( "指定されたエラー番号は範囲外です\n" );
		break;
	case MMSYSERR_INVALPARAM :
		DispString( "指定されたポインタまたは構造体は無効です\n" );
		break;
	default :
		::wsprintf( szErrorText, "未知のエラー？ 番号: %d\n", uRet );
		DispString( szErrorText );
	}
}

bool MidiLib::NoteOn( int track, int scale, int vel )
{
	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( scale < 0 ) scale =0;	// 最小スケール
	if( scale > 0x7f ) scale = 0x7f;	// 最大スケール
	if( vel < 0 ) vel = 0;		// 最小ベロシティ
	if( vel > 0x7f ) vel = 0x7f;	// 最大ベロシティ

	return Send( LOC_MID_MNOTEON( track ), scale, vel );
}

bool MidiLib::NoteOff( int track, int scale )
{
	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( scale < 0 ) scale =0;	// 最小スケール
	if( scale > 0x7f ) scale = 0x7f;	// 最大スケール

	return Send( LOC_MID_MNOTEOFF( track ), scale, 0 );
}

bool MidiLib::TrackProgramChange( int track, int instrument )
{
	track--;					// トラック番号は1大きい
	instrument--;				// 楽器番号は指定値は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( instrument < 0 ) instrument = 0;		// 最小楽器番号
	if( instrument > 0x7f ) instrument = 0x7f;	// 最大楽器番号

	return Send( LOC_MID_MPROGRAMCHANGE( track ), instrument, 0 );
}

bool MidiLib::TrackProgramBankChange( int track, int instrument, int bank )
{
	track--;					// トラック番号は1大きい
	instrument--;				// 楽器番号は指定値は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( instrument < 0 ) instrument = 0;		// 最小楽器番号
	if( instrument > 0x7f ) instrument = 0x7f;	// 最大楽器番号
	if( bank < 0 ) bank = 0;	// 最小バンク
	if( bank > 0x7f ) bank = 0x7f;	// 最大バンク

	if( !Send( LOC_MID_MCONTROLCHANGE( track ), 0x00, bank ) ){
	// トラック毎のバンクセレクトはコントロールチェンジ0x00の方でよいはず
		return false;
	}

	return Send( LOC_MID_MPROGRAMCHANGE( track ), instrument, 0 );
	// バンクセレクト終了後、プログラムチェンジ
}

bool MidiLib::TrackPanChange( int track, int pan )
{
	track--;					// トラック番号は1大きい
	pan += 0x40;				// パンの中央値は0x40となるので、0->0x40へ平行移動

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( pan < 0 ) pan = 0;		// 最小パン(左)
	if( pan > 0x7f ) pan = 0x7f;	// 最大パン(右)

	return Send( LOC_MID_MCONTROLCHANGE( track ), 0x0a, pan );
	// トラック毎のパンはコントロールチェンジの0x0a

}

bool MidiLib::TrackVolumeChange( int track, int volume )
{
	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( volume < 0 ) volume = 0;		// 最小ボリューム
	if( volume > 0x7f ) volume = 0x7f;	// 最大ボリューム

	return Send( LOC_MID_MCONTROLCHANGE( track ), 0x07, volume );
	// トラック毎のボリュームはコントロールチェンジの0x07
}

bool MidiLib::TrackPitchBendSensitivity( int track, int maxpitchwidth )
{
	bool	bRet = true;

	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( maxpitchwidth <  0 ) maxpitchwidth =  0;	// 最小幅 0 (ピッチ変化なし?)
	if( maxpitchwidth > 12 ) maxpitchwidth = 12;	// 最大幅 12 (1オクターブ)

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 100, 0x00 );
	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 101, 0x00 );
	// RPNの機能で、PitchBendSensitivity(RPN MSB = 0x00, RPN LSB = 0x00)を選択

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 0x06, maxpitchwidth );
	// RPNのデータエントリ(0x06)で、設定値を送信

	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 100, 0x7f );
	bRet = bRet && Send( LOC_MID_MCONTROLCHANGE( track ), 101, 0x7f );
	// RPNの機能選択を解除する (RPNヌル)

	return bRet;
}

bool MidiLib::TrackPitchBend( int track, int pitch )
{
	WORD	wPitch;
	BYTE	byLSBPitch, byMSBPitch;

	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( pitch < -8192 ) pitch = -8192;	// 下方向最大変化量
	if( pitch > 8191 ) pitch = 8191;	// 上方向最大変化量

	wPitch = (WORD) ( pitch + 8192 );		// 変化量の正数値化
	byLSBPitch = wPitch % 0x80;				// 下7bit分
	byMSBPitch = wPitch / 0x80;				// 上7bit分

	return Send( LOC_MID_MPITCHBENDCHANGE( track ), byLSBPitch, byMSBPitch );
}

bool MidiLib::GM_System_On()
{
	bool	bRet;
	BYTE	GM_System_On_Data[8];

	GM_System_On_Data[0] = 0xf0;	// ex開始ステータス
	GM_System_On_Data[1] = 0x7e;
	GM_System_On_Data[2] = 0x7f;
	GM_System_On_Data[3] = 0x09;
	GM_System_On_Data[4] = 0x01;
	GM_System_On_Data[5] = 0xf7;	// ex終了ステータス

	bRet = SendLongMsg( GM_System_On_Data, 6 );

	::Sleep( 55 );	// 次のMIDI命令との間に最低50msは間を空ける必要あり
	return bRet;

}

bool MidiLib::GS_Reset( int devid )
{
	bool	bRet;
	BYTE	GS_Reset_Data[16];

	if( devid < 0 ) devid = 0;
	if( devid > 0x1f ) devid = 0x1f;

	GS_Reset_Data[ 0] = 0xf0;	// ex開始ステータス
	GS_Reset_Data[ 1] = 0x41;
	GS_Reset_Data[ 2] = (BYTE) devid;	// device ID (通常は0x10)
	GS_Reset_Data[ 3] = 0x42;
	GS_Reset_Data[ 4] = 0x12;
	GS_Reset_Data[ 5] = 0x40;
	GS_Reset_Data[ 6] = 0x00;
	GS_Reset_Data[ 7] = 0x7f;
	GS_Reset_Data[ 8] = 0x00;
	GS_Reset_Data[ 9] = 0x41;
	GS_Reset_Data[10] = 0xf7;	// ex終了ステータス

	bRet = SendLongMsg( GS_Reset_Data, 11 );

	::Sleep( 55 );	// 次のMIDI命令との間に最低50msは間を空ける必要あり
	return bRet;
}

bool MidiLib::GS_TrackDrumModeSet( int track, int mode, int devid )
{
	bool	bRet;
	BYTE	byCmdData[16];

	track--;					// トラック番号は1大きい

	if( track < 0 ) track = 0;	// 最小トラック (トラック1)
	if( track > 0xf ) track = 0xf;	// 最大トラック (トラック16)
	if( mode < 0 ) mode = 0;
	if( mode > 2 ) mode = 2;
	if( devid < 0 ) devid = 0;
	if( devid > 0x1f ) devid = 0x1f;

	byCmdData[ 0] = 0xf0;	// ex開始ステータス
	byCmdData[ 1] = 0x41;
	byCmdData[ 2] = (BYTE) devid;	// device ID (通常は0x10)
	byCmdData[ 3] = 0x42;
	byCmdData[ 4] = 0x12;
	byCmdData[ 5] = 0x40;
	if( track == 9 )		byCmdData[ 6] = 0x10;			// track 10
	else if( track < 9 )	byCmdData[ 6] = 0x11 + track;	// track 1～9
	else					byCmdData[ 6] = 0x10 + track;	// track 11～16
	byCmdData[ 7] = 0x15;
	byCmdData[ 8] = (BYTE) mode;	// ドラムモードの指定
	// mode = 0 で音律楽器(ドラムセットではなし)、mode = 1 でドラム1、mode = 2 でドラム2
	byCmdData[ 9] = 128 - ( ( byCmdData[5] + byCmdData[6] + byCmdData[7] + byCmdData[8] ) % 128 );
	// ↑チェックサム
	byCmdData[10] = 0xf7;	// ex終了ステータス

	bRet = SendLongMsg( byCmdData, 11 );

	::Sleep( 55 );	// 次のMIDI命令との間に最低50msは間を空ける必要あり
	return bRet;
}

/*******************************************
音名をノート番号へ変換
入力:
	char chOnmei
		音名
		C, D, E, F, G, A, Bで指定のこと	(アルファベット半角大文字のみ)
		それ以外はすべてC扱い
	int iOctave;
		オクターブ番号 -1, 0～9で指定。
		それ以外はすべて 4 扱い
	int iSharpFlat
		シャープやフラットを指定する。
		シャープの場合 1, フラットの場合 -1
		無指定 0 で、何もなし
戻り値:
	ノート番号 ( 0～127 )
********************************************/
int	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat )
{
	int	iZougenOnmei;

	// 音名の判別
	switch ( chOnmei ){
	case 'C' :	iZougenOnmei =  0;	break;
	case 'D' :	iZougenOnmei =  2;	break;
	case 'E' :	iZougenOnmei =  4;	break;
	case 'F' :	iZougenOnmei =  5;	break;
	case 'G' :	iZougenOnmei =  7;	break;
	case 'A' :	iZougenOnmei =  9;	break;
	case 'B' :	iZougenOnmei = 11;	break;
	default	 :	iZougenOnmei =  0;	break;	// 指定がおかしい場合、C
	}

	// オクターブ番号の範囲外を検査
	if( iOctave < -1 || iOctave > 9 )	iOctave = 4;

	int	iNoteNo = ( iOctave + 1 ) * 12 + iZougenOnmei + iSharpFlat;

	if( iNoteNo < 0 )	iNoteNo = 0;
	if( iNoteNo > 127 )	iNoteNo = 127;

	return iNoteNo;
}
