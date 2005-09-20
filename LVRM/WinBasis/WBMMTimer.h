#ifndef MMTIMER_H
#define MMTIMER_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WBWin.h"
namespace LVRM {

/**	マルチメディアタイマー.
	タイマー動作中にデバッガを使うとOSが落ちるので，デバッグ用に
	スレッドによる擬似動作モードを用意してある．	*/
class WBMMTimer{
	static int count;
	int tick;
	public:
	typedef void MMTimerFunc(void* arg);

	protected:
	static UINT resolution;		///<	タイマーの時間精度
	UINT interval;				///<	タイマーの時間間隔
	MMTimerFunc* func;			///<	時間が来るたびに呼ばれるコールバック関数．
	void* arg;					///<	コールバック関数に渡す引数
	UINT timerID;
	volatile bool bCreated;		///<	タイマーが動作しているかどうか
	volatile bool bThread;		///<	タイマーがスレッドとして擬似動作しているかどうか
    volatile bool bRun;			///<	コールバック関数が実行中かどうか
	HANDLE hThread;				///<	擬似動作用スレッド
	static void CALLBACK TimerCallback(UINT uID, UINT, DWORD dwUser, DWORD, DWORD);
	static DWORD WINAPI ThreadCallback(void* arg);
	void BeginPeriod();
	void EndPeriod();

	public:
	volatile int heavy;

	WBMMTimer();					///<	コンストラクタ
	~WBMMTimer();					///<	デストラクタ
	UINT Resolution();				///<	タイマーの時間精度
	void Resolution(UINT res);		///<	タイマーの時間精度の設定
	UINT Interval();				///<	タイマーの時間間隔
	void Interval(UINT i);			///<	タイマーの時間間隔の設定
	void Set(MMTimerFunc* f, void* arg);
									///<	コールバック関数の設定
	bool Create();					///<	タイマー動作開始
	bool Thread();					///<	タイマー擬似動作開始
	bool IsCreated(){return bCreated;}
									///<	タイマーが動作しているかどうか
	bool IsThread(){return bThread;}
									///<	タイマーが擬似動作しているかどうか
	void Release();					///<	タイマーの停止
};

}	//	namespace LVRM
#endif
