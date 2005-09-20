#include "GraphicsGL.h"
#pragma hdrstop

namespace Spr {;


//-------------------------------------------------------------------
//	Loader
//-------------------------------------------------------------------
void SPR_DLL RegisterLoaderForGraphicsWGL(FILoadScene* l){
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForGraphicsWGL(FISaveScene* s){
}

}

void RegisterLoaderForGraphicsWGL(Spr::FILoadScene* l){
	Spr::RegisterLoaderForGraphicsWGL(l);
}
void SPR_DLL RegisterSaverForGraphicsWGL(Spr::FISaveScene* s){
	Spr::RegisterSaverForGraphicsWGL(s);
}
