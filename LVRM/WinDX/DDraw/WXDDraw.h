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
///	This class represents display card. �f�B�X�v���C�J�[�h��\���N���X�B
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
	///	�f�o�C�X�h���C�o�̖��O
	std::string name;
	//	�f�o�C�X�h���C�o�̐���
	std::string desc;
	GUID* pGuid;
	WXDDSurfaceDesc displayMode;
	//	etc
	///	�\��ʂ̃o�b�t�@
	WXDDSurface surfPrimary;
	///	�t���X�N���[�����̃f�B�X�v���C���[�h
	WXDDDisplayModes modes;
	///	pDDraw�̊J���O�ɊJ������I�u�W�F�N�g
	std::vector<WXUnknown> objs;

	///
	WXDDraw();
	///
	WXDDraw(const WXDDraw& dd);
	~WXDDraw();
	///	�쐬(2D�̂�)
	bool Create();
	///	�쐬(3D���܂�)
	bool Create3D();
	///	�J��(~WXDDraw���Ă�)
	void Release();
	///	device window �Ƃ��Ďg�� window
	HWND HWnd(){return hWnd;}
	/**	device window �̐ݒ�
		�ʏ�͐ݒ肵�Ȃ��ł悢�B�ݒ肵�Ȃ���Ύ����I�ɃE�B���h�E�����������B
		focus window�Ɠ����E�B���h�E�����̎q�E�B���h�E��ݒ肷��B */
	void HWnd(HWND full);

	///	�t���X�N���[�����[�h�ɂ���
	bool Fullscreen(const WXDDDisplayMode& m);
	///	�t���X�N���[�����[�h�ɂ���
	bool Fullscreen(int w=640, int h=480, int bitPerPixel=16,int refresh=0);
	///	�E�B���h�E���[�h�ɖ߂�
	bool Window();
	///	�t���X�N���[�����[�h�Ȃ� true ��Ԃ��B
	bool IsFullscreen(){return bFullscreen;}
	WXDDraws* PWXDDraws(){return pWXDDraws;}
	static DWORD BPPToDDBD(int bpp);
	static DWORD FindBPP(DWORD ddbd, int bpp);

	friend WXDDSurface;
	friend WXDDFrontSurface;
	friend WXDDFlipSurface;
	friend WXD3Device;
};

/**	PC�ɂȂ����Ă��邷�ׂẴf�B�X�v���[�J�[�h��\���N���X.
	DirectDraw���g�p���邽�߂ɁA1�A�v���P�[�V������1����
	�C���X�^���X�����B
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
    //  Thread �̔r������p
#ifdef MULTITHREAD
	static CCriticalSection& CriticalSection();
#endif
	WXDDraws();
	~WXDDraws();
	///	focus window �Ƃ��Ďg��Window�B
	HWND HWnd(){return hWnd;}
	/**	focus window �Ƃ��Ďg��Window�̐ݒ�B
		�A�v���P�[�V�����̃��C���E�B���h�E��p����B
		���C���E�B���h�E�ȊO��n���ƁA�e�E�B���h�E�����ǂ��ă��C���E�B���h�E��������B
		�ݒ肵�Ȃ��ƃt���X�N���[���ɂł��Ȃ��B
		DirectSound�ȂǑ���DirectX�ɓn��Window�Ɠ����E�B���h�E��n�����ƁB */
	void HWnd(HWND wnd);
	///	�f�B�X�v���C�J�[�h�ւ̃A�N�Z�X
	///	�f�X�N�g�b�v�S�̂�\���B
	WXDDraw& Primary();
	///	i�Ԗڂ̃f�B�X�v���C�J�[�h�A0�̓f�B�X�N�g�b�v�S�̂�\���B
	WXDDraw&	DDraw(int i){ return begin()[i]; }
	///	i�Ԗڂ̃f�B�X�v���C�J�[�h�A0�̓f�B�X�N�g�b�v�S�̂�\���B
	WXDDraw& operator [](int i){	return begin()[i]; }
	///	�f�B�X�v���C�J�[�h�̐�(+1)
	int Size() const { return size(); }
	///	�Ō�ɍ����WXDDraws (2�ȏ���̂͗ǂ��Ȃ�)
	static WXDDraws* GetLastCreate();
	};
}	//	namespace LVRM

#endif
