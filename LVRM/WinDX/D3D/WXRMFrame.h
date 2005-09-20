#ifndef HASE_HD3DRMFRAME_H
#define HASE_HD3DRMFRAME_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <WinBasis/WBWin.h>
#include "WXD3Affine.h"
#include <WinDX/Base/WxComBase.h>
#include <d3drm.h>

namespace LVRM {

 
class WXRMVisual;
class WXRMMesh;
class WXRMLight;
class WXRMColFrame;

///	IDirect3DRMObject�̃��b�p�N���X
class WXRMObject:public WXUnknown
	{
	protected:
	char* name;
	IDirect3DRMObject*& PRMObj() const
		{
		return (IDirect3DRMObject*&) intf;	//	IDirect3DRMObject�ւ̃|�C���^
		}
	virtual IDirect3DRMObject* Clone() const;	//	�I�u�W�F�N�g�̕���������ĕԂ��D
	static void DestroyCallback(IDirect3DRMObject* , void* pArg);
												//	
	public:
	WXRMObject(const WXRMObject& o);
	WXRMObject(IDirect3DRMObject* o=NULL);
	~WXRMObject();
	virtual void BeforeCreate();
	virtual bool AfterCreate();
	virtual bool Create();
	virtual void Release();
	//	���O
	const char* Name();
	void Name(const char* n);
	//	Interface's GUID
	virtual const IID& InterfaceID() const;
	bool Check(IUnknown* pUk) const;
	//	���Z�q
	const WXRMObject& operator = (const WXRMObject& o);
	bool operator == (const WXRMObject& o) const
		{
		return PRMObj() == o.PRMObj();
		}
	};
//----------------------------------------------------------------------------
/**	Direct3DRM�̃t���[��.
	�؍\�������A�r�W���A���i���b�V���Ȃǁj�ƌ�����ێ�����.
	�؍\���̐e�ɑ΂��Ĉʒu��ݒ�ł���.
	IDirect3DRMFrame3�̑���Ɏg����.

  �؍\���̑���ɂ���
	
	����:�e�t���[���ɒǉ������q�t���[���A���b�V���A�����́A�e�t���[����������܂�
	��������ɕێ������B
	
	��:	\begin{verbatim}
	WXD3RM rm;
	WXRMFrame fr1;
	void BuildScene()
		{
		WXRMFrame fr2;
		WXRMMesh mesh("mesh.X()");
		fr1.Add(mesh);
		fr2.Add(mesh);
		rm.Scene().Add(fr1);
		rm.Scene().Add(fr2);
		fr1.DelThis();				//	fr1���V�[������폜
		}	
		//	�����ϐ� fr2, mesh �͏����邪�A�V�[���ɒǉ����ꂽ�t���[����
		//	���b�V���͎c��B
		//	fr1�ɒǉ����ꂽ���b�V�����c�邽�߁Afr1�̃��b�V����BuildScene()��
		//	�ĂԂ��тɑ�����B
	\end{verbatim}	*/
class WXRMFrame:public WXRMObject{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMFrame3 intf_type;
#elif DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DRMFrame2 intf_type;
#else
	typedef IDirect3DRMFrame intf_type;
#endif
protected:
	void CreateFrame();
	intf_type*& PFrame() const
		{
		return (intf_type*&) intf;
		}
	public:
	//	�\�z
	///	�R�s�[�R���X�g���N�^
	WXRMFrame(const WXRMFrame& f):WXRMObject(f){}
	///	�����Ȃ��̃R���X�g���N�^
	WXRMFrame(intf_type* f=NULL):WXRMObject(f){}
	///	�t�@�C������ǂݏo��
	WXRMFrame(const char* fname);
	bool Create(){
		if (!PFrame()) CreateFrame();
		return PFrame()!=NULL;
	}
	///	.X�t�@�C������ǂݏo�� �؍\�����ۂ��ƃt�@�C�����烍�[�h����B
	bool Load(const char* fname);
	
	///	intf_type �֕ϊ�
	operator intf_type* () const
		{
		return PFrame();
		}
	///	intf_type �̃��\�b�h�̌Ăяo���p
	intf_type * operator ->()
		{
		bool b = Create();
		VRASSERT(b);
		return PFrame();
		}
	//	�ȉ��t���[���̑���
	///	���O�� name �̎q�t���[����T��
	WXRMFrame Find(const char* name);
	///	fr ���q�t���[���Ȃ� true
	bool Find(const WXRMFrame& fr);
	///	�e�t���[��
	WXRMFrame Parent();
	///	�e�t���[���̐ݒ�
	virtual void Parent(intf_type* pf);
	//	�q�t���[����ǉ�
	virtual void Child(WXRMFrame& fr){Add(fr);}
	//	�q�t���[����ǉ�
	virtual void Add(WXRMFrame& fr);
	void Add(const WXRMFrame& fr){WXRMFrame f=fr; Add(fr);}
	///	�q�t���[�����폜
	virtual void Del(const WXRMFrame& fr);
	///	������e�t���[������폜
	virtual void DelThis();
	///	�q�t���[���̐�
	int NChild();
	///	i�Ԗڂ̎q�t���[��
	WXRMFrame Child(int i);
	//	�ȉ��r�W���A���i�����_�����O���ʂ���������́AMesh�Ȃǁj�̑���
	///	�r�W���A���̒ǉ�
	virtual void Add(WXRMVisual& vis);
	void Add(const WXRMVisual& vis);
	///	�r�W���A���̍폜
	virtual void Del(const WXRMVisual& vis);
	///	�r�W���A���̐�
	int NVisual();
	///	i�Ԗڂ̃r�W���A��
	WXRMVisual Visual(int i);
	///	���O�� name �̃r�W���A��
	WXRMVisual Visual(const char* name);

	//	�ȉ������̑���
	///	�����̒ǉ�
	virtual void Add(const WXRMLight& light);
	///	�����̍폜
	virtual void Del(const WXRMLight& light);
	///	�����̐�
	int NLight();
	///	i�Ԗڂ̌���
	WXRMLight Light(int i);
	///	���O�� name �̌���
	WXRMLight Light(const char* name);

	///	���ׂĂ̎q�t���[���A�r�W���A���A�������폜
	virtual void Clear();

	///	���x
	REAL RotAngle();
	WXD3Vec3 RotAxis();
	virtual void Rot(const WXD3Vec3& axis, D3DVALUE rVel);
	WXD3Vec3 Vel();
	virtual void Vel(const WXD3Vec3& vel);

	///	�e�t���[�����猩���ʒu
	virtual void Ori(const WXD3Vec3& ez, const WXD3Vec3& ey, const WXRMFrame& frRef=WXRMFrame());
	virtual void Pos(const WXD3Vec3& v, const WXRMFrame& frRef = WXRMFrame());
	///	�e�t���[�����猩�����̃t���[���̈ʒu
	WXD3Affine AfParent();
	///	�e�t���[�����猩�����̃t���[���̈ʒu�̐ݒ�
	virtual void AfParent(const WXD3Affine& af);
	///	World���W�n���猩���ʒu
	virtual WXD3Affine AfWorld();
	///	World���W�n���猩���ʒu�̐ݒ�
	virtual void AfWorld(const WXD3Affine& af);
	//	���̑�
	///
	virtual const IID& InterfaceID() const;
	///
	virtual const CLSID& ClassID() const;
	};
}	//	namespace LVRM
#endif