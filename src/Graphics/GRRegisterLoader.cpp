#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;
void SPR_DLL RegisterLoaderForGraphics(FILoadScene* l){
	REGISTER_LOADER(l, GRCamera);
	REGISTER_LOADER(l, GRMesh);
	REGISTER_LOADER(l, GRMeshMaterialList);
	REGISTER_LOADER(l, GRMaterial);
	REGISTER_LOADER(l, GRMeshNormals);
	REGISTER_LOADER(l, GRMeshTextureCoords);
	REGISTER_LOADER(l, GRSphere);
	REGISTER_LOADER(l, GRLight);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForGraphics(FISaveScene* s){
	REGISTER_SAVER(s, GRCamera);
	REGISTER_SAVER(s, GRMesh);
	REGISTER_SAVER(s, GRMaterial);
	REGISTER_SAVER(s, GRSphere);
	REGISTER_SAVER(s, GRLight);
}
}

void RegisterLoaderForGraphics(Spr::FILoadScene* l){
	Spr::RegisterLoaderForGraphics(l);
}
void SPR_DLL RegisterSaverForGraphics(Spr::FISaveScene* s){
	Spr::RegisterSaverForGraphics(s);
}
