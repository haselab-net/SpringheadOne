#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <vcl.h>
#include <DesignEditors.hpp>

#include "SHPTMPropEdit.h"
#include "SHAffine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TSHAffinefDummy::Assign(TPersistent* src)
{
	if (src->InheritsFrom( __classid(TSHAffinefDummy) )){
		taff= ((TSHAffinefDummy*)src)->taff;
	}
}

//---------------------------------------------------------------------------
namespace Shaffine
{
char ex[] = "Ex";
char ey[] = "Ey";
char ez[] = "Ez";
char pos[] = "Pos";
	void __fastcall PACKAGE Register(){
		//	ExDummyのプロパティエディタを登録
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHAffinefDummy),
		"ExDummy", __classid(TVectorProperty<ex>));
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHAffinefDummy),
		"EyDummy", __classid(TVectorProperty<ey>));
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHAffinefDummy),
		"EzDummy", __classid(TVectorProperty<ez>));
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHAffinefDummy),
		"PosDummy", __classid(TVectorProperty<pos>));
	}
}

