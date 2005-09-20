#include "Physics.h"
#pragma hdrstop

namespace Spr{;
SGOBJECTIMP(PHPhysicalMaterial, SGObject);

class PHPhysicalMaterialLoader:public FIObjectLoader<PHPhysicalMaterial>{
	virtual void LoadData(FILoadScene* ctx, UTRef<PHPhysicalMaterial>& pmat){
		pmat = new PHPhysicalMaterial;
		ctx->docs.Top()->GetData(pmat->pMatData);
	}
};

DEF_REGISTER_LOADER(PHPhysicalMaterial);

}
