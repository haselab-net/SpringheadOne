#ifndef	_MIDILIB_H_
#define	_MIDILIB_H_

#include <windows.h>
#include <mmsystem.h>

class MidiLib {
	HMIDIOUT	hMidiOut;	// MIDI出力のハンドル
	UINT		nMidiDevice;	// MIDI Device 番号
	bool		bMidiOff;	// MIDIを使わない

public:
	void SetDevice( const char *pszDeviceName );	// MIDIデバイス名の検索＆デバイスセット
	bool Open();			// 初期化
	bool Open( const char *pszDeviceName )			// 初期化(MIDIデバイス名指定)
	{
		SetDevice( pszDeviceName );		// MIDIデバイス名の検索＆デバイスセット
		bool bRet = Open();				// MIDIオープン
		return bRet;
	}
	bool Close();			// 後処理
	bool Send( int ac, int mes1, int mes2 );	// MIDIへ送信
	bool SendLongMsg( const BYTE *pbyData, int iDataSize );	// MIDIへ送信

	void ErrorDisp( UINT nError );		// エラー表示
	virtual void DispString( const char *szDisp )	// 文字列の表示
	{
		::OutputDebugString( szDisp );	// デバッグウィンドウへ表示
	}

	//	MIDI ショートメッセージ系
	bool NoteOn( int track, int scale, int vel );	// ノートオン
	bool NoteOff( int track, int scale );		// ノートオフ
	bool TrackProgramChange( int track, int instrument );	// 楽器変更
	bool TrackProgramBankChange( int track, int instrument, int bank );	// 楽器変更
	bool TrackPanChange( int track, int pan );	// パン変更
	bool TrackVolumeChange( int track, int volume );	// ボリューム変更
	bool TrackPitchBendSensitivity( int track, int maxpitchwidth );	// ピッチベンド幅設定
	bool TrackPitchBend( int track, int pitch );	// ピッチベンド

	//	MIDI ロングメッセージ系
	bool GM_System_On();				// GMシステムオン
	bool GS_Reset( int devid = 0x10 );	// GSリセット
	bool GS_TrackDrumModeSet( int track, int mode, int devid = 0x10 );	// ドラムモードのセット

	MidiLib::MidiLib(){		// コンストラクタ
		hMidiOut = NULL;	// ハンドルのヌル初期化
		nMidiDevice = MIDI_MAPPER;	// オープンする時のデフォルトはMIDIマッパー
		bMidiOff = false;	// MIDIは使用にセット
	}
	MidiLib::~MidiLib(){		// デストラクタ
		Close();
	}
};

int	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat = 0 );	// 音名をノート番号へ変換

#endif	// _MIDILIB_H_
