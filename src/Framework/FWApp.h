#ifndef FWAPP_H
#define FWAPP_H
#include "FWPointer.h"
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <HIS/HIMouse.h>
#include <Device/DVDeviceManager.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>

namespace Spr{;

class FWApp:public UTRefCount{
protected:
	HWND hWnd;							//	ウィンドウハンドル

	WBCriticalSection cs;				//	クリティカルセクション
	Spr::WBMMTimer timer;				//	シミュレーション用マルチメディアタイマー
	UINT simuTimerId;					//	シミュレーション用WMタイマーのID
	float defaultTimeStep;				//	シーンに時間刻みが書いてないときに使う時間刻み
	bool bPause;						//	一時停止中フラグ
	volatile bool bBusy;				//	計算が間に合わないとき true になる．
	bool bOutForce;						//	力覚提示を行うかどうか
	float simuTimerPeriod;
	//	
	UTString initialLoadFile;			//	シーンが準備できる前に，ロードが呼ばれた場合，ファイル名をここに覚えておく
	UTString lastLoadFile;				//	最後にロードしたファイル名
	std::vector<std::string> fileList;	//	ファイルを切り替える場合のファイルリスト
	int fileListCur;					//	ファイルリスト内での位置
public:
	enum {
		TIMER_SIMU = 1,
		TIMER_DRAW = 2,
	};
	enum {TIMERTICK = 1000};
	UTRef<SGScene> scene;
	UTRef<GRRender> render; 
	FWPointers pointers;
	UTRef<HIMouse> mouse;
	DVDeviceManager devMan;

	FWApp();
	virtual ~FWApp();

	void InitDevice();
	float GetSimuTimerPeriod(){return simuTimerPeriod;}
	virtual void Init(HWND hWnd);
	virtual void Step();
	virtual bool AddHis(const char* type);
	virtual void ClearHis();
	virtual void ConnectHis();

	virtual bool PreviewMessage(MSG* pMsg);

	virtual void Draw();
	virtual void CreateD3DRender();
	virtual void CreateGLRender();
	virtual void LoadList();
	virtual void Load(UTString fn);
	virtual void LoadImp(UTString fn){}
	virtual void Save(UTString fn);
	virtual void SaveImp(UTString fn){}
	virtual bool OnKeyDown(unsigned int key);
	virtual void Reset();
protected:
	static void TimerFunc(void* arg);
	static void CALLBACK WMTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

};

}
#endif
