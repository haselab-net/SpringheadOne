#ifndef HASE_WXD3D_HD3DRM_H
#define HASE_WXD3D_HD3DRM_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WXRMFrame.h"
#include "WXD3Dev.h"
#include <d3drm.h>
#define RMERRMSG(n, msg)		WXERRMSG((n), (msg))
#define CKRMERR(n)				WXCHECK(n)
#define CKRMERRMSG(n,msg)		WXCHECKMSG((n), (msg))

namespace Spr {

//----------------------------------------------------------------------------
//	WXRMError

///	Direct3DRMのエラー番号(HRESULT)をエラーメッセージに変換する
class WXRMError:public WXDDError{
	public:
	virtual bool Check(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
	static bool RMErrorToString(char*& err, char*& desc, HRESULT error);
};
//----------------------------------------------------------------------------
//	WXRMViewFrustum

/**	視錐台(透視変換のパラメータ)を表す。
	視錐台とは、3D空間のうち画面にレンダリングされる部分のこと。
	
	ある視点から長方形の画面を通して見える3次元空間は、大きさ無限大の角錐になる。
	視点に近すぎるものと視点から遠すぎるものをレンダリングしないことにすると、
	角錐の先端(とがっている部分)とその反対側が切り取られるための角錘台になる。
	そのため、このレンダリングされる空間のことを視錐台と呼ぶ。
	*/
struct WXRMViewFrustum{
	/**	コンストラクタ
	@param f fより遠くのものをレンダリングする。
	@param b bより近くのものをレンダリングする。
	@param size 視錐台の上面(fで定まる平面)の大きさ
	@param pos 視錐台の上面(fで定まる平面)の中心位置
	*/
	WXRMViewFrustum(REAL f=0, REAL b=0, Vec2f size=Vec2f(), Vec2f pos = Vec2f());
	/**	コンストラクタ
	@param f fより遠くのものをレンダリングする。
	@param b bより近くのものをレンダリングする。
	@param w 視錐台の上面(fで定まる平面)の横幅
	@param asp スクリーンの縦横比
	*/
	WXRMViewFrustum(REAL f, REAL b, REAL w, REAL asp=1);
	REAL front, back;
	Vec2f bgn, end;
	REAL aspect;
};
//----------------------------------------------------------------------------
//	WXRMViewport

//	視点とスクリーンを表す。
struct WXRMViewport{
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMDevice3 dev_intf_type;
	typedef IDirect3DRMViewport2 intf_type;
#elif DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DRMDevice2 dev_intf_type;
	typedef IDirect3DRMViewport intf_type;
#else
	typedef IDirect3DRMDevice dev_intf_type;
	typedef IDirect3DRMViewport intf_type;
#endif
	dev_intf_type* pRMDev;
	intf_type* pRMView;
	WXRMViewport(const WXRMViewport& v);
	WXRMViewport(dev_intf_type* d, intf_type* v);
	WXRMViewport();
	~WXRMViewport();
	void Release();
	bool operator ==(const WXRMViewport&i) const{return pRMDev == i.pRMDev;}
	bool operator <(const WXRMViewport&i) const{return pRMDev < i.pRMDev;}
	//	視錐台の設定
	bool SetViewFrustum(const WXRMViewFrustum& vf);
	//	レンダリング品質の指定
	bool SetRenderQuality(D3DRMRENDERQUALITY rq);
	//	テクスチャ品質の指定
	bool SetTextureQuality(D3DRMTEXTUREQUALITY tq);
	/**	サーフェイスのどこに描画するかの指定
		サーフェイス全体の中での割合(0..1)で位置を指定する。
		@param bgn 描画先の左上
		@param end 描画先の右下								*/
	bool SetViewport(Vec2f bgn, Vec2f end);
	/**	視点フレームの設定
		視点フレームはシーンフレームの子孫でなければならない。
		@param f 視点フレーム		*/
	bool SetCamera(WXRMFrame::intf_type* f);
	//	強制的に全領域を描画(普段は更新されない領域は描画しない)
	bool ForceUpdate();
};
//----------------------------------------------------------------------------
//	WXRM

/**	Direct3DRMの本体.
	視点、描画物、描画先などを保持する.
	1つのDirect3DRMの描画先(1ウィンドウ)につき、ひとつ用意する。
*/
class WXRM:public WXRMViewport
	{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRM3 intf_type;
	static IID InterfaceID(){ return IID_IDirect3DRM3; } 
#elif DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DRM2 intf_type;
	static IID InterfaceID(){ return IID_IDirect3DRM2; } 
#else
	typedef IDirect3DRM intf_type;
	static IID InterfaceID(){ return IID_IDirect3DRM; } 
#endif
protected:
	/// for getting rendering destination surface from other WXRM object.
	WXRM* pD3RMBase;
	/// the surface to be rendered.
	WXD3Device* pD3Dev;
	/// the window for geting rendering destination surface.
	HWND hWnd;
	/// if hWnd is selected WXRM will use this clipper.
	IDirectDrawClipper* pClipper;

	/// the position of lefttop of the viewport in surface
	Vec2f viewPos;
	/// the size of the viewport in surface
	Vec2f viewSize;
	/// Master (route) frame in which other frames are placed
    WXRMFrame scene;
	/// The frame which is describing the user's point of view
    WXRMFrame camera;
	/// viewing frustum set by user
	WXRMViewFrustum viewFrustum;
	/// current view frustum
	WXRMViewFrustum curViewFrustum;
	/// current shade mode, fill mode and lighting state
	D3DRMRENDERQUALITY renderQuality;
	/// current texture interpolation
	D3DRMTEXTUREQUALITY textureQuality;
	///	set the viewing frustum to the renderer.
	bool SetViewFrustum();
	///	Create D3DRMViewport.
	bool Create(bool bAddCallback);
	///	Release D3DRMViewport
	void Release(bool bDelCallback);
	///	When WXDDSurface is created this function will be called.
	void CreateCallback(WXDDSurface* surf);
	///	When WXDDSurface is released this function will be called.
	void ReleaseCallback(WXDDSurface* surf);
	///	When WXDDSurface is deleted this function will be called.
	void DeleteCallback(WXDDSurface* surf);
	///	Create Direct3DRM
	bool CreateRM();
	///	Release Direct3DRM
	void ReleaseRM();

	public://-----------------------------------------------------------------
	///	The intf_type com interface.
	static intf_type* pD3DRM;
	
	//	Constructor
	WXRM();
	~WXRM();
	
	///	最初のレンダリングをはじめる前に呼ぶ
	bool Create(){return Create(true);}
	///	レンダラの開放。
	void Release(){Release(true);}
	///	pD3DRMの参照カウントを増やす
	static ULONG CreateD3DRM();
	///	pD3DRMの参照カウントを減らす
	static ULONG ReleaseD3DRM();

	/**	@name set rendering destination. call one of following.*/
	//@{
	///	Render to directdraw surface.
	void Surf(WXD3Device& surf);
	///	Pointer to the redering destination surface.
	WXD3Device* PSurf(){return pD3Dev;}
	///	Get rendering destination surface from other WXRM object.
	void RMBase(WXRM& rm);
	/**	Pointer to the other WXRM object.
		If this method is called, the destination surface will be get from it.	*/
	WXRM* PRMBase(){return pD3RMBase;}
	///	Set the handle of the rendering destination window.
	void HWnd(HWND wnd);
	///	Get the handle of the rendering destination window.
	HWND HWnd(){return hWnd;}
	//@}

	/**@name scene and camera*/
	//@{
	///	The secen frame, which represents root frame for rendering.
	WXRMFrame Scene(){return scene;}
	///	The camera frame, which represents view point.
	WXRMFrame Camera(){return camera;}
	///	Set scene frame.
	void Scene(WXRMFrame& s);
	void Scene(const WXRMFrame& s);
	///	set camera frame
	void Camera(WXRMFrame& c);
	void Camera(const WXRMFrame& c);
	///	Set scene frame and camera frame at once.
	void SceneCamera(WXRMFrame&s, WXRMFrame& c);
	//@}

	/**	@name rendering setup */
	//@{
	///	set the viwing frustum.
	bool ViewFrustum(const WXRMViewFrustum& v);
	///	get the viwing frustum.
	WXRMViewFrustum ViewFrustum(){return viewFrustum;}
	/**	サーフェイスのどこに描画するかの指定
		サーフェイス全体の中での割合(0..1)で位置を指定する。
		set the position and the size of the viewport (set rate 0to1).
		@param bgn 描画先の左上
		@param end 描画先の右下								*/
	void Viewport(Vec2f vPos, Vec2f vSize);
	///	get the viewport position.
	Vec2f ViewportPos(){return viewPos;}
	///	get the viewport size.
	Vec2f ViewportSize(){return viewSize;}
	///	set the render quality (IDirect3DRMDevice::SetQuality).
	void RenderQuality(D3DRMRENDERQUALITY rq);
	/// get the render quality.
	D3DRMRENDERQUALITY RenderQuality(){return renderQuality;}
	///	set the texture qualtiy. (IDirect3DRMDevice::SetTextureQuality)
	void TextureQuality(D3DRMTEXTUREQUALITY tq);
	///	get the texture qualtiy.
	D3DRMTEXTUREQUALITY TextureQuality(){return textureQuality;}
	//@}

	/** @name rendering */
	//@{
	///	one step of the rendering.
	bool Step();
	//	rendering detail
	///	Fit the surface to the window	
	bool FitSurface();
	///	Clear viewport
	bool Clear();
	///	Render scene
	bool Render();
	///	Update back buffer
	bool Update();
	///	Flip or copy back buffer to flont buffer.
	bool Flip();
	//@}

	//	utility
	///	find WXRMObject whose name is name.
	WXRMObject Find(const char* name);
	///
	intf_type* operator ->() const;
	///
	intf_type* operator *() const;
	///	ワールド系の座標を視点系に変換する。
	WXD3Vec3 WorldToView(const WXD3Vec3& w);
	///	視点系の座標をワールド系に変換する。
	WXD3Vec3 ViewToWorld(const WXD3Vec3& v);
	///	Other classes who need this class's instance will be able to use this method.
	static WXRM& GetLastCreate();
	static WXRMError& Error();
private:
	bool bD3DRMCreated;
	static WXRMError* error;
	static WXRM* last;
};
}	//	namespace Spr
#endif