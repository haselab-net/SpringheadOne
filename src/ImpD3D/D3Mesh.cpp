#include "ImpD3D.h"
#pragma hdrstop

namespace Spr {;

//----------------------------------------------------------------------------
//	D3Mesh
SGOBJECTIMP(D3Mesh, GRVisual);

HRESULT D3Mesh::LockVertexBuffer(DWORD flg, void** v){
#if SPR_DXVER==8
	return intf->LockVertexBuffer(flg, (BYTE**)v);
#elif SPR_DXVER==9
	return intf->LockVertexBuffer(flg, v);
#endif
}

HRESULT D3Mesh::LockIndexBuffer(DWORD flg, void** indexBuf){
#if SPR_DXVER==8
	HRESULT rv = intf->LockIndexBuffer(flg, (BYTE**)indexBuf);
#elif SPR_DXVER==9
	HRESULT rv = intf->LockIndexBuffer(flg, (void**)indexBuf);
#endif
	return rv;
}

void D3Mesh::Set(GRMesh* gm, SGScene* scene){
	D3Render* render = NULL;
	scene->GetRenderers().Find(render);
		
	DWORD fvf;
	if (gm->texCoords.size()) fvf = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
	else fvf = D3DFVF_XYZ|D3DFVF_NORMAL;
	D3DXCreateMeshFVF(gm->triangles.size()/3, gm->vertices.size(), D3DXMESH_MANAGED, fvf, render->device, &intf.Intf() );

	if (gm->texCoords.size()){
		struct VtxFVF{
			Vec3f pos;
			Vec3f normal;
			Vec2f tex;
		};
		VtxFVF* vtxs;
		LockVertexBuffer(0, (void**)&vtxs);
		for(unsigned int i=0; i<gm->vertices.size(); ++i){
			vtxs[i].pos = gm->vertices[i];
			vtxs[i].normal = gm->normals[i];
			vtxs[i].tex = gm->texCoords[i];
		}
		intf->UnlockVertexBuffer();
	}else{
		struct VtxFVF{
			Vec3f pos;
			Vec3f normal;
		};
		VtxFVF* vtxs;
		LockVertexBuffer(0, (void**)&vtxs);
		for(unsigned int i=0; i<gm->vertices.size(); ++i){
			vtxs[i].pos = gm->vertices[i];
			vtxs[i].normal = gm->normals[i];
		}
		intf->UnlockVertexBuffer();
	}
	
	WORD* indexBuf=NULL;
	LockIndexBuffer(0, (void**)&indexBuf);
	for(unsigned i=0; i<gm->triangles.size(); ++i) indexBuf[i] = (WORD)gm->triangles[i];
	intf->UnlockIndexBuffer();
	DWORD* attrs=NULL;
	intf->LockAttributeBuffer(0, &attrs);
	for(unsigned i=0; i<gm->attributes.size(); ++i) attrs[i] = gm->attributes[i];
	intf->UnlockAttributeBuffer();
	intf->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

	materials.resize(gm->materials.size());
	for(unsigned i=0; i<materials.size(); ++i){
		materials[i] = new D3Material;
		memcpy(&materials[i]->material, (GRMaterialData*)&*gm->materials[i], sizeof(GRMaterialData));
		materials[i]->bOpaque = gm->materials[i]->IsOpaque();
		materials[i]->textureFilename = gm->materials[i]->textureFilename;
		materials[i]->texture = render->textureManager.Get(materials[i]->textureFilename);
	}
}

void D3Mesh::Render(SGFrame* f, GRRender* renderBase){
	D3Render* render = (D3Render*)renderBase;
	for(unsigned int i=0; i<materials.size(); i++){
		if (materials[i]->bOpaque && render->drawState&GRRender::DRAW_OPAQUE){
			materials[i]->Render(f, renderBase);
			WXCHECK(intf->DrawSubset(i));
		}
		if (!materials[i]->bOpaque && render->drawState&GRRender::DRAW_TRANSPARENT){
			materials[i]->Render(f, renderBase);
			WXCHECK(intf->DrawSubset(i));
		}
	}
	//	テクスチャを戻す．
	render->device->SetTexture(0,NULL);
}

}
