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

///	Direct3DRM�̃G���[�ԍ�(HRESULT)���G���[���b�Z�[�W�ɕϊ�����
class WXRMError:public WXDDError{
	public:
	virtual bool Check(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
	static bool RMErrorToString(char*& err, char*& desc, HRESULT error);
};
//----------------------------------------------------------------------------
//	WXRMViewFrustum

/**	������(�����ϊ��̃p�����[�^)��\���B
	������Ƃ́A3D��Ԃ̂�����ʂɃ����_�����O����镔���̂��ƁB
	
	���鎋�_���璷���`�̉�ʂ�ʂ��Č�����3������Ԃ́A�傫��������̊p���ɂȂ�B
	���_�ɋ߂�������̂Ǝ��_���牓��������̂������_�����O���Ȃ����Ƃɂ���ƁA
	�p���̐�[(�Ƃ����Ă��镔��)�Ƃ��̔��Α����؂����邽�߂̊p����ɂȂ�B
	���̂��߁A���̃����_�����O������Ԃ̂��Ƃ�������ƌĂԁB
	*/
struct WXRMViewFrustum{
	/**	�R���X�g���N�^
	@param f f��艓���̂��̂������_�����O����B
	@param b b���߂��̂��̂������_�����O����B
	@param size ������̏��(f�Œ�܂镽��)�̑傫��
	@param pos ������̏��(f�Œ�܂镽��)�̒��S�ʒu
	*/
	WXRMViewFrustum(REAL f=0, REAL b=0, Vec2f size=Vec2f(), Vec2f pos = Vec2f());
	/**	�R���X�g���N�^
	@param f f��艓���̂��̂������_�����O����B
	@param b b���߂��̂��̂������_�����O����B
	@param w ������̏��(f�Œ�܂镽��)�̉���
	@param asp �X�N���[���̏c����
	*/
	WXRMViewFrustum(REAL f, REAL b, REAL w, REAL asp=1);
	REAL front, back;
	Vec2f bgn, end;
	REAL aspect;
};
//----------------------------------------------------------------------------
//	WXRMViewport

//	���_�ƃX�N���[����\���B
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
	//	������̐ݒ�
	bool SetViewFrustum(const WXRMViewFrustum& vf);
	//	�����_�����O�i���̎w��
	bool SetRenderQuality(D3DRMRENDERQUALITY rq);
	//	�e�N�X�`���i���̎w��
	bool SetTextureQuality(D3DRMTEXTUREQUALITY tq);
	/**	�T�[�t�F�C�X�̂ǂ��ɕ`�悷�邩�̎w��
		�T�[�t�F�C�X�S�̂̒��ł̊���(0..1)�ňʒu���w�肷��B
		@param bgn �`���̍���
		@param end �`���̉E��								*/
	bool SetViewport(Vec2f bgn, Vec2f end);
	/**	���_�t���[���̐ݒ�
		���_�t���[���̓V�[���t���[���̎q���łȂ���΂Ȃ�Ȃ��B
		@param f ���_�t���[��		*/
	bool SetCamera(WXRMFrame::intf_type* f);
	//	�����I�ɑS�̈��`��(���i�͍X�V����Ȃ��̈�͕`�悵�Ȃ�)
	bool ForceUpdate();
};
//----------------------------------------------------------------------------
//	WXRM

/**	Direct3DRM�̖{��.
	���_�A�`�敨�A�`���Ȃǂ�ێ�����.
	1��Direct3DRM�̕`���(1�E�B���h�E)�ɂ��A�ЂƂp�ӂ���B
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
	
	///	�ŏ��̃����_�����O���͂��߂�O�ɌĂ�
	bool Create(){return Create(true);}
	///	�����_���̊J���B
	void Release(){Release(true);}
	///	pD3DRM�̎Q�ƃJ�E���g�𑝂₷
	static ULONG CreateD3DRM();
	///	pD3DRM�̎Q�ƃJ�E���g�����炷
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
	/**	�T�[�t�F�C�X�̂ǂ��ɕ`�悷�邩�̎w��
		�T�[�t�F�C�X�S�̂̒��ł̊���(0..1)�ňʒu���w�肷��B
		set the position and the size of the viewport (set rate 0to1).
		@param bgn �`���̍���
		@param end �`���̉E��								*/
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
	///	���[���h�n�̍��W�����_�n�ɕϊ�����B
	WXD3Vec3 WorldToView(const WXD3Vec3& w);
	///	���_�n�̍��W�����[���h�n�ɕϊ�����B
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