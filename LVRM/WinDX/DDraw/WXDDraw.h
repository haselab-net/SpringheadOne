#ifndef LVRM_DDRAW_DDRAW_H
#define LVRM_DDRAW_DDRAW_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WXDDMode.h"
#include "WXDDFSurf.h" 
#include <WinDX/Base/WxComBase.h>
#include <WinDX/Base/WXError.h>
#include <WinBasis/WBCriticalSection.h>

#ifdef MULTITHREAD
 #define AUTOLOCK WXDDAutoLock autoLock;
#else
 #define AUTOLOCK
#endif

struct IDirect3D;
struct IDirect3D2;
struct IDirect3D7;

namespace LVRM {
class WXDDrawError;
class WXDDraw;
class WXDDraws;
class WXDDSurfaceDesc;
class WXDDSurface;
class WXDDFlipSurface;
class WXD3Device;


class WXDDraws;
///	This class represents display card. ディスプレイカードを表すクラス。
class WXDDraw{
	friend WXDDraws;
protected:
	HWND hWnd;
	bool bFullscreen;
	WINDOWPLACEMENT wplaceOld;
	bool CreatePalette();
	void ReleasePalette();
	void SetClipper();
	void GetDisplayMode();
	WXDDraws* pWXDDraws;
public:
	///	direct draw interfaces
	IDirectDraw* pDDraw;
	IDirectDraw2* pDDraw2;
#if DIRECTDRAW_VERSION >= 0x0700
	IDirectDraw7* pDDraw7;
	typedef IDirectDraw7 IDirectDrawC;
	IDirectDrawC*& PDDraw(){return pDDraw7;}
#else
	typedef IDirectDraw2 IDirectDrawC;
	IDirectDrawC*& PDDraw(){return pDDraw2;}
#endif
	IDirectDrawPalette* pPalette;
	IDirectDrawClipper* pClipper;
	///	direct 3d interface
#if DIRECTDRAW_VERSION >= 0x0700
	IDirect3D2* pD3D;
	IDirect3D7* pD3D7;
	typedef IDirect3D7 IDirect3DC;
	IDirect3DC*& PWXD3D(){return pD3D7;}
#elif DIRECTDRAW_VERSION >= 0x0500
	IDirect3D2* pD3D;
	typedef IDirect3D2 IDirect3DC;
	IDirect3DC*& PWXD3D(){return pD3D;}
#else
	IDirect3D* pD3D;
	typedef IDirect3D IDirect3DC;
	IDirect3DC* PWXD3D(){return pD3D;}
#endif
	///	ddraw device driver infomation
	///	デバイスドライバの名前
	std::string name;
	//	デバイスドライバの説明
	std::string desc;
	GUID* pGuid;
	WXDDSurfaceDesc displayMode;
	//	etc
	///	表画面のバッファ
	WXDDSurface surfPrimary;
	///	フルスクリーン時のディスプレイモード
	WXDDDisplayModes modes;
	///	pDDrawの開放前に開放するオブジェクト
	std::vector<WXUnknown> objs;

	///
	WXDDraw();
	///
	WXDDraw(const WXDDraw& dd);
	~WXDDraw();
	///	作成(2Dのみ)
	bool Create();
	///	作成(3Dも含む)
	bool Create3D();
	///	開放(~WXDDrawが呼ぶ)
	void Release();
	///	device window として使う window
	HWND HWnd(){return hWnd;}
	/**	device window の設定
		通常は設定しないでよい。設定しなければ自動的にウィンドウが生成される。
		focus windowと同じウィンドウかその子ウィンドウを設定する。 */
	void HWnd(HWND full);

	///	フルスクリーンモードにする
	bool Fullscreen(const WXDDDisplayMode& m);
	///	フルスクリーンモードにする
	bool Fullscreen(int w=640, int h=480, int bitPerPixel=16,int refresh=0);
	///	ウィンドウモードに戻す
	bool Window();
	///	フルスクリーンモードなら true を返す。
	bool IsFullscreen(){return bFullscreen;}
	WXDDraws* PWXDDraws(){return pWXDDraws;}
	static DWORD BPPToDDBD(int bpp);
	static DWORD FindBPP(DWORD ddbd, int bpp);

	friend WXDDSurface;
	friend WXDDFrontSurface;
	friend WXDDFlipSurface;
	friend WXD3Device;
};

/**	PCにつながっているすべてのディスプレーカードを表すクラス.
	DirectDrawを使用するために、1アプリケーションに1つだけ
	インスタンスを作る。
*/
class WXDDraws: protected std::vector<WXDDraw>
	{
	friend WXDDraw;
	protected:
	static WXDDraws* last;
	WXDDraws* lastOld;
	WXDDraw* primary;
	HWND hWnd;
	bool Create();
	static BOOL WINAPI DDEnumCallback(GUID* pGUID, char* pDesc, char* pName, void* pContext);
#if DIRECTDRAW_VERSION < 0x700
	static BOOL WINAPI DDEnumCallbackEx(GUID* pGUID, char* pDesc, char* pName, void* pContext, HANDLE hm);
#else
	static BOOL WINAPI DDEnumCallbackEx(GUID* pGUID, char* pDesc, char* pName, void* pContext, HMONITOR hm);
#endif

	public:
    //  Thread の排他制御用
#ifdef MULTITHREAD
	static CCriticalSection& CriticalSection();
#endif
	WXDDraws();
	~WXDDraws();
	///	focus window として使うWindow。
	HWND HWnd(){return hWnd;}
	/**	focus window として使うWindowの設定。
		アプリケーションのメインウィンドウを用いる。
		メインウィンドウ以外を渡すと、親ウィンドウをたどってメインウィンドウを見つける。
		設定しないとフルスクリーンにできない。
		DirectSoundなど他のDirectXに渡すWindowと同じウィンドウを渡すこと。 */
	void HWnd(HWND wnd);
	///	ディスプレイカードへのアクセス
	///	デスクトップ全体を表す。
	WXDDraw& Primary();
	///	i番目のディスプレイカード、0はディスクトップ全体を表す。
	WXDDraw&	DDraw(int i){ return begin()[i]; }
	///	i番目のディスプレイカード、0はディスクトップ全体を表す。
	WXDDraw& operator [](int i){	return begin()[i]; }
	///	ディスプレイカードの数(+1)
	int Size() const { return size(); }
	///	最後に作ったWXDDraws (2つ以上作るのは良くない)
	static WXDDraws* GetLastCreate();
	};
}	//	namespace LVRM

#endif
