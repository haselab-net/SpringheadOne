#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;
void SPR_DLL RegisterLoaderForSceneGraph(FILoadScene* l){
	REGISTER_LOADER(l, SGFrame);
	REGISTER_LOADER(l, SGTransformMatrix);
	REGISTER_LOADER(l, SGScene);
	REGISTER_LOADER(l, SGSimulator);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForSceneGraph(FISaveScene* s){
	REGISTER_SAVER(s, SGFrame);
	REGISTER_SAVER(s, SGScene);
	REGISTER_SAVER(s, SGSimulator);
}
}


void RegisterLoaderForSceneGraph(Spr::FILoadScene* l){
	Spr::RegisterLoaderForSceneGraph(l);
}
void SPR_DLL RegisterSaverForSceneGraph(Spr::FISaveScene* s){
	Spr::RegisterSaverForSceneGraph(s);
}
