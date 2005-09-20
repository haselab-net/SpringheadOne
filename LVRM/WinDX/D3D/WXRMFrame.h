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

///	IDirect3DRMObjectのラッパクラス
class WXRMObject:public WXUnknown
	{
	protected:
	char* name;
	IDirect3DRMObject*& PRMObj() const
		{
		return (IDirect3DRMObject*&) intf;	//	IDirect3DRMObjectへのポインタ
		}
	virtual IDirect3DRMObject* Clone() const;	//	オブジェクトの複製を作って返す．
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
	//	名前
	const char* Name();
	void Name(const char* n);
	//	Interface's GUID
	virtual const IID& InterfaceID() const;
	bool Check(IUnknown* pUk) const;
	//	演算子
	const WXRMObject& operator = (const WXRMObject& o);
	bool operator == (const WXRMObject& o) const
		{
		return PRMObj() == o.PRMObj();
		}
	};
//----------------------------------------------------------------------------
/**	Direct3DRMのフレーム.
	木構造を作り、ビジュアル（メッシュなど）と光源を保持する.
	木構造の親に対して位置を設定できる.
	IDirect3DRMFrame3の代わりに使える.

  木構造の操作について
	
	注意:親フレームに追加した子フレーム、メッシュ、光源は、親フレームが消えるまで
	メモリ上に保持される。
	
	例:	\begin{verbatim}
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
		fr1.DelThis();				//	fr1をシーンから削除
		}	
		//	自動変数 fr2, mesh は消えるが、シーンに追加されたフレームと
		//	メッシュは残る。
		//	fr1に追加されたメッシュも残るため、fr1のメッシュはBuildScene()を
		//	呼ぶたびに増える。
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
	//	構築
	///	コピーコンストラクタ
	WXRMFrame(const WXRMFrame& f):WXRMObject(f){}
	///	引数なしのコンストラクタ
	WXRMFrame(intf_type* f=NULL):WXRMObject(f){}
	///	ファイルから読み出し
	WXRMFrame(const char* fname);
	bool Create(){
		if (!PFrame()) CreateFrame();
		return PFrame()!=NULL;
	}
	///	.Xファイルから読み出し 木構造を丸ごとファイルからロードする。
	bool Load(const char* fname);
	
	///	intf_type へ変換
	operator intf_type* () const
		{
		return PFrame();
		}
	///	intf_type のメソッドの呼び出し用
	intf_type * operator ->()
		{
		bool b = Create();
		VRASSERT(b);
		return PFrame();
		}
	//	以下フレームの操作
	///	名前が name の子フレームを探す
	WXRMFrame Find(const char* name);
	///	fr が子フレームなら true
	bool Find(const WXRMFrame& fr);
	///	親フレーム
	WXRMFrame Parent();
	///	親フレームの設定
	virtual void Parent(intf_type* pf);
	//	子フレームを追加
	virtual void Child(WXRMFrame& fr){Add(fr);}
	//	子フレームを追加
	virtual void Add(WXRMFrame& fr);
	void Add(const WXRMFrame& fr){WXRMFrame f=fr; Add(fr);}
	///	子フレームを削除
	virtual void Del(const WXRMFrame& fr);
	///	自分を親フレームから削除
	virtual void DelThis();
	///	子フレームの数
	int NChild();
	///	i番目の子フレーム
	WXRMFrame Child(int i);
	//	以下ビジュアル（レンダリング結果が見えるもの、Meshなど）の操作
	///	ビジュアルの追加
	virtual void Add(WXRMVisual& vis);
	void Add(const WXRMVisual& vis);
	///	ビジュアルの削除
	virtual void Del(const WXRMVisual& vis);
	///	ビジュアルの数
	int NVisual();
	///	i番目のビジュアル
	WXRMVisual Visual(int i);
	///	名前が name のビジュアル
	WXRMVisual Visual(const char* name);

	//	以下光源の操作
	///	光源の追加
	virtual void Add(const WXRMLight& light);
	///	光源の削除
	virtual void Del(const WXRMLight& light);
	///	光源の数
	int NLight();
	///	i番目の光源
	WXRMLight Light(int i);
	///	名前が name の光源
	WXRMLight Light(const char* name);

	///	すべての子フレーム、ビジュアル、光源を削除
	virtual void Clear();

	///	速度
	REAL RotAngle();
	WXD3Vec3 RotAxis();
	virtual void Rot(const WXD3Vec3& axis, D3DVALUE rVel);
	WXD3Vec3 Vel();
	virtual void Vel(const WXD3Vec3& vel);

	///	親フレームから見た位置
	virtual void Ori(const WXD3Vec3& ez, const WXD3Vec3& ey, const WXRMFrame& frRef=WXRMFrame());
	virtual void Pos(const WXD3Vec3& v, const WXRMFrame& frRef = WXRMFrame());
	///	親フレームから見たこのフレームの位置
	WXD3Affine AfParent();
	///	親フレームから見たこのフレームの位置の設定
	virtual void AfParent(const WXD3Affine& af);
	///	World座標系から見た位置
	virtual WXD3Affine AfWorld();
	///	World座標系から見た位置の設定
	virtual void AfWorld(const WXD3Affine& af);
	//	その他
	///
	virtual const IID& InterfaceID() const;
	///
	virtual const CLSID& ClassID() const;
	};
}	//	namespace LVRM
#endif