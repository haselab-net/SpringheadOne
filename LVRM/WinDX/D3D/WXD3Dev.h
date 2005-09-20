#ifndef LVRM_WXD3D_WXD3Dev_H
#define LVRM_WXD3D_WXD3Dev_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "../DDraw/WXDDraw.h"
#include "../DDraw/WXDDFSurf.h"
#include "WXD3Affine.h"
#include <d3d.h>

namespace LVRM {

//----------------------------------------------------------------------------
//	WXD3Driver
///	3D�����_�����O�h���C�o�ɑΉ�����N���X
class WXD3Driver{
public:
	///	3D�����_�����O�h���C�o��GUID
	GUID guid;
	///	3D�����_�����O�h���C�o�̐���
	std::string about; 
	///	3D�����_�����O�h���C�o�̖��O
	std::string name; 
	///	HAL���ǂ���
	bool bHAL;
	///	desc7���g�p���邩�ǂ���
	bool bDesc7;
private:
#if DIRECTDRAW_VERSION >= 0x700
	///	�쐬�ł���T�[�t�F�C�X�ɂ��Ă̋L�q(DirectX7�ȍ~)
	D3DDEVICEDESC7 desc7;
#endif
	///	�쐬�ł���T�[�t�F�C�X�ɂ��Ă̋L�q
	D3DDEVICEDESC desc;

public:
	WXD3Driver();
	int ZBufferDepth(int max=16);
	/**	�w�肳�ꂽ�F���̃T�[�t�F�C�X�Ƀ����_�����O�\��
		@param	bitPerPixel	1�s�N�Z���̃r�b�g��.
		@result	�\�Ȃ� true.		*/
	bool CheckRendDepth(int bitPerPixel);
	void SetDesc(D3DDEVICEDESC& d);
	D3DDEVICEDESC* GetDesc(){ return bDesc7 ? NULL : &desc; }
#if DIRECTDRAW_VERSION >= 0x700
	void SetDesc(D3DDEVICEDESC7& d7);
	D3DDEVICEDESC7* GetDesc7(){ return bDesc7 ? &desc7 : NULL; }
#endif
protected:
	DWORD ZBufferDepthMask();
};
//----------------------------------------------------------------------------
//	WXD3Drivers

///	3D�����_�����O�h���C�o�̔z��
class WXD3Drivers: public std::vector<WXD3Driver>{
protected:
	int cur;
	bool Select(int rendDepth, bool bHALOK=true);
	bool Select();
public:
	///	�g������3D�����_�����w��
	int prefer;
	WXD3Drivers();
	int Cur(){return cur;}
	///	���ݎg�p����3D�����_��
	WXD3Driver& CurDriver();
	///	�g�p����3D�����_���� id �ɂ���B(�ł��Ȃ����Ƃ�����)
	void Set(int id);
#if DIRECTDRAW_VERSION >= 0x700
	static HRESULT PASCAL EnumDevicesCallback(char* desc, char* name, D3DDEVICEDESC7* devDesc, void* pContext);
#endif
	static HRESULT PASCAL EnumDevicesCallback(GUID* pGuid, char* a, char* n, D3DDEVICEDESC* pHAL, D3DDEVICEDESC* pHEL, void* pContext);
	friend class WXD3Device;
	friend class WXD3DPDevice;
};

//----------------------------------------------------------------------------
//	WXD3Device
//
///	3D�`��\�ȃT�[�t�F�C�X
class WXD3Device:public WXDDFlipSurface{
public:
#if DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DDevice2 intf_type;
#else
	typedef IDirect3DDevice intf_type;
#endif
	///	�g�p����3D�����_��
	WXD3Drivers drivers;
	///	�f�o�C�X��COM�C���^�t�F�C�X
	intf_type* pD3DDev;
	intf_type* operator ->() const {return pD3DDev;}
	intf_type* operator ->() {
		if (!pD3DDev) Create();
		return pD3DDev;
	}
	///
	WXD3Device();
	~WXD3Device();
	///	�`���f�B�X�v���C�J�[�h�̐ݒ�
	virtual void DDraw(WXDDraw& dd);
	///	�g�p�\��3D�����_�����
	virtual bool EnumDrivers();
	///	�T�[�t�F�C�X�̍쐬
	virtual bool Create(){return Create(true);}
	///	�T�[�t�F�C�X�̊J��
	virtual void Release(){Release(true);}
	///	�T�[�t�F�C�X�������Ă���Ε�������
	virtual bool Restore();

protected:
	virtual bool Create(bool bCallCallback, int nBackBuffer);
	virtual bool Create(bool bCallCallback){ return Create(bCallCallback, 1); }
	virtual void Release(bool bCallCallback);
	class CZSurface:public WXDDSurface {
		virtual bool Create(){ return Create(true); }
		virtual bool Create(bool bCallCallback){ return Create(16, true, true); }
		virtual bool Create(DWORD depth, bool bHardWare, bool bCallCallback=true);
		friend WXD3Device;
	}zBuffer;
	friend class WXRM;
	friend class WXD3IM;
};
}	//	namespace LVRM
#endif
