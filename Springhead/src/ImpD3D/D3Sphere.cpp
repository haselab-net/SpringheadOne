#include "ImpD3D.h"
#pragma hdrstop
#include "D3Sphere.h"

namespace Spr {;

//----------------------------------------------------------------------------
//	D3Sphere
SGOBJECTIMP(D3Sphere, D3Mesh);
void D3Sphere::Set(GRSphere* s, SGScene* scene){
	D3Render* render = NULL;
	scene->GetRenderers().Find(render);

	WXINTF(D3DXBuffer) adjacency;
	D3DXCreateSphere(render->device, s->radius, s->slices, s->stacks, &intf.Intf(), &adjacency.Intf());
	//	–@ü‚ª‚È‚¢ê‡–@ü‚ð‘‚«ž‚Þ
	if (intf){
		DWORD fvf = intf->GetFVF();
		if(fvf != (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)){
			WXINTF(D3DXMesh) meshTmp;
			intf->CloneMeshFVF(intf->GetOptions(),D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
				render->device, &meshTmp.Intf());
			intf = meshTmp;
		}
		#if SPR_DXVER>=8
		D3DXComputeNormals(intf, (DWORD*)adjacency->GetBufferPointer());
		#else
		D3DXComputeNormals(intf);
		#endif		
	}
	assert(intf);
/*
	struct Vtx{
		Vec3f pos;
		Vec3f normal;
		Vec2f tex;
	};
	Vtx* vtx;
	int nVtx = intf->GetNumVertices();
	LockVertexBuffer(0, (void**)&vtx);
	for(int i=0; i < nVtx; ++i) vtx[i].normal *= -1;
	intf->UnlockVertexBuffer();
*/
	materials.resize(1);
	materials[0] = new D3Material;
	memcpy(&materials[0]->material, (GRMaterialData*)&*s->material, sizeof(GRMaterialData));
	materials[0]->bOpaque = s->material->IsOpaque();
	materials[0]->textureFilename = s->material->textureFilename;
	materials[0]->texture = render->textureManager.Get(materials[0]->textureFilename);
};

}