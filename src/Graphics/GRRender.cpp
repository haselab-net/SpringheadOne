#include "Graphics.h"
#pragma hdrstop
#include "GRRender.h"

namespace Spr{;
//----------------------------------------------------------------------------
//	GRRender
SGOBJECTIMPABST(GRRender, SGObject);

GRRender::GRRender(){
	camera = new GRCamera;
	drawState = DRAW_BOTH;
}
GRRender::~GRRender(){
}
void GRRender::InitTree(SGFrame* fr, SGScene* scene){
}
void GRRender::Set(SGObject* obj){
	GRCamera* cam = DCAST(GRCamera, obj);
	if (cam) camera = cam;
}
void GRRender::Clear(SGScene* s){
	camera = new GRCamera;
}
void GRRender::RenderRecurse(SGFrame* n){
	for(SGObjects::iterator it=n->contents.begin(); it != n->contents.end(); ++it){
		GRVisual* vis = DCAST(GRVisual, *it);
		if (vis) vis->Render(n, this);
	}
	for(SGFrames::const_iterator it=n->Children().begin(); it != n->Children().end(); ++it){
		RenderRecurse(*it);
	}
	for(SGObjects::reverse_iterator it=n->contents.rbegin(); it != n->contents.rend(); ++it){
		GRVisual* vis = DCAST(GRVisual, *it);
		if (vis) vis->Rendered(n, this);
	}
}

}
