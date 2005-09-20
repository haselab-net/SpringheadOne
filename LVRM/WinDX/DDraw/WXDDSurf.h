#ifndef LVRM_DDRAW_DDSURF_H
#define LVRM_DDRAW_DDSURF_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <WinBasis/WBWin.h>
#include <WinDX/Base/WXCallback.h>
#include <ddraw.h>

namespace LVRM {
class WXDDSurfaceBase;
class WXDDSurface;
class WXDDFrontSurface;
class WXDDFlipSurface;
class WXDDSurfaceDesc;
class WXDDraw;

/**	(仮想)VRAM.
	surface とはグラフィック用のバッファのこと。
	DirectDrawSurfaceは少なくとも以下の機能を持つ
	All direct draw surfaces have these method at least. */
class WXDDSurfaceBase{
	protected:
	WXDDraw* pWXDD;

	public:
	WXDDSurfaceBase(){pWXDD=NULL;}
	///	バッファの確保
	virtual bool Create() = 0;
	///	バッファの開放
	virtual void Release() = 0;
	/**	ブロック転送。surf の内容を自分にコピーする。
		@param surf コピー元サーフェイス
		@param dest コピー先矩形領域の左上の点の座標
		@param sz	コピー領域のサイズ
		@param src	コピー元サーフェイスの左上の点の座標	*/
	virtual bool Blt(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags=DDBLT_WAIT);
	virtual bool Blt(WXDDSurface& surf, POINT dest, SIZE size, DWORD flags=DDBLT_WAIT){POINT pt={0,0}; return Blt(surf, dest, size, pt, flags);}
	virtual bool Blt(WXDDSurface& surf, DWORD flags=DDBLT_WAIT){POINT pt={0,0}; SIZE size={-1, -1}; return Blt(surf, pt, size, flags);}
	/**	オーバーレイ．surf を自分の上にオーバーレイする．
		@param surf コピー元サーフェイス
		@param ptDest	オーバーレイ先矩形領域の左上の点の座標
		@param szDest	オーバーレイ領域のサイズ
		@param ptSrc	オーバーレイ元サーフェイスの左上の点の座標
		@param szDest	オーバーレイ元の領域のサイズ	*/
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, POINT ptSrc, SIZE szSrc, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL);
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, POINT ptSrc, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ SIZE szSrc = {-1,-1}; return Overlay(ptDest, surf, szDest, ptSrc, szSrc, flags, fx); }
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ POINT ptSrc = {0,0}; return Overlay(ptDest, surf, szDest, ptSrc, flags, fx); }
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ SIZE szDest = {-1,-1}; return Overlay(ptDest, surf, szDest, flags, fx); }
	virtual bool Overlay(WXDDSurface& surf, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ POINT ptDest = {0,0}; return Overlay(ptDest, surf, flags, fx); }
	/**	オーバーレイの停止．*/
	virtual bool StopOverlay(WXDDSurface& surf){ return false; }

	/**	クリア、サーフェイスの塗りつぶし。
		@param	color	サーフェイスを塗りつぶす色	*/
	virtual bool Clear(DWORD color=0, DWORD flags=DDBLT_WAIT)=0;
	/**	クリア、サーフェイスの矩形領域の塗りつぶし。
		@param	pt		矩形領域の左上の点の座標
		@param	sz		矩形領域の大きさ
		@param	color	サーフェイスを塗りつぶす色	*/
	virtual bool Clear(POINT pt, SIZE sz, DWORD color=0, DWORD flags=DDBLT_WAIT)=0;
	///	デバイスコンテキストの取得(必ず描画し終わる度に開放すること)
	virtual HDC GetDC()=0;
	///	デバイスコンテキストの開放
	virtual void ReleaseDC()=0;
	///	バッファが失われていないことを確認
	virtual bool Restore()=0;
	///	バッファの大きさ(pixel単位)
	virtual SIZE Size()=0;
	///	色をピクセル値に変換
	virtual DWORD ColorToPixel(COLORREF col)=0;
	///	1バイトあたりのピクセル数
	virtual DWORD PixelPerByte()=0;
	///	1ピクセルあたりのバイト数
	virtual DWORD BytePerPixel()=0;
	///	バッファ中の矩形領域への直接アクセスの開始
	virtual bool Lock(POINT pt, SIZE sz)=0;
	///	バッファ全体への直接アクセスの開始
	virtual bool Lock()=0;
	///	バッファへの直接アクセスの終了
	virtual void Unlock()=0;
	///	y行目の行頭のピクセルのアドレス
	virtual BYTE* Line(DWORD y)=0;
	///	行数
	virtual DWORD NLine()=0;
	///	行の長さ(byte単位)
	virtual DWORD LineLen()=0;
	///	サーフェイスが属するWXDDraw
	virtual void DDraw(WXDDraw& dd);
	WXDDraw& DDraw(){return *pWXDD;}
	WXDDraw* PWXDD(){return pWXDD;}
	///	
	virtual WXDDSurfaceDesc& Desc()=0;
	enum PixelFormatT{
		RGB555,
		RGB565,
		RGB16=RGB565,
		RGB24,
		RGB32,
		Y221,
		YUY2,
		UYVY,
		VYUY,
		vyuy
	};
	void PixelFormat(WXDDSurfaceBase::PixelFormatT t);
protected:
	virtual bool OverlayImp(RECT* prcDest, WXDDSurface& surf, RECT* prcSrc, DWORD flags, DDOVERLAYFX* fx)=0;
	virtual bool BltImp(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags)=0;
};

///	surface の機能と状態を保持する構造体
class WXDDSurfaceDesc: public DDSURFACEDESC{
protected:
	DWORD lineLen;
	RECT rcLock;
	SIZE szLock;
	bool bLock;
	void Clear();

public:
	DWORD pixelPerByte;
	DWORD bytePerPixel;
	DWORD bitPerPixel;

	WXDDSurfaceDesc();
	WXDDSurfaceDesc(const DDSURFACEDESC& desc);
	void Caps(DWORD caps);
	DWORD Caps();
	SIZE Size() const;
	void Size(const SIZE& sz);
	void PixelFormat(WXDDSurfaceBase::PixelFormatT t);
	void Calc();
	friend class WXDDSurface;
};

///	IDirectDrawSurfaceの基本クラス
class WXDDSInterface
	{
	public:
	WXDDSInterface();
	WXDDSInterface(IDirectDrawSurface*);
	WXDDSInterface(const WXDDSInterface& cidds);
	~WXDDSInterface();
	void Release();
	IDirectDrawSurface* pDDS;
	operator IDirectDrawSurface*(){return pDDS;}
	IDirectDrawSurface2* pDDS2;
	operator IDirectDrawSurface2*(){return pDDS2;}
#if DIRECTDRAW_VERSION >= 0x0500
	IDirectDrawSurface3* pDDS3;
	operator IDirectDrawSurface3*(){return pDDS3;}
#endif
#if DIRECTDRAW_VERSION >= 0x0700
	IDirectDrawSurface7* pDDS7;
	operator IDirectDrawSurface7*(){return pDDS7;}
#endif

#if DIRECTDRAW_VERSION >= 0x0700
	IDirectDrawSurface7* operator ->(){return pDDS7;}
	typedef IDirectDrawSurface7 IDirectDrawSurfaceLast;
#elif DIRECTDRAW_VERSION >= 0x0500
	IDirectDrawSurface3* operator ->(){return pDDS3;}
	typedef IDirectDrawSurface3 IDirectDrawSurfaceLast;
#else
	IDirectDrawSurface2* operator ->(){return pDDS2;}
	typedef IDirectDrawSurface2 IDirectDrawSurfaceLast;
#endif
	bool operator ==(const WXDDSInterface& d)const {return pDDS==d.pDDS;}
	bool operator >(const WXDDSInterface& d)const {return pDDS>d.pDDS;}
	bool operator <(const WXDDSInterface& d)const {return pDDS<d.pDDS;}
	WXDDSInterface operator =(const WXDDSInterface& d);
	};
/**	一般的な direct draw surface.
	Direct3Dの描画先には WXD3Device,ダブルバッファには WXDDFlipSurface
	を用いる.
	バックバッファとして用いることができる.
*/
class WXDDSurface:public WXDDSurfaceBase, public WXDDSInterface{
public:
	typedef WXDDSurfaceBase baseclass;
protected:
	virtual bool Create(bool bCallCallback);
	virtual void Release(bool bCallCallback);
	WXDDSurfaceDesc descOld;
	HDC hDC;
	bool bCallDeleteCallbacks;
	friend WXDDFlipSurface;

public:
	WXDDSurfaceDesc desc;
	WXCallbacks<WXDDSurface> createCallbacks;
	WXCallbacks<WXDDSurface> releaseCallbacks;
	WXCallbacks<WXDDSurface> deleteCallbacks;

	WXDDSurface();
	WXDDSurface(IDirectDrawSurface* intf);
	~WXDDSurface();
	
	///	Create() の前に バッファの大きさをセットする
	virtual void Size(const SIZE& sz){desc.Size(sz);}
	virtual SIZE Size();
	///	Create() の前に DDSCAPS をセットする
	virtual void Caps(DWORD caps){desc.Caps(caps);}
	virtual DWORD Caps(){return desc.Caps();}

	//	Override WXDDSurfaceBase's methods.
	bool Create(){return Create(true);}
	void Release(){Release(true);}
	virtual bool Clear(DWORD color=0, DWORD flags=DDBLT_WAIT);
	virtual bool Clear(POINT pt, SIZE sz, DWORD color=0, DWORD flags=DDBLT_WAIT){ return ClearImp(pt, sz, color, flags); }
	virtual bool Lock(POINT pt, SIZE sz);
	virtual bool Lock(){POINT pt={0,0}; SIZE sz={-1,-1};return Lock(pt,sz);}
	virtual void Unlock();
	virtual HDC GetDC();
	virtual void ReleaseDC();
	virtual bool Restore();
	virtual bool IsLost();
	virtual DWORD ColorToPixel(COLORREF col);
	virtual DWORD PixelPerByte(){return desc.pixelPerByte;}
	virtual DWORD BytePerPixel(){return desc.bytePerPixel;}
	virtual DWORD LineLen(){return desc.lineLen;}
	virtual DWORD NLine(){return desc.rcLock.bottom - desc.rcLock.top;}
	virtual BYTE* Line(DWORD y){return (desc.bLock) ? (BYTE*)desc.lpSurface+desc.lPitch*y : NULL;}
	virtual WXDDSurfaceDesc& Desc(){ return desc; }
	virtual bool StopOverlay(WXDDSurface& surf);
protected:
	virtual bool OverlayImp(RECT* prcDest, WXDDSurface& surf, RECT* prcSrc, DWORD flags, DDOVERLAYFX* fx);
	virtual bool BltImp(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags);
	virtual bool ClearImp(POINT pt, SIZE sz, DWORD color, DWORD flags);
	friend class WXDDFrontSurface;
};

///	ウィンドウの大きさを管理する
class WXDDWindowManage
	{
	protected:
	HWND hWnd;
	WINDOWPLACEMENT* pWndPlaceOld;
	WINDOWPLACEMENT* pMainWndPlaceOld;

	public:
	WXDDWindowManage();
	~WXDDWindowManage();
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

///	表示中のバッファ
class WXDDFrontSurface:public WXDDSurfaceBase, public WXDDWindowManage{
public:
	typedef WXDDSurfaceBase baseclass;
protected:
	IDirectDrawClipper* pClipper;

public:
	WXDDFrontSurface();
	~WXDDFrontSurface();
	///	Create() の前に設定
	void HWnd(HWND wnd){WXDDWindowManage::HWnd(wnd);}
	HWND HWnd(){return WXDDWindowManage::HWnd();}

	//	Override WXDDWindowManage's methods.
	void ClientToScreen(POINT* pt, SIZE* sz);

	//	Override WXDDSurfaceBase's methods.
	bool Create();
	void Release();
	bool IsLost();
	bool Restore();
	bool Clear(DWORD color=0, DWORD flags=DDBLT_WAIT);
	bool Clear(POINT pt, SIZE sz, DWORD color=0, DWORD flags=DDBLT_WAIT);
	bool FitWindowToSurface();
	void Unlock();
	HDC GetDC();
	void ReleaseDC();
	/**	バッファへの直接アクセスの開始.
		バッファ直接アクセスでは、クリッピングできないため、
		上に乗ったウィンドウなどを無視して描画してしまう。*/
	bool Lock(POINT pt, SIZE sz);
	bool Lock(){POINT pt={0,0}; SIZE sz={-1,-1};return Lock(pt,sz);}
	SIZE Size(){return WXDDWindowManage::Size();}
	DWORD ColorToPixel(COLORREF col);
	DWORD PixelPerByte();
	DWORD BytePerPixel();
	DWORD LineLen();
	DWORD NLine();
	BYTE* Line(DWORD y);
	virtual WXDDSurfaceDesc& Desc();
	virtual bool StopOverlay(WXDDSurface& surf);
protected:
	bool BltImp(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags);
	bool OverlayImp(RECT* prcDest, WXDDSurface& surf, RECT* prcSrc, DWORD flags, DDOVERLAYFX* fx);
	bool SetClipper();
};

struct WXDDSInterfaces:public std::vector<WXDDSInterface>
	{
	WXDDSInterfaces();
	int cur;
	int Cur(){return cur;}
	WXDDSInterface& CurDDS(){return begin()[cur];}
	void Cur(int c);
	};

}	//	namespace LVRM

#endif