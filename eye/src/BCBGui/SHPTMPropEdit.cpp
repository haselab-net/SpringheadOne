#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <vcl.h>
#include "SHPTMPropEdit.h"
#include "SHAffine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Shptmpropedit{
//---------------------------------------------------------------------------
char ExStr[]= "Ex";
char EyStr[]= "Ey";
char EzStr[]= "Ez";
char TrnStr[]= "Trn";

void __fastcall PACKAGE Register()
{
  //TSHMatrix3fDummy::ExDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHMatrix3fDummy),
	"ExDummy", __classid(TVectorProperty<ExStr>));
  //TSHMatrix3fDummy::EyDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHMatrix3fDummy),
	"EyDummy", __classid(TVectorProperty<EyStr>));
  //TSHMatrix3fDummy::EzDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHMatrix3fDummy),
	"EzDummy", __classid(TVectorProperty<EzStr>));


  //TSHAffinefDummy::ExDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHAffinefDummy),
	"ExDummy", __classid(TVectorProperty<ExStr>));
  //TSHAffinefDummy::EyDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHAffinefDummy),
	"EyDummy", __classid(TVectorProperty<EyStr>));
  //TSHAffinefDummy::EzDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHAffinefDummy),
	"EzDummy", __classid(TVectorProperty<EzStr>));
  //TSHAffinefDummy::TrnDummyのプロパティエディタを登録
  RegisterPropertyEditor(__typeinfo(TSHVec3fDummy),
	__classid(TSHAffinefDummy),
	"TrnDummy", __classid(TVectorProperty<TrnStr>));
}

//---------------------------------------------------------------------------
}//namespace Shptmpropedit
//---------------------------------------------------------------------------
