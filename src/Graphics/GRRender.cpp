#include "Graphics.h"
#pragma hdrstop
#include "GRRender.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	GRFont
GRFont::GRFont(int h, const char* f){
	height=h;
	width=0;
	face=f ? f : "";
	weight = 400;
	color=0xffffffff;
	bItalic=false;
}
bool GRFont::operator < (GRFont& f){
	if (face < f.face) return true;
	if (height < f.height) return true;
	if (color < f.color) return true;
	return false;
}

//----------------------------------------------------------------------------
//	GRRender
SGOBJECTIMPABST(GRRender, SGObject);

GRRender::GRRender(){
	camera = new GRCamera;
	drawState = DRAW_BOTH;
	bDrawDebug = false;
	scene = NULL;
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
void GRRender::Render(SGScene* s){
	scene = s;
	//	Ž‹“_s—ñ‚ÌÝ’è
    if (camera){
		camera->UpdatePosture();
		SetViewMatrix(camera->data.view);
	}

	//	•s“§–¾•”‚Ì•`‰æ
	drawState = DRAW_OPAQUE;
	PushModelMatrix();
	SetModelMatrix(Affinef());
	frames.Push(s->GetWorld());
	RenderRecurse();
	frames.Pop();
	s->GetBehaviors().Render(this, s);
	PopModelMatrix();

	//	”¼“§–¾•”‚Ì•`‰æ
	PushModelMatrix();
	drawState = DRAW_TRANSPARENT;
	SetDepthWrite(false);
	SetAlphaMode(BF_SRCALPHA, BF_INVSRCALPHA);
	frames.Push(s->GetWorld());
	RenderRecurse();
	frames.Pop();
	s->GetBehaviors().Render(this, s);
	PopModelMatrix();
	
	//	•’i‚ÍBOTH‚É‚µ‚Ä‚¨‚­
	drawState = DRAW_BOTH;
	SetDepthWrite(true);
	SetAlphaMode(BF_ONE, BF_ZERO);

	scene = NULL;
}

void GRRender::RenderRecurse(){
	SGFrame* n = frames.Top();
	PushModelMatrix();
	MultModelMatrix(n->GetPosture());
	for(SGObjects::iterator it=n->contents.begin(); it != n->contents.end(); ++it){
		GRVisual* vis = DCAST(GRVisual, *it);
		if (vis) vis->Render(n, this);
	}
	for(SGFrames::const_iterator it=n->Children().begin(); it != n->Children().end(); ++it){
		frames.Push(*it);
		RenderRecurse();
		frames.Pop();
	}
	for(SGObjects::reverse_iterator it=n->contents.rbegin(); it != n->contents.rend(); ++it){
		GRVisual* vis = DCAST(GRVisual, *it);
		if (vis) vis->Rendered(n, this);
	}
	PopModelMatrix();
}

}
