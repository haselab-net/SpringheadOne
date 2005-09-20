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
	static HRESULT WINAPI EnumBackBufferCallback(IDirectDrawSurface* pDDS, DDSURFACEDESC* pDesc, void* pContext);	//	�o�b�N�o�b�t�@�̗�
	void ReleaseCallback(WXDDSurface* surf);
	virtual bool Create(bool bCallCallback, int nBack);
	bool Create(bool bCallCallback){ return Create(bCallCallback, 0); }
	virtual void Release(bool bCallCallback);

	public:
	//	�\���
	WXDDFrontSurface front;

	WXDDFlipSurface();
	~WXDDFlipSurface();
	//	�`��o�b�t�@�ƕ\���o�b�t�@�̓���ւ�
	virtual bool Flip();
	//	���ۂɓ���ւ��Ă���̂��A�R�s�[�����Ă���̂�
	bool IsRealFlip(){return bFlippable;}
	//	�E�B���h�E�̈ʒu�Ƒ傫���̊Ǘ�
	virtual bool FitWindow(POINT pt, SIZE sz){return front.FitWindow(pt, sz);}
	virtual bool FitWindowToSurface(){return front.FitWindowToSurface();}
	virtual void SaveWindowPlace(){front.SaveWindowPlace();}
	virtual bool RestoreWindowPlace(){return front.RestoreWindowPlace();}
	virtual bool FitSurface();			//	Surface���E�B���h�E�̑傫���ɂ��킹��
	
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