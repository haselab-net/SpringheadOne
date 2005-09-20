// CollisionArea.cpp: CollisionArea クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
//#include "virtualhuman.h"
#include "CollisionArea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CollisionArea::CollisionArea()
{
	render = NULL;
	collision = NULL;
	bAdd = false;
	bClear = false;
}

CollisionArea::~CollisionArea()
{

}

void CollisionArea::ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* viewRender,Spr::PHHuman& phHuman){
	
	if(!bAdd && scene && viewRender){
		scene->GetBehaviors().Find(collision);
		render = viewRender;
		AddListener();
	}
}

void CollisionArea::Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
								 Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer){
	if(bClear){
		ClearCoVetxs();
	}
	bClear = false;

	for(Spr::CDQHPlane<Spr::CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//双対変換をしている。
		//it->deletedがtrueになっていると使わなくなった面を示す。
		if(it->dist != 0 && !it->deleted){
			coVetxs.push_back(it->normal / it->dist + conv->commonPoint);
		}
	}
}

void CollisionArea::ClearCoVetxs(){
	coVetxs.clear();
}

void CollisionArea::ReLoad(){
	ClearCoVetxs();
	collision = NULL;
	render = NULL;
	bAdd = false;
}

void CollisionArea::AddListener(){
	if(collision){
		listenerPos = collision->AddListener(this);
		bAdd = true;
	}
}

void CollisionArea::DrawCollisionVetxs(){
	
	if(render && coVetxs.size() > 0){
		render->SetModelMatrix(Spr::Affinef());
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
		
		//接触点を描画
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(Spr::GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
			//DSTR << coVetxs[i] <<std::endl;
		}
		
		/*
		//接触点を線で結んでいる。
		std::vector<Spr::Vec3f> vert;
		for(int i = 0; i < coVetxs.size(); ++i){
			vert.push_back(coVetxs[i]);
		}
		vert.push_back(coVetxs[0]);
		for(int i = 0; i < vert.size()-1; ++i){
			render->DrawDirect(Spr::GRRender::LINES, &vert[i], &vert[i+2]);
		}
		vert.clear();
		*/
	}

	bClear = true;	//描画後にcoVetxsをクリアする。そのbool

}

void CollisionArea::Step(Spr::SGScene* scene,Spr::GRRender* mainrender,Spr::PHHuman& phHuman){

	if(scene && mainrender && !bAdd){
		ConnectBehaviorEngines(scene,mainrender,phHuman);
		SelectCDFrame(phHuman);
	}

	if(bAdd){
		DrawCollisionVetxs();
	}
}

void CollisionArea::SelectCDFrame(Spr::PHHuman& phHuman){
	if (collision){
		for(Spr::CDCollisionEngine::TFrameIt it = collision->FramePairBegin(); it != collision->FramePairEnd(); ++it){
			if (!*it) continue;
			static int iii =0;
			bool b0 = (*it)->frame[0] && HasFrame((*it)->frame[0]->frame,phHuman);
			bool b1 = (*it)->frame[1] && HasFrame((*it)->frame[1]->frame,phHuman);
			if (!((b0 && b1) || (!b0 && !b1))){
				collision->AddActive((*it)->frame[0]->frame,(*it)->frame[1]->frame, listenerPos);
			}
		}
		collision->Init();
	}
}
bool CollisionArea::HasFrame(Spr::SGFrame* f,Spr::PHHuman& phHuman){
	for(int i=0; i<phHuman.solids.size(); ++i){
		if (phHuman.solids[i] && phHuman.solids[i]->GetFrame() == f) return true;
	}
	return false;
}


