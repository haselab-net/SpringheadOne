//---------------------------------------------------------------------------
#ifndef SHPhysicalMaterialH
#define SHPhysicalMaterialH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Physics/PHPhysicalMaterial.h>
#include "SHScene.h"
//---------------------------------------------------------------------------
//コンポーネント化された物理マテリアル
class PACKAGE TSHPhysicalMaterial : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::PHPhysicalMaterial);
protected:

	//GetとSet
	float __fastcall GetReflexSpring();
	void __fastcall SetReflexSpring(float f);
	float __fastcall GetReflexDamper();
	void __fastcall SetReflexDamper(float f);
	float __fastcall GetFrictionSpring();
	void __fastcall SetFrictionSpring(float f);
	float __fastcall GetFrictionDamper();
	void __fastcall SetFrictionDamper(float f);
	float __fastcall GetStaticFriction();
	void __fastcall SetStaticFriction(float f);
	float __fastcall GetDynamicFriction();
	void __fastcall SetDynamicFriction(float f);

	virtual void __fastcall Loaded();

public:
	__fastcall TSHPhysicalMaterial(TComponent* Owner);

__published:
	//プロパティ
	///	抗力のばね係数
	__property float ReflexSpring = {read=GetReflexSpring, write=SetReflexSpring, stored=false};
	///	抗力のダンパ係数
	__property float ReflexDamper = {read=GetReflexDamper, write=SetReflexDamper, stored=false};
	///	摩擦力のばね係数
	__property float FrictionSpring = {read=GetFrictionSpring, write=SetFrictionSpring, stored=false};
	///	摩擦力のダンパ係数
	__property float FrictionDamper = {read=GetFrictionDamper, write=SetFrictionDamper, stored=false};
	///	最大静止摩擦係数
	__property float StaticFriction = {read=GetStaticFriction, write=SetStaticFriction, stored=false};
	///	動摩擦係数
	__property float DynamicFriction = {read=GetDynamicFriction, write=SetDynamicFriction, stored=false};
};
//---------------------------------------------------------------------------
#endif
