#include "GraphicsGL.h"
#pragma hdrstop

namespace Spr{;

SGOBJECTIMP(GLMesh, GRVisual);

void GLMesh::Set(GRMesh* gmesh, SGScene* scene){
	gm = gmesh;
	GLRender* render = NULL;
	scene->GetRenderers().Find(render);
	assert(render);
	listOpaque = glGenLists(1);
	listTransparent = glGenLists(1);
	int lists[] = {listOpaque, listTransparent};
	bool bOpaque = true;
	for(int i=0; i<2; ++i){
		glNewList(lists[i], GL_COMPILE);
		for(unsigned m=0; m < gm->materials.size(); ++m){
			if (gm->materials[m]->IsOpaque() != bOpaque) continue;
			render->SetMaterial(*gm->materials[m]);
			render->SetTexture(gm->materials[m]->textureFilename.length() ? gm->materials[m]->textureFilename.c_str() : NULL);
			glBegin(GL_TRIANGLES);
			for(unsigned  i=0; i<gm->attributes.size(); ++i){
				if (gm->attributes[i] == m){
					std::vector<DWORD>::iterator tri=gm->triangles.begin()+3*i;
					for(int i=0; i<3; ++i){
						if (gm->texCoords.size()) glTexCoord2fv(gm->texCoords[tri[i]]);
						glNormal3fv(gm->normals[tri[i]]);
						glVertex3fv(gm->vertices[tri[i]]);
					}
				}
			}
			glEnd();
			render->SetTexture(NULL);
		}
		glEndList();
		bOpaque = false;
	}
}

void GLMesh::Render(SGFrame* n, GRRender* render){
	if (render->drawState & GRRender::DRAW_OPAQUE) glCallList(listOpaque);
	if (render->drawState & GRRender::DRAW_TRANSPARENT) glCallList(listTransparent);	
}

}