//---------------------------------------------------------------------------
#ifndef SHPhysicalMaterialH
#define SHPhysicalMaterialH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Physics/PHPhysicalMaterial.h>
#include "SHScene.h"
//---------------------------------------------------------------------------
//�R���|�[�l���g�����ꂽ�����}�e���A��
class PACKAGE TSHPhysicalMaterial : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::PHPhysicalMaterial);
protected:

	//Get��Set
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
	//�v���p�e�B
	///	�R�͂̂΂ˌW��
	__property float ReflexSpring = {read=GetReflexSpring, write=SetReflexSpring, stored=false};
	///	�R�͂̃_���p�W��
	__property float ReflexDamper = {read=GetReflexDamper, write=SetReflexDamper, stored=false};
	///	���C�͂̂΂ˌW��
	__property float FrictionSpring = {read=GetFrictionSpring, write=SetFrictionSpring, stored=false};
	///	���C�͂̃_���p�W��
	__property float FrictionDamper = {read=GetFrictionDamper, write=SetFrictionDamper, stored=false};
	///	�ő�Î~���C�W��
	__property float StaticFriction = {read=GetStaticFriction, write=SetStaticFriction, stored=false};
	///	�����C�W��
	__property float DynamicFriction = {read=GetDynamicFriction, write=SetDynamicFriction, stored=false};
};
//---------------------------------------------------------------------------
#endif
