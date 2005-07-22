#include "Graphics.h"
#pragma hdrstop
#include "GRCamera.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	GRCamera
SGOBJECTIMP(GRCamera, SGObject);

GRCameraData::GRCameraData(){ InitData(); }

void GRCameraData::InitData(){
	front = 0.4f;
	back = 4000.0f;
	size = Vec2f(front, front);
	center = Vec2f(0, 0);
	view.Pos() = Vec3f(0, 6, 12);
    view.LookAtGL(Vec3f(), Vec3f(0,1,0));
	view = view.inv();
}
void GRCamera::UpdatePosture(){
	if (frPosture){
		data.view = frPosture->GetWorldPosture().inv();
	}
}

bool GRCamera::AddChildObject(SGObject* obj, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, obj);
	if (fr){
		frPosture = fr;
		return true;
	}
	return false;
}

typedef float FLOAT;
typedef Affinef Matrix4x4;
DEF_RECORD(XCamera, {
	GUID Guid(){ return WBGuid("21482E41-4BBA-4e9b-B84B-8C94E2C17C29"); }
	Matrix4x4 View;
	FLOAT width;
	FLOAT height;
	FLOAT offsetX;
	FLOAT offsetY;
	FLOAT front;
	FLOAT back;
})

class GRCameraLoader:public FIObjectLoader<GRCamera>{
public:
	GRCameraLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Matrix4x4);
		db->REG_RECORD_PROTO(XCamera);
	}
	virtual bool LoadData(FILoadScene* ctx, GRCamera* cam){
		assert(sizeof(cam->data) == sizeof(XCamera));
		ctx->docs.Top()->GetWholeData(cam->data);
		///	¶ŽèŒn¨‰EŽèŒn•ÏŠ·
		cam->data.view.ExZ() *= -1;
		cam->data.view.EyZ() *= -1;
		cam->data.view.EzX() *= -1;
		cam->data.view.EzY() *= -1;
		cam->data.view.PosZ() *= -1;
		cam->data.view = cam->data.view.inv();

		ctx->scene->GetRenderers().Set(cam);
		return true;
	}
};

//	Saver
class GRCameraSaver:public FIObjectSaver<GRCamera>{
public:
	virtual UTString GetType() const{ return "GRCamera"; }
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, GRCamera* cam){
		GRCameraData data = cam->data;
		data.view = data.view.inv();
		///	‰EŽèŒn¨¶ŽèŒn•ÏŠ·
		data.view.ExZ() *= -1;
		data.view.EyZ() *= -1;
		data.view.EzX() *= -1;
		data.view.EzY() *= -1;
		data.view.PosZ() *= -1;
		doc->SetWholeData(data);
	}
};
DEF_REGISTER_BOTH(GRCamera);

}
