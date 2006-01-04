//---------------------------------------------------------------------------
#ifndef SHLightH
#define SHLightH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics/GRLight.h>
#include "SHScene.h"
#include "SHVector.h"
//---------------------------------------------------------------------------
enum SHLightType{//光源の種類
	ltPoint/*= D3DLIGHT_POINT*/,//ポイントライト
	ltSpot/*= D3DLIGHT_SPOT*/,//スポットライト
	ltDirectional/*= D3DLIGHT_DIRECTIONAL*/,//ディレクショナルライト
};

//---------------------------------------------------------------------------
//コンポーネント化されたライト
class PACKAGE TSHLight : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::GRLight);
protected:
	TSHVec3fDummy* FPositionDummy;
	TSHVec3fDummy* FDirectionDummy;

	SHLightType __fastcall GetType();
	void __fastcall SetType(SHLightType t);
	TColor __fastcall GetDiffuse();
	void __fastcall SetDiffuse(TColor c);
	TColor __fastcall GetSpecular();
	void __fastcall SetSpecular(TColor c);
	TColor __fastcall GetAmbient();
	void __fastcall SetAmbient(TColor c);
	SHVec3f __fastcall GetPosition();
	void __fastcall SetPosition(SHVec3f v);
	SHVec3f __fastcall GetDirection();
	void __fastcall SetDirection(SHVec3f v);
	float __fastcall GetRange();
	void __fastcall SetRange(float f);
	float __fastcall GetFalloff();
	void __fastcall SetFalloff(float f);
	float __fastcall GetAttenuation0();
	void __fastcall SetAttenuation0(float f);
	float __fastcall GetAttenuation1();
	void __fastcall SetAttenuation1(float f);
	float __fastcall GetAttenuation2();
	void __fastcall SetAttenuation2(float f);
	float __fastcall GetTheta();
	void __fastcall SetTheta(float f);
	float __fastcall GetPhi();
	void __fastcall SetPhi(float f);
	Spr::SGFrame* __fastcall GetTargetFrame();
	void __fastcall SetTargetFrame(Spr::SGFrame* f);
	AnsiString __fastcall GetTargetFrameByName();
	void __fastcall SetTargetFrameByName(AnsiString s);

	virtual void __fastcall Loaded();

public:
	__fastcall TSHLight(TComponent* Owner);
	static TColor Color(Spr::Vec4f c);//D3DCOLORVALUE→TColor
	static Spr::Vec4f DColor(const TColor& c);//TColor→D3DCOLORVALUE

__published:
	//光源の種類
	__property SHLightType Type= {read=GetType, write=SetType, default=ltPoint, stored=false};
	//ディフューズ色
	__property TColor Diffuse= {read=GetDiffuse, write=SetDiffuse, stored=false};
	//スペキュラ色
	__property TColor Specular= {read=GetSpecular, write=SetSpecular, stored=false};
	//アンビエント色
	__property TColor Ambient= {read=GetAmbient, write=SetAmbient, stored=false};
	//光の影響範囲
	__property float Range= {read=GetRange, write=SetRange, stored=false};
	//スポットライトの周囲の減衰の度合い
	__property float Falloff= {read=GetFalloff, write=SetFalloff, stored=false};
	//距離による減衰の度合いのパラメータ
	__property float Attenuation0= {read=GetAttenuation0, write=SetAttenuation0, stored=false};
	__property float Attenuation1= {read=GetAttenuation1, write=SetAttenuation1, stored=false};
	__property float Attenuation2= {read=GetAttenuation2, write=SetAttenuation2, stored=false};
	//スポットライトの減衰しない範囲の角度
	__property float Theta= {read=GetTheta, write=SetTheta, stored=false};
	//スポットライトの照射角
	__property float Phi= {read=GetPhi, write=SetPhi, stored=false};

};
//---------------------------------------------------------------------------
#endif
