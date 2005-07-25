#include "ImpD3D.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	D3Material
SGOBJECTIMP(D3Material, GRVisual);

D3Material::D3Material(){
}
D3Material::D3Material(GRMaterial& gm){
	material = (D3DMATERIAL_SPR&)(GRMaterialData&)gm;
	textureFilename = gm.textureFilename;
	bOpaque = gm.IsOpaque();
}

void D3Material::Render(SGFrame* f, GRRender* renderBase){
	D3Render* render = (D3Render*)renderBase;
	//	マテリアルの設定
	WXCHECK(render->device->SetMaterial(&material));
	//	テクスチャを貼る
	if (!texture && textureFilename.length()){
		texture = render->textureManager.Get(textureFilename);
	}
	if (texture){
		WXCHECK(render->device->SetTexture(0, texture));
		
		// alpha blending to texture
		// process alpha blending
//		WXCHECK(render->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATEBLENDTEXTUREALPHA));
WXCHECK(render->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE));
		// set arguments
		render->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		render->device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}
}
void D3Material::SetXMaterial(D3Material::XMaterial xmat){
	material.Diffuse.a = material.Ambient.a = xmat.face.a;
	material.Diffuse.r = material.Ambient.r = xmat.face.r;
	material.Diffuse.g = material.Ambient.g = xmat.face.g;
	material.Diffuse.b = material.Ambient.b = xmat.face.b;
	material.Specular.a = 1.0f;
	material.Specular.r = xmat.specular.r;
	material.Specular.g = xmat.specular.g;
	material.Specular.b = xmat.specular.b;
	material.Emissive.a = 1.0f;
	material.Emissive.r = xmat.emissive.r;
	material.Emissive.g = xmat.emissive.g;
	material.Emissive.b = xmat.emissive.b;
	material.Power = xmat.power;
}
D3Material::XMaterial D3Material::GetXMaterial(){
	D3Material::XMaterial xmat;
	xmat.face.a = material.Diffuse.a;
	xmat.face.r = material.Diffuse.r;
	xmat.face.g = material.Diffuse.g;
	xmat.face.b = material.Diffuse.b;
	xmat.specular.r = material.Specular.r;
	xmat.specular.g = material.Specular.g;
	xmat.specular.b = material.Specular.b;
	xmat.emissive.r = material.Emissive.r;
	xmat.emissive.g = material.Emissive.g;
	xmat.emissive.b = material.Emissive.b;
	xmat.power = material.Power;
	return xmat;
}

//	D3MaterialLoad は D3Mesh.cpp にある．


}	//	Spr

