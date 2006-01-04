#include "Graphics.h"
#pragma hdrstop
#include "GRSphere.h"

namespace Spr {;

//----------------------------------------------------------------------------
//	GRSphere
SGOBJECTIMP(GRSphere, SGObject);
bool GRSphere::AddChildObject(SGObject* obj, SGScene* s){
	if (DCAST(GRMaterial, obj)){
		GRMaterial* mat = (GRMaterial*)obj;
		material = mat;
		return true;
	}
	return false;
}
size_t GRSphere::NChildObjects(){
	return material ? 1 : 0;
}
SGObject* GRSphere::ChildObject(size_t i){
	if (i==0) return material;
	return NULL;
}

	
typedef float FLOAT;
DEF_RECORD(XSphere, {
	///	Ç±ÇÃå^ÇÃGUIDÅDTypeDescÇ™GUIDÇéùÇ¬ÇÊÇ§Ç…Ç»ÇÈÅD
	GUID Guid(){ return WBGuid("4B4F22CC-9DD9-4f04-9373-9E0A03E36C35"); }
	FLOAT Radius;
	DWORD Slices;
	DWORD Stacks;
});
class GRSphereLoader:public FIObjectLoader<GRSphere>{
public:
	UTRef<FITypeDescDb> db;
	GRSphereLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(DWORD);
		db->REG_RECORD_PROTO(XSphere);
	}
	virtual bool LoadData(FILoadScene* ctx, GRSphere* sphere){
		XSphere xsphere;
		ctx->docs.Top()->GetWholeData(xsphere);
		sphere->radius = xsphere.Radius;
		sphere->slices = xsphere.Slices;
		sphere->stacks= xsphere.Stacks;
		return true;
	}
};
class GRSphereSaver:public FIObjectSaver<GRSphere>{
public:
	void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, GRSphere* sphere){
		XSphere xsphere;
		xsphere.Radius = sphere->radius;
		xsphere.Slices = sphere->slices;
		xsphere.Stacks = sphere->stacks;
		doc->SetWholeData(xsphere);
	}
};

DEF_REGISTER_BOTH(GRSphere);



}
