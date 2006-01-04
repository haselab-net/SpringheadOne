#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <basepch.h>

#include <DesignIntf.hpp>
#include "SHSceneGraphDlg.h"
#include "SHPropEdit.h"
#include "SHPTMPropEdit.h"
#include "SHFrame.h"
#include "SHSolid.h"
#include "SHBox.h"
#include "SHCamera.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHFrame *)
{
	new TSHSolid(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHSolid::TSHSolid(TComponent* Owner)
	: TSHComponent(Owner),
    Inertia(&GetInertia, &SetInertia),
    Center(&GetCenter, &SetCenter),
    Velocity(&GetVelocity, &SetVelocity),
    AngularVelocity(&GetAngularVelocity, &SetAngularVelocity)
{
	FInertiaDummy = new TSHMatrix3fDummy(Inertia);
	Inertia.OnChange = &NotifyChange;
	FCenterDummy = new TSHVec3fDummy(Center);
	Center.OnChange = &NotifyChange;
	FVelocityDummy = new TSHVec3fDummy(Velocity);
	Velocity.OnChange = &NotifyChange;
	FAngularVelocityDummy = new TSHVec3fDummy(AngularVelocity);
	AngularVelocity.OnChange = &NotifyChange;
}

void __fastcall TSHSolid::Loaded(){
	if (!Imp){
		Spr::PHSolid* so;
		Scene->Imp->FindObject(so, Name.c_str());
		Imp = so;
	}
}

//---------------------------------------------------------------------------
Spr::SGFrame* __fastcall TSHSolid::GetFrame()
{
	if (Imp) return Imp->GetFrame();
	else return NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSHSolid::SetFrame(Spr::SGFrame* f)
{
	if (Imp) Imp->SetFrame(f);
	NotifyChange();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TSHSolid::GetFrameByName()
{
	if (Imp && Imp->GetFrame())
		return Imp->GetFrame()->GetName();
	else
		return "";
}

//---------------------------------------------------------------------------
void __fastcall TSHSolid::SetFrameByName(AnsiString s)
{
	if (Imp && Scene){
		Spr::SGFrame* fr =
			dynamic_cast<Spr::SGFrame*>(Scene->FindObject(s.c_str()));
		if (fr) Frame= fr;
	}
}


//---------------------------------------------------------------------------

void __fastcall TSHSolid::SetMass(double m)
{
	if (Imp) Imp->SetMass(m);
	NotifyChange();
}
double __fastcall TSHSolid::GetMass()
{
	if (Imp) return Imp->GetMass();
	else return 0.1;
}

//---------------------------------------------------------------------------
namespace Shsolid
{
    char inertia[] = "Inertia";
    char velocity[] = "Velocity";
    char angularvelocity[] = "AngularVelocity";
	void __fastcall PACKAGE Register()
	{
        TComponentClass classes[1] = {__classid(TSHSolid)};
		RegisterComponents("SpringHead", classes, 0);
		//	InertiaDummyのプロパティエディタを登録
		RegisterPropertyEditor(__typeinfo(TSHMatrix3fDummy), __classid(TSHSolid),
		"InertiaDummy", __classid(TMatrixProperty<inertia>));
		//	VelocityDummyのプロパティエディタを登録
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHSolid),
		"VelocityDummy", __classid(TVectorProperty<velocity>));
		//	AngularVelocityDummyのプロパティエディタを登録
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHSolid),
		"AngularVelocityDummy", __classid(TVectorProperty<angularvelocity>));
	}
}
