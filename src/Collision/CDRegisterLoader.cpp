#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;
void SPR_DLL RegisterLoaderForCollision(FILoadScene* l){
	REGISTER_LOADER(l, CDMesh);
	REGISTER_LOADER(l, CDSphere);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForCollision(FISaveScene* s){
}
}

void RegisterLoaderForCollision(Spr::FILoadScene* l){
	Spr::RegisterLoaderForCollision(l);
}
void SPR_DLL RegisterSaverForCollision(Spr::FISaveScene* s){
	Spr::RegisterSaverForCollision(s);
}



