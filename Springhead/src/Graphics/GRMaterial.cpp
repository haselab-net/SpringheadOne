#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	GRMaterial
SGOBJECTIMPABST(GRMaterial, GRVisual);

void GRMaterial::Render(SGFrame* f, GRRender* render){
	render->SetMaterial(*this);
}

//	GRMaterialLoad �� GRMesh.cpp �ɂ���D


}	//	Spr

