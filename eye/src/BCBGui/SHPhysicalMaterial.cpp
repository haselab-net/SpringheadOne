#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <basepch.h>

#include <DesignIntf.hpp>
#include "SHSceneGraphDlg.h"
#include "SHPropEdit.h"
#include "SHPTMPropEdit.h"
#include "SHPhysicalMaterial.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHFrame *)
{
	new TSHPhysicalMaterial(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHPhysicalMaterial::TSHPhysicalMaterial(TComponent* Owner): TSHComponent(Owner){
}

void __fastcall TSHPhysicalMaterial::Loaded(){
	if (!Imp){
		Spr::PHPhysicalMaterial* pm;
		Scene->Imp->FindObject(pm, Name.c_str());
		Imp = pm;
	}
}

//---------------------------------------------------------------------------

float __fastcall TSHPhysicalMaterial::GetReflexSpring()
{
	if (Imp) return Imp->pMatData.reflexSpring;
	else return 1.0;
}
void __fastcall TSHPhysicalMaterial::SetReflexSpring(float f)
{
	if (Imp) Imp->pMatData.reflexSpring = f;
	NotifyChange();
}

float __fastcall TSHPhysicalMaterial::GetReflexDamper()
{
	if (Imp) return Imp->pMatData.reflexDamper;
	else return 1.0;
}
void __fastcall TSHPhysicalMaterial::SetReflexDamper(float f)
{
	if (Imp) Imp->pMatData.reflexDamper = f;
	NotifyChange();
}

float __fastcall TSHPhysicalMaterial::GetFrictionSpring()
{
	if (Imp) return Imp->pMatData.frictionSpring;
	else return 1.0;
}
void __fastcall TSHPhysicalMaterial::SetFrictionSpring(float f)
{
	if (Imp) Imp->pMatData.frictionSpring = f;
	NotifyChange();
}

float __fastcall TSHPhysicalMaterial::GetFrictionDamper()
{
	if (Imp) return Imp->pMatData.frictionDamper;
	else return 1.0;
}
void __fastcall TSHPhysicalMaterial::SetFrictionDamper(float f)
{
	if (Imp) Imp->pMatData.frictionDamper = f;
	NotifyChange();
}

float __fastcall TSHPhysicalMaterial::GetStaticFriction()
{
	if (Imp) return Imp->pMatData.staticFriction;
	else return 0.6;
}
void __fastcall TSHPhysicalMaterial::SetStaticFriction(float f)
{
	if (Imp) Imp->pMatData.staticFriction = f;
	NotifyChange();
}

float __fastcall TSHPhysicalMaterial::GetDynamicFriction()
{
	if (Imp) return Imp->pMatData.dynamicFriction;
	else return 0.3;
}
void __fastcall TSHPhysicalMaterial::SetDynamicFriction(float f)
{
	if (Imp) Imp->pMatData.dynamicFriction = f;
	NotifyChange();
}

//---------------------------------------------------------------------------
namespace Shphysicalmaterial
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHPhysicalMaterial)};
		RegisterComponents("SpringHead", classes, 0);
	}
}
//---------------------------------------------------------------------------



