#ifndef LVRM_DX_CUNKNOWN_H
#define LVRM_DX_CUNKNOWN_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#undef  __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 475 )

#include "WinBasis/WBWin.h"
#include <Base/utilities.h>

namespace LVRM {

template <class T>
void RELEASE(T& x){
	if (x){
		x->Release();
		x = NULL;
	}
}

///	IUnknown のラッパ
class WXDXBase {
protected:
	///	IUnknownへのポインタ
	IUnknown* intf;

public:
	///	コピーコンストラクタ，u がNULLでなければ参照カウンタを増やす(IUnknown::AddRefを呼ぶ)
	WXDXBase(const WXDXBase& u);
	///	構築，pObj がNULLでなければ参照カウンタを増やす(IUnknown::AddRefを呼ぶ)
	WXDXBase(IUnknown* i = NULL);

	///	破棄，参照を減らす(IUnknown::Releaseを呼ぶ)
	virtual ~WXDXBase();

	///	代入，Release()後，pObject=o.pObjectとし，参照を増やす．
	WXDXBase& operator = (const WXDXBase& o);

	///	COMインターフェイスのGUID
	virtual const IID& InterfaceID() const;

	///	COMクラスのGUID
	virtual const CLSID& ClassID() const;
	///	IUnknown への参照を減らし，pObject=NULLとする．
	virtual void Release();
};
template <class IF, const IID* iid>
class CDXInterface: public WXDXBase{
public:
	///
	typedef IF intf_type;
	///
	typedef CDXInterface<intf_type, iid> base_type;
	///
	CDXInterface(IF* i = NULL): WXDXBase(i){}
	///
 	IF*& Intf(){ return (IF*&)intf; }
	///
 	IF* Intf() const { return (IF*)intf; }
	///
	IF* operator ->() const { return Intf(); }
	///
	operator IF*() const { return Intf(); }
	///	COMインターフェイスのGUID
	virtual const IID& InterfaceID() const{ return *iid; }
};
#define CDXINTF(OBJ) CDXInterface<I##OBJ, &IID_I##OBJ>

class CDXComInitImp: public UTRefCount{
public:
	CDXComInitImp();
	~CDXComInitImp();
	static CDXComInitImp* comInitImp;
};
class CDXComInit:public UTRef<CDXComInitImp>{
public:
	CDXComInit();
};

}	//	namespace LVRM
#endif