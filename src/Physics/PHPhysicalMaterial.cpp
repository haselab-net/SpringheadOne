#include "Physics.h"
#pragma hdrstop

namespace Spr{;
SGOBJECTIMP(PHPhysicalMaterial, SGObject);

void PHPhysicalMaterialData::InitData(){
	reflexSpring = reflexDamper = frictionSpring = frictionDamper = 1.0f;
	staticFriction = 0.6f;
	dynamicFriction = 0.3f;
}

//	Loader
class PHPhysicalMaterialLoader:public FIObjectLoader<PHPhysicalMaterial>{
	virtual bool LoadData(FILoadScene* ctx, PHPhysicalMaterial* pmat){
		ctx->docs.Top()->GetWholeData(pmat->pMatData);
		return true;
	}
};

//	Saver
class PHPhysicalMaterialSaver:public FIObjectSaver<PHPhysicalMaterial>{
public:
	virtual UTString GetType() const{ return "PHPhysicalMaterial"; }
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHPhysicalMaterial* pmat){
		doc->SetWholeData(pmat->pMatData);
	}
};
DEF_REGISTER_BOTH(PHPhysicalMaterial);

}
