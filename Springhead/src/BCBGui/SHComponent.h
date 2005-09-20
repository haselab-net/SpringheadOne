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

/**	�V�[���O���t�̃I�u�W�F�N�g��C++Builder GUI�p�̃��b�p�[�D
*/
class PACKAGE TSHComponent: public TComponent{
protected:
	Spr::UTRef<Spr::SGObject> imp;
	TSHScene* scene;
	virtual void __fastcall SetObject(Spr::SGObject* o);
	virtual Spr::SGObject* __fastcall GetObject(){ return imp; }
	void __fastcall TSHComponent::Loaded();
public:
	///	�R���X�g���N�^
	__fastcall TSHComponent(TComponent* AOwner):
		TComponent(AOwner), scene(NULL), imp(NULL){}
	///	�f�X�g���N�^
	virtual __fastcall ~TSHComponent(){
		if (imp) imp->userData = NULL;
	}
	///	���O�̐ݒ�
	virtual void __fastcall SetName(const AnsiString NewName);
	///	�ύX�̒ʒm
	void __fastcall NotifyChange(TObject* o=NULL);

	__property Spr::SGObject* Imp = {read=GetObject, write=SetObject};
__published:
	///	�V�[���O���t�R���|�[�l���g
	__property TSHScene* Scene = {read=scene, write=scene};
	///	�R���|�[�l���g�̖��O���V�[���O���t�ł̖��O
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
