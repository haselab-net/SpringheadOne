#include "GraphicsGL.h"
#pragma hdrstop

namespace Spr{;

SGOBJECTIMP(GLSphere, GRVisual);

void GLSphere::Set(GRSphere* gm, SGScene* scene){
	GLRender* render = NULL;
	scene->GetRenderers().Find(render);
	assert(render);
	bOpaque = gm->material->IsOpaque();
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	render->SetMaterial(*gm->material);
	GLUquadricObj* q = gluNewQuadric();
	gluSphere(q, gm->radius, gm->slices, gm->stacks);
	gluDeleteQuadric(q);
	glEndList();
}

void GLSphere::Render(SGFrame* n, GRRender* r){
	if (bOpaque){
		if (r->drawState&GRRender::DRAW_OPAQUE) glCallList(list);
	}else{
		if (r->drawState&GRRender::DRAW_TRANSPARENT) glCallList(list);
	}
}

}