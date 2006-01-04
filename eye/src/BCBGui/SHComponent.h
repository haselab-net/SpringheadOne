//---------------------------------------------------------------------------
#ifndef SHOBJECT_H
#define SHOBJECT_H
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include <SceneGraph/SGObject.h>

//---------------------------------------------------------------------------
class __declspec(delphiclass) TSHScene;

/**	シーングラフのオブジェクトのC++Builder GUI用のラッパー．
*/
class PACKAGE TSHComponent: public TComponent{
protected:
	Spr::UTRef<Spr::SGObject> imp;
	TSHScene* scene;
	virtual void __fastcall SetObject(Spr::SGObject* o);
	virtual Spr::SGObject* __fastcall GetObject(){ return imp; }
	void __fastcall TSHComponent::Loaded();
public:
	///	コンストラクタ
	__fastcall TSHComponent(TComponent* AOwner):
		TComponent(AOwner), scene(NULL), imp(NULL){}
	///	デストラクタ
	virtual __fastcall ~TSHComponent(){
		if (imp) imp->userData = NULL;
	}
	///	名前の設定
	virtual void __fastcall SetName(const AnsiString NewName);
	///	変更の通知
	void __fastcall NotifyChange(TObject* o=NULL);

	__property Spr::SGObject* Imp = {read=GetObject, write=SetObject};
__published:
	///	シーングラフコンポーネント
	__property TSHScene* Scene = {read=scene, write=scene};
	///	コンポーネントの名前＝シーングラフでの名前
	__property Name;
};


#define DEFINE_PROPERTY(type, name, value)	\
protected:\
	type __fastcall Get##name() { return value; } \
	void __fastcall Set##name(type a) { value=a; } \
public:\
	__property type name = {read=Get##name, write=Set##name}

#define DEFINE_PROPERTYR(type, name, value)	\
protected:\
	type __fastcall Get##name() { return value; } \
public:\
	__property type name = {read=Get##name}


#define DEFINE_OBJECT_IMP(T)\
private:	\
	virtual void __fastcall SetImp(T* o){ SetObject(o); }	\
	virtual T* __fastcall GetImp(){ return (T*)GetObject(); }	\
public:	\
	__property T* Imp = {read=GetImp, write=SetImp}

//---------------------------------------------------------------------------
#endif
