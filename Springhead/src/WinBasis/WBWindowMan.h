#ifndef WBWINDOWMAN_H
#define WBWINDOWMAN_H
#include "WBWin.h"

namespace Spr{;

///	ウィンドウの大きさを管理する
class WBWindowMan{
	protected:
	HWND hWnd;
	WINDOWPLACEMENT* pWndPlaceOld;
	WINDOWPLACEMENT* pMainWndPlaceOld;

	public:
	WBWindowMan();
	~WBWindowMan();
	///	ウインドウの位置を pt 大きさを sz にする
	virtual bool FitWindow(POINT pt, SIZE sz);
	///	ウィンドウの位置と大きさを記憶する
	virtual void SaveWindowPlace();
	///	最後に記録した位置と大きさに戻す
	virtual bool RestoreWindowPlace();
	///	扱っているウィンドウのハンドル
	HWND HWnd(){return hWnd;}
	///	扱うウインドウの設定
	virtual void HWnd(HWND wnd){hWnd = wnd;}
	///	pt, sz をスクリーン座標系に変換
	void ClientToScreen(POINT* pt, SIZE* sz);
	///	ウィンドウのクライアント領域の大きさ
	SIZE Size();
};

}	//	namespace Spr

#endif
