#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	GRMaterial
SGOBJECTIMPABST(GRMaterial, GRVisual);

void GRMaterial::Render(SGFrame* f, GRRender* render){
	render->SetMaterial(*this);

    // set the texture to the render
    if(textureFilename.c_str() != ""){
        render->SetTexture(textureFilename.c_str());
	}
}

//	GRMaterialLoad ÇÕ GRMesh.cpp Ç…Ç†ÇÈÅD


}	//	Spr

