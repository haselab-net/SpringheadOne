#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <vcl.h>
#include <DesignEditors.hpp>

#include "SHPTMPropEdit.h"
#include "SHMatrix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TSHMatrix3fDummy::Assign(TPersistent* src)
{
	if (src->InheritsFrom( __classid(TSHMatrix3fDummy) )){
		tmat= ((TSHMatrix3fDummy*)src)->tmat;
	}
}

//---------------------------------------------------------------------------
namespace Shmatrix3
{
    char ex[] = "Ex";
    char ey[] = "Ey";
    char ez[] = "Ez";

	void __fastcall PACKAGE Register(){
		//	プロパティエディタを登録
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHMatrix3fDummy),
		"ExDummy", __classid(TVectorProperty<ex>));
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHMatrix3fDummy),
		"EyDummy", __classid(TVectorProperty<ey>));
		RegisterPropertyEditor(__typeinfo(TSHVec3fDummy), __classid(TSHMatrix3fDummy),
		"EzDummy", __classid(TVectorProperty<ez>));
	}
}

