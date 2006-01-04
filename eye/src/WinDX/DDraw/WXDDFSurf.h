#ifndef Spr_DDRAW_HDDFSURF_H
#define Spr_DDRAW_HDDFSURF_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WXDDSurf.h"

namespace Spr {

//----------------------------------------------------------------------------
//	WXDDFlipSurface

/**	A flippable surface which has both back buffer and front buffer.
	If you need DirectDraw and don't need Direct3D, use this class
	as a flippable surface.	*/
class WXDDFlipSurface:public WXDDSurface{
	protected:
	bool bFlippable;
	static HRESULT WINAPI EnumBackBufferCallback(IDirectDrawSurface* pDDS, DDSURFACEDESC* pDesc, void* pContext);	//	バックバッファの列挙
	void ReleaseCallback(WXDDSurface* surf);
	virtual bool Create(bool bCallCallback, int nBack);
	bool Create(bool bCallCallback){ return Create(bCallCallback, 0); }
	virtual void Release(bool bCallCallback);

	public:
	//	表画面
	WXDDFrontSurface front;

	WXDDFlipSurface();
	~WXDDFlipSurface();
	//	描画バッファと表示バッファの入れ替え
	virtual bool Flip();
	//	実際に入れ替えているのか、コピーをしているのか
	bool IsRealFlip(){return bFlippable;}
	//	ウィンドウの位置と大きさの管理
	virtual bool FitWindow(POINT pt, SIZE sz){return front.FitWindow(pt, sz);}
	virtual bool FitWindowToSurface(){return front.FitWindowToSurface();}
	virtual void SaveWindowPlace(){front.SaveWindowPlace();}
	virtual bool RestoreWindowPlace(){return front.RestoreWindowPlace();}
	virtual bool FitSurface();			//	Surfaceをウィンドウの大きさにあわせる
	
	//	Override WXDDSurface's methods.
	HWND HWnd(){return front.HWnd();}
	virtual void HWnd(HWND wnd){front.HWnd(wnd);}
	void DDraw(WXDDraw& dd){WXDDSurface::DDraw(dd);front.DDraw(dd);}
	bool Create(){return Create(true);}
	void Release(){Release(true);}
	virtual bool Restore();
};

}	//	namespace Spr

#endif