#ifndef WXD3D_H
#define WXD3D_H

#include "../Base/WXComBase.h"
#include "../Base/WXError.h"
#include <WinBasis/WBWindowMan.h>
#include <Base/Affine.h>

#if SPR_DXVER==9	//	see WXComBase.h
# include <d3d9.h>
# define D3DVIEWPORT_SPR	D3DVIEWPORT9
# define D3DMATERIAL_SPR	D3DMATERIAL9
# define D3DLIGHT_SPR		D3DLIGHT9

#elif SPR_DXVER==8
# include <d3d8.h>
# define D3DVIEWPORT_SPR	D3DVIEWPORT8
# define D3DMATERIAL_SPR	D3DMATERIAL8
# define D3DLIGHT_SPR		D3DLIGHT8

#endif

#include <dxfile.h>
namespace Spr{;

///	Direct3Dオブジェクト
class SPR_DLL WXD3D:public WXSINGLETONV(Direct3D){
	IUnknown* Create();
public:
	WXD3D();
};
class SPR_DLL WXD3DPresentParameters:public D3DPRESENT_PARAMETERS{
public:
	WXD3DPresentParameters();
	void Window(int adaptor=0, int width=0, int height=0);
	void Fullscreen(int adaptor=0);
};

///	Direct3Dデバイス（ディスプレイアダプタに対応するデバイス）．
class SPR_DLL WXD3DDevice:public WXINTFV(Direct3DDevice){
protected:
	bool bCanDraw;
	bool bFullscreen;
	bool bInit;
public:
	///	表示先の指定
	WXD3DPresentParameters presentParameters;
	///	フォーカスウィンドウ
	HWND hFocusWindow;
	///	ウィンドウの情報
	WBWindowMan winMan;

	///	コンストラクタ	
	WXD3DDevice(){
		bInit = false;
		hFocusWindow=NULL;
		presentParameters.Window();
		bCanDraw = false;
		bFullscreen = false;
	}
	/**	デバイスの構築
		hw の親をたどっていき，一番親のノードが見つかったら，それをフォーカスウィンドウとする．
		デバイスウィンドウが指定されていなければ，hwをデバイスウィンドウにする．	*/
	bool Create(HWND hw,
		UINT adapter=D3DADAPTER_DEFAULT, D3DDEVTYPE deviceType=D3DDEVTYPE_HAL,
		DWORD behaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE);
	///	サーフェースサイズの取得
	Vec2f GetSurfaceSize();
	///	ビューポートのサイズの取得
	Vec2f GetViewportSize();
	///	 ビューポートの変更．必要ならResizeを呼び出してサーフェースを大きくする．
	bool SetViewport(Vec2f sz, Vec2f pos=Vec2f(), float minZ=0.0f, float maxZ=1.0f);
	bool SetViewport(HWND hw);
	///	描画先のサイズの変更．成功した場合，trueを返す．
	bool Resize(Vec2f size, HWND wnd=NULL);
	///	画面クリア
	bool Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	///	画面に表示
	bool Present(CONST RECT* pSourceRect=NULL, CONST RECT* pDestRect=NULL, HWND hDestWindowOverride=NULL, CONST RGNDATA* pDirtyRegion=NULL);

	
	///	 フルスクリーンかどうか
	bool IsFullscreen(){ return bFullscreen; }
	///	フルスクリーン化
	void Fullscreen();
	///	ウィンドウ化
	void Window();
	///	描画できるかどうか
	bool CanDraw(){ return bCanDraw; }
	///	描画開始
	void BeginScene();
	///	描画終了
	void EndScene();
};

///	DirectXFileオブジェクト
class SPR_DLL WXDXFile: public WXSingletonInterface<IDirectXFile, &IID_IDirectXFile>{
	IUnknown* Create();
public:
	WXDXFile();
};

}

#endif
