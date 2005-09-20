#ifdef USE_SOUND
#ifndef __EASYDS_H_
#define __EASYDS_H_

#include <windows.h>
//#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <vector>

class CEasyDS;
class CEasyDSWaveBuffer;

class CEasyDS
{
	HWND hwnd;
	char *errorMsg;
	bool available;
	int waveMax;

	// ダイレクトサウンドオブジェクト
	LPDIRECTSOUND8 DSObject;
	CEasyDSWaveBuffer **waveObjects;

	// ＭＩＤＩ再生用変数
//	char MciBuffer[32];
//	bool PlayFlg;

	// 諸関数
	void RELEASE_COM(IUnknown *x) { x->Release(); x=NULL; }

	bool DS_INIT();
	void DS_END();

public:
	CEasyDS(HWND _hwnd,int _waveMax);
	~CEasyDS();
	bool IsAvailable(){ return available; }
	char* ErrorMsg(){ return errorMsg; }
	CEasyDSWaveBuffer *Waves(int index);
};

class CEasyDSWaveBuffer
{
	LPDIRECTSOUND8 DSObject;
	std::vector<LPDIRECTSOUNDBUFFER> buffers;
	int synchronous; //最大同時発音数
	int bufferPoint;
	char *errorMsg;

	void RELEASE_COM(IUnknown *x) { x->Release(); x=NULL; }
	bool CreateDuplexBuffers(int index);

public:
	CEasyDSWaveBuffer(LPDIRECTSOUND8 _DSObject);
	~CEasyDSWaveBuffer();
//	bool IsAvailable(){ return available; }	// DirectSoundを利用可能か
	char* ErrorMsg(){ return errorMsg; }	// エラーメッセージを返す
	bool LoadFromFile( char *filename );	// Waveファイルを読み込んでバッファに格納
	int GetSynchronous(){ return buffers.size(); } // 同時発音数を返す
	bool SetSynchronous(int synchronous);	// 同時発音数を設定する
	bool Play();							// 再生する
	void Clear();							// 保存されているバッファを消去する
};


#endif // end of __EASYDS_H_
#endif // USE_SOUND