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

///	Direct3D�I�u�W�F�N�g
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

///	Direct3D�f�o�C�X�i�f�B�X�v���C�A�_�v�^�ɑΉ�����f�o�C�X�j�D
class SPR_DLL WXD3DDevice:public WXINTFV(Direct3DDevice){
protected:
	bool bCanDraw;
	bool bFullscreen;
	bool bInit;
public:
	///	�\����̎w��
	WXD3DPresentParameters presentParameters;
	///	�t�H�[�J�X�E�B���h�E
	HWND hFocusWindow;
	///	�E�B���h�E�̏��
	WBWindowMan winMan;

	///	�R���X�g���N�^	
	WXD3DDevice(){
		bInit = false;
		hFocusWindow=NULL;
		presentParameters.Window();
		bCanDraw = false;
		bFullscreen = false;
	}
	/**	�f�o�C�X�̍\�z
		hw �̐e�����ǂ��Ă����C��Ԑe�̃m�[�h������������C������t�H�[�J�X�E�B���h�E�Ƃ���D
		�f�o�C�X�E�B���h�E���w�肳��Ă��Ȃ���΁Chw���f�o�C�X�E�B���h�E�ɂ���D	*/
	bool Create(HWND hw,
		UINT adapter=D3DADAPTER_DEFAULT, D3DDEVTYPE deviceType=D3DDEVTYPE_HAL,
		DWORD behaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE);
	///	�T�[�t�F�[�X�T�C�Y�̎擾
	Vec2f GetSurfaceSize();
	///	�r���[�|�[�g�̃T�C�Y�̎擾
	Vec2f GetViewportSize();
	///	 �r���[�|�[�g�̕ύX�D�K�v�Ȃ�Resize���Ăяo���ăT�[�t�F�[�X��傫������D
	bool SetViewport(Vec2f sz, Vec2f pos=Vec2f(), float minZ=0.0f, float maxZ=1.0f);
	bool SetViewport(HWND hw);
	///	�`���̃T�C�Y�̕ύX�D���������ꍇ�Ctrue��Ԃ��D
	bool Resize(Vec2f size, HWND wnd=NULL);
	///	��ʃN���A
	bool Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	///	��ʂɕ\��
	bool Present(CONST RECT* pSourceRect=NULL, CONST RECT* pDestRect=NULL, HWND hDestWindowOverride=NULL, CONST RGNDATA* pDirtyRegion=NULL);

	
	///	 �t���X�N���[�����ǂ���
	bool IsFullscreen(){ return bFullscreen; }
	///	�t���X�N���[����
	void Fullscreen();
	///	�E�B���h�E��
	void Window();
	///	�`��ł��邩�ǂ���
	bool CanDraw(){ return bCanDraw; }
	///	�`��J�n
	void BeginScene();
	///	�`��I��
	void EndScene();
};

///	DirectXFile�I�u�W�F�N�g
class SPR_DLL WXDXFile: public WXSingletonInterface<IDirectXFile, &IID_IDirectXFile>{
	IUnknown* Create();
public:
	WXDXFile();
};

}

#endif
