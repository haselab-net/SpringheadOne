#include "ImpD3D.h"
#pragma hdrstop

namespace Spr {;


//-------------------------------------------------------------------
//	Loader
//-------------------------------------------------------------------
void SPR_DLL RegisterLoaderForImpD3D(FILoadScene* l){
	REGISTER_LOADER(l, D3Import);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForImpD3D(FISaveScene* s){
	REGISTER_SAVER(s, D3Import);
}

}

void RegisterLoaderForImpD3D(Spr::FILoadScene* l){
	Spr::RegisterLoaderForImpD3D(l);
}
void SPR_DLL RegisterSaverForImpD3D(Spr::FISaveScene* s){
	Spr::RegisterSaverForImpD3D(s);
}
