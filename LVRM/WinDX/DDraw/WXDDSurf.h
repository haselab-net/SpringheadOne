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

/**	(���z)VRAM.
	surface �Ƃ̓O���t�B�b�N�p�̃o�b�t�@�̂��ƁB
	DirectDrawSurface�͏��Ȃ��Ƃ��ȉ��̋@�\������
	All direct draw surfaces have these method at least. */
class WXDDSurfaceBase{
	protected:
	WXDDraw* pWXDD;

	public:
	WXDDSurfaceBase(){pWXDD=NULL;}
	///	�o�b�t�@�̊m��
	virtual bool Create() = 0;
	///	�o�b�t�@�̊J��
	virtual void Release() = 0;
	/**	�u���b�N�]���Bsurf �̓��e�������ɃR�s�[����B
		@param surf �R�s�[���T�[�t�F�C�X
		@param dest �R�s�[���`�̈�̍���̓_�̍��W
		@param sz	�R�s�[�̈�̃T�C�Y
		@param src	�R�s�[���T�[�t�F�C�X�̍���̓_�̍��W	*/
	virtual bool Blt(WXDDSurface& surf, POINT dest, SIZE sz, POINT src, DWORD flags=DDBLT_WAIT);
	virtual bool Blt(WXDDSurface& surf, POINT dest, SIZE size, DWORD flags=DDBLT_WAIT){POINT pt={0,0}; return Blt(surf, dest, size, pt, flags);}
	virtual bool Blt(WXDDSurface& surf, DWORD flags=DDBLT_WAIT){POINT pt={0,0}; SIZE size={-1, -1}; return Blt(surf, pt, size, flags);}
	/**	�I�[�o�[���C�Dsurf �������̏�ɃI�[�o�[���C����D
		@param surf �R�s�[���T�[�t�F�C�X
		@param ptDest	�I�[�o�[���C���`�̈�̍���̓_�̍��W
		@param szDest	�I�[�o�[���C�̈�̃T�C�Y
		@param ptSrc	�I�[�o�[���C���T�[�t�F�C�X�̍���̓_�̍��W
		@param szDest	�I�[�o�[���C���̗̈�̃T�C�Y	*/
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, POINT ptSrc, SIZE szSrc, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL);
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, POINT ptSrc, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ SIZE szSrc = {-1,-1}; return Overlay(ptDest, surf, szDest, ptSrc, szSrc, flags, fx); }
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, SIZE szDest, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ POINT ptSrc = {0,0}; return Overlay(ptDest, surf, szDest, ptSrc, flags, fx); }
	virtual bool Overlay(POINT ptDest, WXDDSurface& surf, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ SIZE szDest = {-1,-1}; return Overlay(ptDest, surf, szDest, flags, fx); }
	virtual bool Overlay(WXDDSurface& surf, DWORD flags=DDOVER_SHOW , DDOVERLAYFX* fx=NULL){ POINT ptDest = {0,0}; return Overlay(ptDest, surf, flags, fx); }
	/**	�I�[�o�[���C�̒�~�D*/
	virtual bool StopOverlay(WXDDSurface& surf){ return false; }

	/**	�N���A�A�T�[�t�F�C�X�̓h��Ԃ��B
		@param	color	�T�[�t�F�C�X��h��Ԃ��F	*/
	virtual bool Clear(DWORD color=0, DWORD flags=DDBLT_WAIT)=0;
	/**	�N���A�A�T�[�t�F�C�X�̋�`�̈�̓h��Ԃ��B
		@param	pt		��`�̈�̍���̓_�̍��W
		@param	sz		��`�̈�̑傫��
		@param	color	�T�[�t�F�C�X��h��Ԃ��F	*/
	virtual bool Clear(POINT pt, SIZE sz, DWORD color=0, DWORD flags=DDBLT_WAIT)=0;
	///	�f�o�C�X�R���e�L�X�g�̎擾(�K���`�悵�I���x�ɊJ�����邱��)
	virtual HDC GetDC()=0;
	///	�f�o�C�X�R���e�L�X�g�̊J��
	virtual void ReleaseDC()=0;
	///	�o�b�t�@�������Ă��Ȃ����Ƃ��m�F
	virtual bool Restore()=0;
	///	�o�b�t�@�̑傫��(pixel�P��)
	virtual SIZE Size()=0;
	///	�F���s�N�Z���l�ɕϊ�
	virtual DWORD ColorToPixel(COLORREF col)=0;
	///	1�o�C�g������̃s�N�Z����
	virtual DWORD PixelPerByte()=0;
	///	1�s�N�Z��������̃o�C�g��
	virtual DWORD BytePerPixel()=0;
	///	�o�b�t�@���̋�`�̈�ւ̒��ڃA�N�Z�X�̊J�n
	virtual bool Lock(POINT pt, SIZE sz)=0;
	///	�o�b�t�@�S�̂ւ̒��ڃA�N�Z�X�̊J�n
	virtual bool Lock()=0;
	///	�o�b�t�@�ւ̒��ڃA�N�Z�X�̏I��
	virtual void Unlock()=0;
	///	y�s�ڂ̍s���̃s�N�Z���̃A�h���X
	virtual BYTE* Line(DWORD y)=0;
	///	�s��
	virtual DWORD NLine()=0;
	///	�s�̒���(byte�P��)
	virtual DWORD LineLen()=0;
	///	�T�[�t�F�C�X��������WXDDraw
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

///	surface �̋@�\�Ə�Ԃ�ێ�����\����
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

///	IDirectDrawSurface�̊�{�N���X
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
/**	��ʓI�� direct draw surface.
	Direct3D�̕`���ɂ� WXD3Device,�_�u���o�b�t�@�ɂ� WXDDFlipSurface
	��p����.
	�o�b�N�o�b�t�@�Ƃ��ėp���邱�Ƃ��ł���.
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
	
	///	Create() �̑O�� �o�b�t�@�̑傫�����Z�b�g����
	virtual void Size(const SIZE& sz){desc.Size(sz);}
	virtual SIZE Size();
	///	Create() �̑O�� DDSCAPS ���Z�b�g����
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

///	�E�B���h�E�̑傫�����Ǘ�����
class WXDDWindowManage
	{
	protected:
	HWND hWnd;
	WINDOWPLACEMENT* pWndPlaceOld;
	WINDOWPLACEMENT* pMainWndPlaceOld;

	public:
	WXDDWindowManage();
	~WXDDWindowManage();
	///	�E�C���h�E�̈ʒu�� pt �傫���� sz �ɂ���
	virtual bool FitWindow(POINT pt, SIZE sz);
	///	�E�B���h�E�̈ʒu�Ƒ傫�����L������
	virtual void SaveWindowPlace();
	///	�Ō�ɋL�^�����ʒu�Ƒ傫���ɖ߂�
	virtual bool RestoreWindowPlace();
	///	�����Ă���E�B���h�E�̃n���h��
	HWND HWnd(){return hWnd;}
	///	�����E�C���h�E�̐ݒ�
	virtual void HWnd(HWND wnd){hWnd = wnd;}
	///	pt, sz ���X�N���[�����W�n�ɕϊ�
	void ClientToScreen(POINT* pt, SIZE* sz);
	///	�E�B���h�E�̃N���C�A���g�̈�̑傫��
	SIZE Size();
	};

///	�\�����̃o�b�t�@
class WXDDFrontSurface:public WXDDSurfaceBase, public WXDDWindowManage{
public:
	typedef WXDDSurfaceBase baseclass;
protected:
	IDirectDrawClipper* pClipper;

public:
	WXDDFrontSurface();
	~WXDDFrontSurface();
	///	Create() �̑O�ɐݒ�
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
	/**	�o�b�t�@�ւ̒��ڃA�N�Z�X�̊J�n.
		�o�b�t�@���ڃA�N�Z�X�ł́A�N���b�s���O�ł��Ȃ����߁A
		��ɏ�����E�B���h�E�Ȃǂ𖳎����ĕ`�悵�Ă��܂��B*/
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