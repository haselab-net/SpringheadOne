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
//	WXRMLight	光源　フレームにくっつけて使う
//		intf_type の代わりに使える
class WXRMLight: public WXRMObject{
public:
	typedef IDirect3DRMLight intf_type;
protected:
	IDirect3DRMLight*& PLight() const{ return (IDirect3DRMLight*&) intf; }
public:
	WXRMLight();
	WXRMLight(intf_type*  l);
	WXRMLight(const WXRMLight& l);
	WXRMLight(D3DRMLIGHTTYPE lt, D3DCOLOR col);		//	光源の種類と色の設定
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
//	WXRMTexture		テクスチャのビットマップを保持する
//		IDirect3DRMTexture の代わりに使える
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
		//	テクスチャを.BMPファイルから読み出す。ビットマップの大きさは2^n * 2^nでなければならない。
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
//	WXRMMaterial	メッシュの材料（光り方）を保持する
//		intf_type の代わりに使える
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
		//	反射率の設定 intf_type を参照
		//	放射特性(emissive)	光がなくても見える色
		//	反射特性(specular)	ハイライトの色
		//	強度(power)			ハイライトの鋭さ(大きいほど鏡に近い)
	operator intf_type* () const{ return (intf_type*) intf; }
	virtual const IID& InterfaceID() const;
	virtual const CLSID& ClassID() const;
};
//-----------------------------------------------------------------------------
//	WXRMMesh	ポリゴンデータ（メッシュデータ）を保持する
//		IDirect3DRMeshBuilder の代わりに使える
//
//	メッシュは面と頂点で構成されている。面は頂点のインデックスを持つにすぎない。
//	複数の面が頂点を共有する場合、ある面の頂点への操作はその頂点を共有する他の面
//	にも及ぶ。テクスチャの設定などの操作がこれにあたる。
//	
//	WXRMVertex	メッシュの頂点
//
class WXRMVertices;
class WXRMVertex
	{
	public:
	WXRMVertex(const WXD3Vec3& p=WXD3Vec3(), const WXD3Vec3& n=WXD3Vec3());
	WXRMVertex(const WXRMVertex& v);
	WXD3Vec3 Pos() const{return (WXD3Vec3&)pos;}									//	頂点の位置
	WXD3Vec3 Normal() const{return (WXD3Vec3&)normal;}							//	頂点の法線ベクトル
	void Pos(const WXD3Vec3& p);												//	頂点の位置を設定
	void Normal(const WXD3Vec3& n);											//	頂点の法線ベクトルを設定
	bool Set(D3DCOLOR col);													//	色を設定
	const WXRMVertex operator = (const WXRMVertex& v);
	protected:
	WXRMVertices* vertices;	
	int id;
	D3DVECTOR pos, normal;
	friend WXRMVertices;
	};
//
//	WXRMVertices	メッシュ中の全頂点
//
class WXRMVertices
	{
	public:
	~WXRMVertices();
	WXRMVertex operator[](UINT id);					//	id 番目の頂点
	UINT Size();									//	メッシュの頂点数を返す
	void Size(UINT s);								//	メッシュの頂点数をs以上にする
	virtual int Add(const WXRMVertex& vtx=WXRMVertex());
													//	メッシュに頂点を追加
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
//	WXRMFace	メッシュの面
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
		WXRMVertex operator[](UINT id);	//	面の頂点
		UINT Size();					//	面の頂点数
		bool Add(const WXRMVertex& vtx);	//	面に新しい頂点を追加(メッシュにも追加される)
		bool Add(int id);				//	面にメッシュ中の id 番目の頂点を追加(メッシュにも追加される)
		protected:
		WXRMFace* face;
		friend WXRMFace;
	} vertices;
	WXRMFace();
	WXRMFace(WXRMFace&);
	~WXRMFace();
	bool Set(D3DCOLOR col);					//	面の色を設定
	bool Set(WXRMMaterial::intf_type* m);	//	面のマテリアルを設定
	bool Set(WXRMTexture::intf_type* t);		//	面のテクスチャを設定
	bool Add(const WXRMVertex& vtx=WXRMVertex());
											//	面に新しい頂点を追加(メッシュにも追加される)
	bool Add(int id);						//	面にメッシュ中のid 番目の頂点を追加
	protected:
	WXRMFace(WXRMMesh& mesh, intf_type* pf);
	WXRMMesh* pCMesh;
	intf_type* pFace;
	friend Vertices;
	friend WXRMFaces;	
	friend WXRMMesh;
	};
//
//	WXRMFaces	メッシュ中の全ての面
//
struct WXRMFaces
	{
	WXRMFace operator [](UINT id);				//	メッシュ中の id 番目の面
	UINT Size();								//	メッシュ中の面数
	void Size(UINT s);							//	メッシュ中の面数を s 以上にする
	virtual bool Add(const WXRMFace& face=WXRMFace());
												//	メッシュに面を追加
	protected:
	IDirect3DRMFaceArray* pFaces;
	WXRMMesh* pCMesh;
	WXRMFaces();
	~WXRMFaces();
	void Create();
	friend WXRMMesh;
	};
//
//	WXRMMesh	メッシュデータを保持する
//		IDirect3DRMeshBuilder の代わりに使える
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
	WXRMFaces faces;								//	メッシュ中の全ての面
	WXRMVertices vertices;						//	メッシュ中の全頂点
	WXRMMesh(const WXRMMesh& m);
	WXRMMesh(intf_type* m);
	WXRMMesh();
	WXRMMesh(std::string fname);					//	.Xファイルからロードする
	virtual ~WXRMMesh();
	virtual bool Create(){return CheckCreate();}
	virtual bool Load(const std::string fname);	//	.Xファイルからロードする
	virtual bool Save(const std::string fname, D3DRMXOFFORMAT fmt=D3DRMXOF_TEXT, D3DRMSAVEOPTIONS opt=D3DRMXOFSAVE_ALL);
												//	.Xファイルにセーブする
	virtual bool Set(D3DCOLOR col);				//	ポリゴンの色を設定する
	virtual bool Set(D3DRMCOLORSOURCE colSrc);
	virtual bool Set(WXRMMaterial::intf_type* m);//	ポリゴンの反射率を設定する
	virtual bool Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, const WXD3Affine& af, Vec2f texO=Vec2f(), Vec2f texS=Vec2f(1,1));
		//	テクスチャマッピングを設定する
	virtual bool CalcWrap(WXD3Affine& af, Vec2f& texS, D3DRMWRAPTYPE wt, char axis1, char axis2);
		//	テクスチャマッピング設定パラメータ af, texS を計算する
	virtual bool Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, char axis1='x', char axis2='+', Vec2f texO=Vec2f());
		//	テクスチャマッピングを設定する(CalcWrapを使用)
	virtual bool Scale(const WXD3Vec3& s);		//	メッシュをs倍に拡大する
	virtual bool Pos(const WXD3Vec3& p);		//	メッシュをpだけ並行移動する
	virtual bool GetBox(WXD3Vec3& min, WXD3Vec3& max);
												//	メッシュを含む直方体の頂点を求める
	virtual bool Add(const WXRMFace& face);		//	メッシュに面を追加
	virtual int Add(const WXRMVertex& vtx);		//	メッシュに頂点を追加
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