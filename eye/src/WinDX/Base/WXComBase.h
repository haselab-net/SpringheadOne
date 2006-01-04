#ifndef Spr_DX_CUNKNOWN_H
#define Spr_DX_CUNKNOWN_H
#if _MSC_VER >= 1000
# pragma once
#endif // _MSC_VER >= 1000

#undef  __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 475 )

/**	使用するDirectXのバージョン(Dx8以降，7以前はWinDXOldが対応予定)
	ここを書き換えたら，WinDXLib.bat を書き換えてください．
	また，SpringheadGUI.bpg のライブラリも変更してください．*/
#ifndef SPR_DXVER
# define SPR_DXVER 9
#endif

#include <WinBasis/WBWin.h>
#include <Base/BaseUtility.h>

#ifdef __BORLANDC__
#define sqrtf sqrt
#endif

namespace Spr {

template <class T>
void RELEASE(T& x){
	if (x){
		x->Release();
		x = NULL;
	}
}

/**	IUnknown のラッパ
	
*/
class SPR_DLL WXUnknown{
protected:
	///	IUnknownへのポインタ
	IUnknown* intf;

public:
	///	コピーコンストラクタ，u がNULLでなければ参照カウンタを増やす(IUnknown::AddRefを呼ぶ)
	WXUnknown(const WXUnknown& u);
	///	構築，pObj がNULLでなければ参照カウンタを増やす(IUnknown::AddRefを呼ぶ)
	WXUnknown(IUnknown* i = NULL);

	///	破棄，参照を減らす(IUnknown::Releaseを呼ぶ)
	virtual ~WXUnknown();

	///	代入，Release()後，pObject=o.pObjectとし，参照を増やす．
	WXUnknown& operator = (const WXUnknown& o);

	///	COMインターフェイスのGUID
	virtual const IID& InterfaceID() const;

	///	COMクラスのGUID
	virtual const CLSID& ClassID() const;
	///	IUnknown への参照を減らし，pObject=NULLとする．
	virtual void Release();
};

#define WXINTF(OBJ) WXInterface<I##OBJ, &IID_I##OBJ>
#if SPR_DXVER==8
# define WXINTFV(OBJ) WXInterface<I##OBJ##8, &IID_I##OBJ##8>
#elif SPR_DXVER==9
# define WXINTFV(OBJ) WXInterface<I##OBJ##9, &IID_I##OBJ##9>
#endif

/**	COMインタフェースのラッパ．
*/
template <class IF, const IID* iid>
class WXInterface: public WXUnknown{
public:
	///
	typedef IF intf_type;
	///
	typedef WXInterface<intf_type, iid> base_type;
	///
	WXInterface(IF* i = NULL): WXUnknown(i){}
	///
	WXInterface(IUnknown* i):WXUnknown(NULL){
		if (i) i->QueryInterface(iid, &Intf());
	}
	///
 	IF*& Intf(){ return (IF*&)intf; }
	///
 	IF* Intf() const { return (IF*)intf; }
	///
	IF* operator ->() const { return Intf(); }
	///
	operator IF*() const { return Intf(); }
	///
	WXInterface<intf_type, iid>& operator =(intf_type* t){
		if (Intf()) Intf()->Release();
		Intf() = t;
		if (Intf()) Intf()->AddRef();
		return *this;
	}
	///	COMインターフェイスのGUID
	virtual const IID& InterfaceID() const{ return *iid; }
	///
	const IID& InterfaceIDStatic(){ return *iid; }
};


#define WXSINGLETON(OBJ) WXSingletonInterface<I##OBJ, &IID_I##OBJ>
#if SPR_DXVER==8
#define WXSINGLETONV(OBJ) WXSingletonInterface<I##OBJ##8, &IID_I##OBJ##8>
#elif SPR_DXVER==9
#define WXSINGLETONV(OBJ) WXSingletonInterface<I##OBJ##9, &IID_I##OBJ##9>
#endif
/**	シングルトン用ラッパー．
	プログラム全体で確実にひとつだけ実体が生成される．
	メインより前でも確実に使用できる．
	プログラム終了時に開放される．
*/
template <class IF, const IID* iid>
class WXSingletonInterface:public WXInterface<IF, iid>{
public:
	///	コンストラクタ
	void Prepare(){
		if (Intf()) return;
		WXInterface<IF,iid>& singleton = Singleton< WXInterface<IF,iid> >();
		if (!singleton.Intf()){
			singleton.Intf() = (IF*)Create();
		}
		Intf() = singleton.Intf();
		if (Intf()) Intf()->AddRef();
	}
	/**	インタフェースを生成して返す関数．
		プログラム全体で1度だけ呼ばれる可能性がある．
	*/
	virtual IUnknown* Create()=0;
};

class SPR_DLL WXComInitImp: public UTRefCount{
public:
	WXComInitImp();
	~WXComInitImp();
	static WXComInitImp* comInitImp;
};
class SPR_DLL WXComInit:public UTRef<WXComInitImp>{
public:
	WXComInit();
};

}	//	namespace Spr
#endif