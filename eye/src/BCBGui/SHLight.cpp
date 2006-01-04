#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <basepch.h>

#include <DesignIntf.hpp>
#include "SHSceneGraphDlg.h"
#include "SHPropEdit.h"
#include "SHPTMPropEdit.h"
#include "SHFrame.h"
#include "SHLight.h"
#include "SHBox.h"
#include "SHCamera.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHFrame *)
{
	new TSHLight(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHLight::TSHLight(TComponent* Owner): TSHComponent(Owner){
}

void __fastcall TSHLight::Loaded(){
	if (!Imp){
		Spr::GRLight* li;
		Scene->Imp->FindObject(li, Name.c_str());
		Imp = li;
	}
}

//---------------------------------------------------------------------------
//D3DCOLORVALUE→TColor
TColor TSHLight::Color(Spr::Vec4f c)
{
	typedef unsigned int uint;
	return (TColor)((uint)(c.x*255)+((uint)(c.y*255)<<8)+((uint)(c.z*255)<<16)+((uint)(c.w*255)<<24));
}

//---------------------------------------------------------------------------
//TColor→D3DCOLORVALUE
Spr::Vec4f TSHLight::DColor(const TColor& c)
{
	//システムカラーには未対応...
	unsigned int i= (unsigned int)c;
	Spr::Vec4f v;
	v.x= (i&0xff)/255.0;
	v.y= ((i>>8)&0xff)/255.0;
	v.z= ((i>>16)&0xff)/255.0;
	v.w= 1.0;
	return v;
}

//---------------------------------------------------------------------------
SHLightType __fastcall TSHLight::GetType()
{
	if (Imp){
		if (Imp->position.W() == 0) return ltDirectional;
		if (Imp->spotCutoff == M_PI) return ltPoint;
		return ltSpot;
	}
	return ltPoint;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetType(SHLightType t)
{
	if (Imp){
		switch (t){
			case ltPoint: Imp->position.W()=1; Imp->spotCutoff=M_PI; break;
			case ltSpot: Imp->position.W()=1; Imp->spotCutoff=M_PI/4; break;
			case ltDirectional: Imp->position.W()=0; break;
		}
	}
	NotifyChange();
}

//---------------------------------------------------------------------------
TColor __fastcall TSHLight::GetDiffuse(){
	if (Imp) return Color(Imp->diffuse);
	else return clWhite;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetDiffuse(TColor c)
{
	if (Imp) Imp->diffuse= DColor(c);
	NotifyChange();
}

//---------------------------------------------------------------------------
TColor __fastcall TSHLight::GetSpecular()
{
	if (Imp) return Color(Imp->specular);
	else return clWhite;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetSpecular(TColor c)
{
	if (Imp) Imp->specular= DColor(c);
	NotifyChange();
}

//---------------------------------------------------------------------------
TColor __fastcall TSHLight::GetAmbient()
{
	if (Imp) return Color(Imp->ambient);
	else return clWhite;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetAmbient(TColor c)
{
	if (Imp) Imp->ambient= DColor(c);
	NotifyChange();
}

//---------------------------------------------------------------------------
SHVec3f __fastcall TSHLight::GetPosition()
{
	if (Imp) return (const SHVec3f&)Imp->position;
	else return SHVec3f();
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetPosition(SHVec3f v)
{
	if (Imp) Imp->position.sub_vector(PTM::TSubVectorDim<0,3>()) = v;
	NotifyChange();
}

//---------------------------------------------------------------------------
SHVec3f __fastcall TSHLight::GetDirection()
{
	if (Imp) {
		if (Imp->position.W() == 0) return (const SHVec3f&)Imp->position;
		else return Imp->spotDirection;
	}
	else return SHVec3f();
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetDirection(SHVec3f v)
{
	if (Imp) {
		if (Imp->position.W() == 0) Imp->position.sub_vector(PTM::TSubVectorDim<0,3>()) = v;
		else Imp->spotDirection = v;
	}
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetRange()
{
	if (Imp) return Imp->range;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetRange(float f)
{
	if (Imp) Imp->range= f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetFalloff()
{
	if (Imp) return Imp->spotFalloff;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetFalloff(float f)
{
	if (Imp) Imp->spotFalloff = f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetAttenuation0()
{
	if (Imp) return Imp->attenuation0;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetAttenuation0(float f)
{
	if (Imp) Imp->attenuation0= f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetAttenuation1()
{
	if (Imp) return Imp->attenuation1;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetAttenuation1(float f)
{
	if (Imp) Imp->attenuation1= f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetAttenuation2()
{
	if (Imp) return Imp->attenuation2;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetAttenuation2(float f)
{
	if (Imp) Imp->attenuation2= f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetTheta()
{
	if (Imp) return Imp->spotInner;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetTheta(float f)
{
	if (Imp) Imp->spotInner= f;
	NotifyChange();
}

//---------------------------------------------------------------------------
float __fastcall TSHLight::GetPhi()
{
	if (Imp) return Imp->spotCutoff;
	else return 0.0;
}

//---------------------------------------------------------------------------
void __fastcall TSHLight::SetPhi(float f)
{
	if (Imp) Imp->spotCutoff= f;
	NotifyChange();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace Shlight
{
	char position[] = "Position";
	char direction[] = "Direction";
	char targetframe[] = "TargetFrame";
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHLight)};
		RegisterComponents("SpringHead", classes, 0);
		//	TargetDummyのプロパティエディタを登録
/*
		RegisterPropertyEditor(__typeinfo(AnsiString),
			__classid(TSHLight), "TargetFrameDummy",
			__classid(TNamedProperty<targetframe, TStringProperty>));
*/
	}
}
//---------------------------------------------------------------------------



