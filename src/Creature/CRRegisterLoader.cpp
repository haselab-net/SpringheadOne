#include <FileIO/FIDocScene.h>
#pragma hdrstop

namespace Spr{;

void RegisterLoaderForCreature(FILoadScene* l){
	REGISTER_LOADER(l, CRBeetle);
	REGISTER_LOADER(l, CRVirtualHuman);
	REGISTER_LOADER(l, CRHuman);
	REGISTER_LOADER(l, CRPuppet);
	REGISTER_LOADER(l, CRUser);
}
void RegisterSaverForCreature(FISaveScene* l){
	REGISTER_SAVER(l, CRBeetle);
	REGISTER_SAVER(l, CRVirtualHuman);
	REGISTER_SAVER(l, CRHuman);
	REGISTER_SAVER(l, CRPuppet);
	REGISTER_SAVER(l, CRUser);
}
}

void RegisterLoaderForCreature(Spr::FILoadScene* l){
	Spr::RegisterLoaderForCreature(l);
}
void SPR_DLL RegisterSaverForCreature(Spr::FISaveScene* s){
	Spr::RegisterSaverForCreature(s);
}
