#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;

void RegisterLoaderForCreature(FILoadScene* l){
	REGISTER_LOADER(l, CRBeetle);
	REGISTER_LOADER(l, CRHuman);
}
void RegisterSaverForCreature(FISaveScene* l){
	REGISTER_SAVER(l, CRBeetle);
	REGISTER_SAVER(l, CRHuman);
}
}

void RegisterLoaderForCreature(Spr::FILoadScene* l){
	Spr::RegisterLoaderForCreature(l);
}
void SPR_DLL RegisterSaverForCreature(Spr::FISaveScene* s){
	Spr::RegisterSaverForCreature(s);
}
