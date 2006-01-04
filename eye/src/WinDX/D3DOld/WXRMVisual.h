#ifndef HWXD3DVISUAL_H
#define HWXD3DVISUAL_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <WinBasis/WBWinTypes.h>
#include "WXD3Affine.h"
#include "WXRMFrame.h"
#include <d3drm.h>

namespace Spr {

//-----------------------------------------------------------------------------
//	WXRMVisual
//
class WXRMVisual: public WXRMObject{
public:
	typedef IDirect3DRMVisual intf_type;
	WXRMVisual(intf_type* pVis=NULL):WXRMObject(pVis){}
	WXRMVisual(const WXRMVisual& v):WXRMObject(v){}
	intf_type*& PVisual() const{ return (intf_type*&) intf; }
	virtual const IID& InterfaceID() const { return IID_IDirect3DRMVisual; }
	};
//-----------------------------------------------------------------------------
//	WXRMUserVisual
//
class WXRMUserVisual: public WXRMVisual{
public:
	WXRMUserVisual(WXRMVisual::intf_type* pVis=NULL):WXRMVisual(pVis){}
	WXRMUserVisual(const WXRMUserVisual& v):WXRMVisual(v){}
	IDirect3DRMUserVisual*& PVisual(){ return (IDirect3DRMUserVisual*&) intf; }
	virtual const IID& InterfaceID() const{ return IID_IDirect3DRMUserVisual; }
	virtual const CLSID& ClassID() const{ return CLSID_CDirect3DRMUserVisual; }
};
//-----------------------------------------------------------------------------
//	WXRMLight	�����@�t���[���ɂ������Ďg��
//		intf_type �̑���Ɏg����
class WXRMLight: public WXRMObject{
public:
	typedef IDirect3DRMLight intf_type;
protected:
	IDirect3DRMLight*& PLight() const{ return (IDirect3DRMLight*&) intf; }
public:
	WXRMLight();
	WXRMLight(intf_type*  l);
	WXRMLight(const WXRMLight& l);
	WXRMLight(D3DRMLIGHTTYPE lt, D3DCOLOR col);		//	�����̎�ނƐF�̐ݒ�
	void Set(D3DRMLIGHTTYPE lt, D3DCOLOR col);
	operator intf_type* () const{
		return (intf_type*) intf;
	}
	virtual const IID& InterfaceID() const{
		return IID_IDirect3DRMLight;
	}
	virtual const CLSID& ClassID() const{
		return CLSID_CDirect3DRMLight;
	}
};
//-----------------------------------------------------------------------------
//	WXRMTexture		�e�N�X�`���̃r�b�g�}�b�v��ێ�����
//		IDirect3DRMTexture �̑���Ɏg����
class WXRMTexture:public WXRMVisual{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMTexture3 intf_type;
#elif DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DRMTexture2 intf_type;
#else
	typedef IDirect3DRMTexture intf_type;
#endif
protected:
	intf_type*& PTex() const
		{
		return (intf_type*&) intf;
		}
	public:
	WXRMTexture():WXRMVisual(){}
	WXRMTexture(const WXRMTexture& t):WXRMVisual(t){}
	WXRMTexture(intf_type* t):WXRMVisual(t){}
	WXRMTexture(std::string fname, bool bTransp=false, D3DCOLOR colTransp=RGB(0,0,0), Vec2f decaleSize=Vec2f(1,1), Vec2f decaleOrigin=Vec2f(0,0));
	bool Load(std::string fname, bool bTransp=false, D3DCOLOR colTransp=RGB(0,0,0), Vec2f decaleSize=Vec2f(1,1), Vec2f decaleOrigin=Vec2f(0,0));
		//	�e�N�X�`����.BMP�t�@�C������ǂݏo���B�r�b�g�}�b�v�̑傫����2^n * 2^n�łȂ���΂Ȃ�Ȃ��B
	operator intf_type* () const
		{
		return (intf_type*) intf;
		}
	intf_type* operator ->() const
		{
		return (intf_type*) intf;
		}
#if DIRECTDRAW_VERSION >= 0x0600
	virtual const IID& InterfaceID() const{	return IID_IDirect3DRMTexture3; }
#elif DIRECTDRAW_VERSION >= 0x0500
	virtual const IID& InterfaceID() const{	return IID_IDirect3DRMTexture2; }
#else
	virtual const IID& InterfaceID() const{	return IID_IDirect3DRMTexture; }
#endif
	virtual const CLSID& ClassID() const
		{
		return CLSID_CDirect3DRMTexture;
		}
	};
//-----------------------------------------------------------------------------
//	WXRMMaterial	���b�V���̍ޗ��i������j��ێ�����
//		intf_type �̑���Ɏg����
class WXRMMaterial: public WXRMObject{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMMaterial2 intf_type;
#else
	typedef IDirect3DRMMaterial intf_type;
#endif
protected:
	intf_type*& PMaterial() const{ return (intf_type*&) intf; }
public:
	WXRMMaterial():WXRMObject(){}
	WXRMMaterial(const WXRMMaterial& m):WXRMObject(m){}
	WXRMMaterial(intf_type* m):WXRMObject(m){}
	WXRMMaterial(const WXD3Vec3& emissive, const WXD3Vec3& specular=WXD3Vec3(1,1,1), REAL power=1);
	void Set(const WXD3Vec3& emission=WXD3Vec3(), const WXD3Vec3& specular=WXD3Vec3(1,1,1), REAL power=1);
		//	���˗��̐ݒ� intf_type ���Q��
		//	���˓���(emissive)	�����Ȃ��Ă�������F
		//	���˓���(specular)	�n�C���C�g�̐F
		//	���x(power)			�n�C���C�g�̉s��(�傫���قǋ��ɋ߂�)
	operator intf_type* () const{ return (intf_type*) intf; }
	virtual const IID& InterfaceID() const;
	virtual const CLSID& ClassID() const;
};
//-----------------------------------------------------------------------------
//	WXRMMesh	�|���S���f�[�^�i���b�V���f�[�^�j��ێ�����
//		IDirect3DRMeshBuilder �̑���Ɏg����
//
//	���b�V���͖ʂƒ��_�ō\������Ă���B�ʂ͒��_�̃C���f�b�N�X�����ɂ����Ȃ��B
//	�����̖ʂ����_�����L����ꍇ�A����ʂ̒��_�ւ̑���͂��̒��_�����L���鑼�̖�
//	�ɂ��y�ԁB�e�N�X�`���̐ݒ�Ȃǂ̑��삪����ɂ�����B
//	
//	WXRMVertex	���b�V���̒��_
//
class WXRMVertices;
class WXRMVertex
	{
	public:
	WXRMVertex(const WXD3Vec3& p=WXD3Vec3(), const WXD3Vec3& n=WXD3Vec3());
	WXRMVertex(const WXRMVertex& v);
	WXD3Vec3 Pos() const{return (WXD3Vec3&)pos;}									//	���_�̈ʒu
	WXD3Vec3 Normal() const{return (WXD3Vec3&)normal;}							//	���_�̖@���x�N�g��
	void Pos(const WXD3Vec3& p);												//	���_�̈ʒu��ݒ�
	void Normal(const WXD3Vec3& n);											//	���_�̖@���x�N�g����ݒ�
	bool Set(D3DCOLOR col);													//	�F��ݒ�
	const WXRMVertex operator = (const WXRMVertex& v);
	protected:
	WXRMVertices* vertices;	
	int id;
	D3DVECTOR pos, normal;
	friend WXRMVertices;
	};
//
//	WXRMVertices	���b�V�����̑S���_
//
class WXRMVertices
	{
	public:
	~WXRMVertices();
	WXRMVertex operator[](UINT id);					//	id �Ԗڂ̒��_
	UINT Size();									//	���b�V���̒��_����Ԃ�
	void Size(UINT s);								//	���b�V���̒��_����s�ȏ�ɂ���
	virtual int Add(const WXRMVertex& vtx=WXRMVertex());
													//	���b�V���ɒ��_��ǉ�
	protected:
	WXRMMesh* pCMesh;
	D3DVECTOR* pPos;
	D3DVECTOR* pNor;
	DWORD nPos, nNor;
	WXRMVertices();
	void Create();
	void Release();
	void SetPos(int id, const D3DVECTOR& pos);
	void SetNormal(int id, const D3DVECTOR& nor);
	friend WXRMVertex;
	friend WXRMMesh;
	};
//
//	WXRMFace	���b�V���̖�
//
struct WXRMFaces;
class WXRMMesh;
class WXRMFace{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMFace2 intf_type;
	static IID InterfaceID(){ return IID_IDirect3DRMFace2; }
#else
	typedef IDirect3DRMFace intf_type;
	static IID InterfaceID(){ return IID_IDirect3DRMFace; }
#endif
	class Vertices{
		public:
		WXRMVertex operator[](UINT id);	//	�ʂ̒��_
		UINT Size();					//	�ʂ̒��_��
		bool Add(const WXRMVertex& vtx);	//	�ʂɐV�������_��ǉ�(���b�V���ɂ��ǉ������)
		bool Add(int id);				//	�ʂɃ��b�V������ id �Ԗڂ̒��_��ǉ�(���b�V���ɂ��ǉ������)
		protected:
		WXRMFace* face;
		friend WXRMFace;
	} vertices;
	WXRMFace();
	WXRMFace(WXRMFace&);
	~WXRMFace();
	bool Set(D3DCOLOR col);					//	�ʂ̐F��ݒ�
	bool Set(WXRMMaterial::intf_type* m);	//	�ʂ̃}�e���A����ݒ�
	bool Set(WXRMTexture::intf_type* t);		//	�ʂ̃e�N�X�`����ݒ�
	bool Add(const WXRMVertex& vtx=WXRMVertex());
											//	�ʂɐV�������_��ǉ�(���b�V���ɂ��ǉ������)
	bool Add(int id);						//	�ʂɃ��b�V������id �Ԗڂ̒��_��ǉ�
	protected:
	WXRMFace(WXRMMesh& mesh, intf_type* pf);
	WXRMMesh* pCMesh;
	intf_type* pFace;
	friend Vertices;
	friend WXRMFaces;	
	friend WXRMMesh;
	};
//
//	WXRMFaces	���b�V�����̑S�Ă̖�
//
struct WXRMFaces
	{
	WXRMFace operator [](UINT id);				//	���b�V������ id �Ԗڂ̖�
	UINT Size();								//	���b�V�����̖ʐ�
	void Size(UINT s);							//	���b�V�����̖ʐ��� s �ȏ�ɂ���
	virtual bool Add(const WXRMFace& face=WXRMFace());
												//	���b�V���ɖʂ�ǉ�
	protected:
	IDirect3DRMFaceArray* pFaces;
	WXRMMesh* pCMesh;
	WXRMFaces();
	~WXRMFaces();
	void Create();
	friend WXRMMesh;
	};
//
//	WXRMMesh	���b�V���f�[�^��ێ�����
//		IDirect3DRMeshBuilder �̑���Ɏg����
class WXRMMesh: public WXRMVisual{
public:
#if DIRECTDRAW_VERSION >= 0x0600
	typedef IDirect3DRMMeshBuilder3 intf_type;
#elif DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DRMMeshBuilder2 intf_type;
#else
	typedef IDirect3DRMMeshBuilder intf_type;
#endif
protected:
	intf_type*& PMeshB() const {return (intf_type*&) intf;}
	bool CheckCreate();
	virtual void UpdateMesh(){};
	public:
	WXRMFaces faces;								//	���b�V�����̑S�Ă̖�
	WXRMVertices vertices;						//	���b�V�����̑S���_
	WXRMMesh(const WXRMMesh& m);
	WXRMMesh(intf_type* m);
	WXRMMesh();
	WXRMMesh(std::string fname);					//	.X�t�@�C�����烍�[�h����
	virtual ~WXRMMesh();
	virtual bool Create(){return CheckCreate();}
	virtual bool Load(const std::string fname);	//	.X�t�@�C�����烍�[�h����
	virtual bool Save(const std::string fname, D3DRMXOFFORMAT fmt=D3DRMXOF_TEXT, D3DRMSAVEOPTIONS opt=D3DRMXOFSAVE_ALL);
												//	.X�t�@�C���ɃZ�[�u����
	virtual bool Set(D3DCOLOR col);				//	�|���S���̐F��ݒ肷��
	virtual bool Set(D3DRMCOLORSOURCE colSrc);
	virtual bool Set(WXRMMaterial::intf_type* m);//	�|���S���̔��˗���ݒ肷��
	virtual bool Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, const WXD3Affine& af, Vec2f texO=Vec2f(), Vec2f texS=Vec2f(1,1));
		//	�e�N�X�`���}�b�s���O��ݒ肷��
	virtual bool CalcWrap(WXD3Affine& af, Vec2f& texS, D3DRMWRAPTYPE wt, char axis1, char axis2);
		//	�e�N�X�`���}�b�s���O�ݒ�p�����[�^ af, texS ���v�Z����
	virtual bool Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, char axis1='x', char axis2='+', Vec2f texO=Vec2f());
		//	�e�N�X�`���}�b�s���O��ݒ肷��(CalcWrap���g�p)
	virtual bool Scale(const WXD3Vec3& s);		//	���b�V����s�{�Ɋg�傷��
	virtual bool Pos(const WXD3Vec3& p);		//	���b�V����p�������s�ړ�����
	virtual bool GetBox(WXD3Vec3& min, WXD3Vec3& max);
												//	���b�V�����܂ޒ����̂̒��_�����߂�
	virtual bool Add(const WXRMFace& face);		//	���b�V���ɖʂ�ǉ�
	virtual int Add(const WXRMVertex& vtx);		//	���b�V���ɒ��_��ǉ�
	operator intf_type* () const
		{
		return (intf_type*)intf;
		}
	virtual const IID& InterfaceID() const;
	virtual const CLSID& ClassID() const;
	friend class WXRMFace;
	friend class WXRMFace::Vertices;
	friend struct WXRMFaces;
	friend class WXRMVertex;
	friend class WXRMVertices;
};
}	//	namespace Spr

#endif