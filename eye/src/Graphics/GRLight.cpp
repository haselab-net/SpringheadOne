#include "Graphics.h"
#pragma hdrstop
#include "GRLight.h"
#include <float.h>

namespace Spr {;
GRLightData::GRLightData(){
    range = FLT_MAX;
	attenuation0 = 0.0f;
	attenuation1 = 0.0f;
	attenuation2 = 0.0f;
    spotFalloff = 0.0f;
	spotInner = 0.0f;
	spotCutoff = 0.0f;
}


//----------------------------------------------------------------------------
//	GRLight
SGOBJECTIMP(GRLight, GRVisual);

bool GRLight::AddChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if (fr) frPosture = fr;
	return fr != NULL;
}
size_t GRLight::NReferenceObjects(){
	return frPosture ? 1 : 0;
}
SGObject* GRLight::ReferenceObject(size_t i){
	if (i == 0) return frPosture;
	return NULL;
}

/// ƒ[ƒhŒã‚ÉŒÄ‚Ño‚³‚ê‚é
void GRLight::Loaded(SGScene* scene)
{
}

void GRLight::Render(SGFrame* f, GRRender* render){
	if (frPosture){
		Affinef posture = frPosture->GetWorldPosture();
		if (position.W() == 0){
			position.sub_vector(PTM::TSubVectorDim<0,3>()) = posture.Ez();
		}else{
			position.sub_vector(PTM::TSubVectorDim<0,3>()) = posture.Pos();
			position.W() = 1;
			spotDirection = posture.Ez();
		}
	}
	render->PushLight(*this);
}

void GRLight::Rendered(SGFrame* f, GRRender* render){
	render->PopLight();
}

//	Loader
typedef Vec4f ColorRGBA;
typedef float FLOAT;
typedef Vec3f Vector;
enum XLightType{
    XLIGHT_POINT = 1,
    XLIGHT_SPOT = 2,
    XLIGHT_DIRECTIONAL = 3,
};

DEF_RECORD(XLight8,{
	GUID Guid(){ return WBGuid("8840e4ac-5c01-4e6a-9df9-880a45fb9e56"); }
	DWORD Type;
	ColorRGBA Diffuse;
	ColorRGBA Specular;
	ColorRGBA Ambient;
	Vector Position;
	Vector Direction;
	FLOAT Range;
	FLOAT Falloff;
	FLOAT Attenuation0;
	FLOAT Attenuation1;
	FLOAT Attenuation2;
	FLOAT Theta;
	FLOAT Phi;
});


class GRLightLoader:public FIObjectLoader<GRLight>{
public:
	GRLightLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_FIELD(ColorRGBA);
		db->REG_RECORD_PROTO(XLight8);
	}
	virtual UTString GetNodeType() const{
		return "Light8";
	}
	virtual bool LoadData(FILoadScene* ctx, GRLight* light){
		XLight8 xl;
		ctx->docs.Top()->GetWholeData(xl);

		light->diffuse = xl.Diffuse;
		light->specular = xl.Specular;
		light->ambient = xl.Ambient;
		if (xl.Type == XLIGHT_DIRECTIONAL){
			light->position.sub_vector(PTM::TSubVectorDim<0,3>()) = xl.Direction;
			light->position.W() = 0;
		}else{
			light->position.sub_vector(PTM::TSubVectorDim<0,3>()) = xl.Position;
			light->position.W() = 1;
		}
		light->spotDirection = xl.Direction;
		light->range = xl.Range;
		light->spotFalloff = xl.Falloff;
		light->attenuation0 = xl.Attenuation0;
		light->attenuation1 = xl.Attenuation1;
		light->attenuation2 = xl.Attenuation2;
		light->spotInner = xl.Theta;
		light->spotCutoff = xl.Phi;

		//	¶ŽèŒn¨‰EŽèŒn‚Ì•ÏŠ·
		light->position.Z() *= -1;
		light->spotDirection.Z() *= -1;
		return true;
	}
};

//	Saver

class GRLightSaver:public FIObjectSaver<GRLight>{
public:
	virtual UTString GetType() const{ return "GRLight"; }
	virtual UTString GetNodeType() const{ return "Light8"; }
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, GRLight* light){
		XLight8 xl;
		if (light->position.W() == 0) xl.Type = XLIGHT_DIRECTIONAL;
		else if (light->spotCutoff == M_PI && light->spotInner == M_PI) xl.Type=XLIGHT_POINT;
		else xl.Type = XLIGHT_SPOT;
	    xl.Diffuse = light->diffuse;
	    xl.Specular = light->specular;
	    xl.Ambient = light->ambient;
	    Vec3f pos = light->position.sub_vector(PTM::TSubVectorDim<0,3>());
		if (xl.Type == XLIGHT_DIRECTIONAL){
			xl.Direction = -pos;
		}else{
			pos /= light->position.W();
			xl.Position = pos;
			xl.Direction = light->spotDirection;
		}
		xl.Range = light->range;
		xl.Falloff = light->spotFalloff;
		xl.Attenuation0 = light->attenuation0;
		xl.Attenuation1 = light->attenuation1;
		xl.Attenuation2 = light->attenuation2;
	    xl.Theta = light->spotInner;
	    xl.Phi = light->spotCutoff;
		//	‰EŽèŒn¨¶ŽèŒn‚Ì•ÏŠ·
		xl.Direction.Z()*=-1;
		xl.Position.Z()*=-1;
		doc->SetWholeData(xl);
	}
};

DEF_REGISTER_BOTH(GRLight);

}	//	Spr
