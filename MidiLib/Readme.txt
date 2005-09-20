MIDI用ライブラリ (MidiLib) の使い方
===================================


■大体の使い方

1. まず、ポインタを用意して、

2. C++標準のnewコマンドで、オブジェクトを生成し、ポインタへ入れる。

3. Open関数で、MIDIデバイスの準備をする。

4. MIDIデバイスを操作する。

( 5. Close関数で、MIDIデバイスの後処理をする。)

6. C++標準のdeleteコマンドで、オブジェクトの破棄をする。
   上記 5.のClose関数は、ライブラリのデストラクタに含まれているので、
   Close関数は、別途呼び出さなくてもOK。

詳しくは、各サンプルを参照。

◎必要なファイルは、MidiLib.h (ヘッダファイル) と、MidiLib.cpp (コード)
のみです。アプリ側で、MidiLib.hをインクルードしてください。
◎Windows標準ライブラリ、winmm.libを使いますので、他のプログラムへ組み込む
場合、winmm.libをリンクするように設定してください。


■関数の説明

各関数に共通事項として、
  戻り値はbool型で、正常終了の場合、true, エラー終了の場合、false
  エラーの場合、DispString関数で設定された場所に、エラー文字列を
  表示する。

MIDIでは、1つのインターフェイスで、1～16までの楽器を指定できますが、
各楽器をパートと言ったり、トラックと言ったり、チャンネルと言ったり
していますが、大体同じものを指しているようです。
MidiLibでは、トラック(track)で統一しています。

・Open( const char *pszDeviceName ) もしくは Open()

  MIDIデバイスの準備。
  MIDIマッパーを用いる場合、Open関数の引数はなし。

・Close()

  MIDIデバイスの後処理。

・NoteOn( int track, int scale, int vel )

  trackで指定されたトラックに、scaleで指定された音程の鍵盤を、
  velで指定された強さで、打鍵する。(すなわち音が出る。)
  この関数を呼ぶと、該当鍵盤は押したままになる。

・NoteOff( int track, int scale )

  trackで指定されたトラックで、scaleで指定された音程の鍵盤から、
  打鍵を止める。(すなわち音が止まる。)
  この関数を呼ぶことによって、NoteOnで押した鍵盤が押されない状態に
  戻る。

・TrackProgramChange( int track, int instrument )

  trackで指定されたトラックの音色をinstrumentで指定した番号の楽器
  に割り当てる。
  ただし、この関数では、128通りのいわゆるGM音色しか割り当てできないので、
  バンク切り替えを用いた音色の割り当ては、GS_Reset(後述)を呼び出した上、
  TrackProgramBankChange(後述)を用いること。
  PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")では、
  バンク0の音色が割りえてられる。

  音色の詳細は、下記、
  「PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")の詳細」
  を参照。

・TrackProgramBankChange( int track, int instrument, int bank )

  trackで指定されたトラックの音色をbank指定を伴うinstrumentの楽器に
  割り当てる。
  PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")では、
  GS_Reset(後述)を受け付けた後でないと、バンク切り替えが有効にならないので、
  GS_Resetを前もって呼び出しておくこと。

  音色の詳細は、下記、
  「PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")の詳細」
  を参照。

・TrackPanChange( int track, int pan )

  trackで指定されたトラックのパン(左右のどの位置に楽器を定位させるか)
  を指定する。
  値範囲は -64～+63で、
  最も左 => -64
  中央   =>   0
  最も右 => +63
  の位置に定位する。
  パンの指定が無い場合、中央に定位する。

・TrackVolumeChange( int track, int volume )

  trackで指定されたトラックのトラックとしてのボリュームを設定する。
  実際の音の大きさは、このボリュームとNoteOnでのvel値の両方によって
  決まる。

・TrackPitchBendSensitivity( int track, int maxpitchwidth )

  trackで指定されたトラックの音程変化(ピッチベンド)の最大変化量を決める
  maxpitchwidthは、0～12で指定され、単位は半音になる。
  したがって、最大値12は 12半音 = 1オクターブ となる。

・TrackPitchBend( int track, int pitch )

  trackで指定されたトラックの音程を変化させる。
  TrackPitchBendSensitivityでの指定値を最大変化量として、
  pitch = 0～8191で、基準音から音の高い方へ最大変化量まで、音程が変化し、
  pitch = 0～-8192で、基準音から音の低い方へ最大変化量まで、音程が変化する。
  pitch = 0で基準音(音程の変化なし)

・GM_System_On()

  音源をGMモードに設定する。
  PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")では、
  では、初期値がGMモードなので、ほとんど使わない。

・GS_Reset() ( または GS_Reset( int devid ) )

  音源をGSモードに設定する。
  devidは音源に設定されているデバイスIDだが、普通はデフォルトで良いはず
  なので、引数はいらないはず。

  GSモードにすると。
  PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")では、
  1. バンク切り替えにより、226音色を使える。
  2. 任意のトラックをドラムモードにしたり、通常楽器モードにできる
     (GMでは、トラック10のみドラムモード、他トラックは通常楽器モードに固定)
  3. ドラムセットを2セット使うことができる。

・GS_TrackDrumModeSet( int track, int mode )
   ( または GS_TrackDrumModeSet( int track, int mode, int devid ) )

  trackで指定されたトラックを、
    通常楽器(音律楽器)モード mode = 0
    ドラムセット1            mode = 1
    ドラムセット2            mode = 2
  に変更できる。
  devidは音源に設定されているデバイスIDだが、普通はデフォルトで良いはず
  なので、引数devidは通常いらないはず。

  ドラムセットを指定したパート内で、ドラムの種類をProgramChageで変更すると、
  同じドラムセットに所属する他パートでも、ドラムの種類が変わってしまう。
  デフォルトでは、トラック10がドラムセット1に所属していて、
  他のパートはすべて通常楽器(音律楽器)モード。


■グローバル関数

・int	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat )
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


■便利なMIDI音確認ツール

SoundChecker
http://members.at.infoseek.co.jp/tomo_kusaba/sound.htm

DrumChecker
http://members.at.infoseek.co.jp/tomo_kusaba/drum.htm

( 作者のページ http://members.at.infoseek.co.jp/tomo_kusaba/ )


■参考資料

PC内蔵音源(いわゆる"Microsoft GS Wavetable SW Synth")の詳細
http://www.wiztext.net/~sakura_news/4neria/mgs.html

MIDIの基本リファレンス ( 音源固有事項以外 )
http://www.kishi.net/rws/music.html

MIDIの上級(?)リファレンス ( エクスクルーシブとか )
http://www2s.biglobe.ne.jp/~puri-q/midilec.html


================================
2003.11.05	tmiura 作成
2003.11.06	tmiura ディレクトリ構成を変更
2003.11.06	tmiura パンの設定値を変更
2003.11.06	tmiura ピッチベンドの機能を追加
2003.11.19	tmiura 音名をノート番号へ変換する関数を追加

