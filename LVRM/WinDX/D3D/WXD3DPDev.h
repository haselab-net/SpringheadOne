#ifndef LVRM_WXD3D_WXD3DPDev_H
#define LVRM_WXD3D_WXD3DPDev_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WXD3Dev.h"

namespace LVRM {


#if DIRECTDRAW_VERSION >= 0x700
//----------------------------------------------------------------------------
//	WXD3Light
//
///	WXD3D7�p�̌���
class WXD3Light:public D3DLIGHT7{
public:
	WXD3Light();
};

//----------------------------------------------------------------------------
//	WXD3Viewport
//
///	WXD3D7�p��Viewport(���_�ƕ`���)
class WXD3Viewport:public D3DVIEWPORT7{
	WXD3Vec3 begin;	//	�r���[�|�[�g�̍����O�̓_, 0..1�Ŏw��
	WXD3Vec3 end;	//	�r���[�|�[�g�̉E�����̓_, 0..1�Ŏw��
	WXD3Vec3 screen;//	�J�������猩���r���[�|�[�g�̒��S�̈ʒu
	D3DVALUE front;	//	��O�̃N���b�s���O���ʂƃJ�����̋���
	D3DVALUE back;	//	���̃N���b�s���O���ʂƃJ�����̋���
public:
	WXD3Viewport(WXD3Vec3 be, WXD3Vec3 e, WXD3Vec3 s, D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(WXD3Vec3 s, D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(const RECT& rc, D3DVALUE minZ=0.0f, D3DVALUE maxZ=1.0f);
	WXD3Viewport(const D3DVIEWPORT7& vp);
	void TargetSize(SIZE sz);
	WXD3Affine Projection();
};

//----------------------------------------------------------------------------
//	WXD3DPDevice
//
///	3D�`��\�ȃT�[�t�F�C�X
class WXD3DPDevice:public WXDDFlipSurface{
protected:
	class CZSurface:public WXDDSurface {
		bool Create(){ return Create(true); }
		bool Create(bool bCallCallback){ return Create(16, true, bCallCallback); }
		bool Create(DWORD depth, bool bHardWare, bool bCallCallback=true);
		friend WXD3DPDevice;
	} zBuffer;
public:
	typedef WXDDFlipSurface baseclass;
	typedef IDirect3DDevice7 intf_type;
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
	WXD3DPDevice();
	~WXD3DPDevice();
	///	�`���f�B�X�v���C�J�[�h�̐ݒ�
	virtual void DDraw(WXDDraw& dd);
	///	�g�p�\��3D�����_�����
	virtual bool EnumDrivers();
	///	�T�[�t�F�C�X�̍쐬
	bool Create(){return Create(true);}
	///	�T�[�t�F�C�X�̊J��
	void Release(){Release(true);}
	///	�T�[�t�F�C�X�������Ă���Ε�������
	virtual bool Restore();
	WXD3Viewport viewport;
	void Viewport(const WXD3Viewport& vp);
	const WXD3Viewport& Viewport(){ return viewport; }

protected:
	virtual bool Create(bool bCallCallback){ return Create(bCallCallback, 1); }
	virtual bool Create(bool bCallCallback, int nBackBuffer);
	virtual void Release(bool bCallCallback);
	bool ClearImp(POINT dest, SIZE sz, DWORD color, DWORD flags);
	friend class WXRM;
};
#endif

}	//	namespace LVRM
#endif
