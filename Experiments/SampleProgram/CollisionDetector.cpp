// CollisionDetector.cpp: CollisionDetector クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CollisionDetector.h"



//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CollisionDetector::CollisionDetector()
{
	render = NULL;
	collision = NULL;
	bAdd = false;
}

CollisionDetector::~CollisionDetector()
{

}

void CollisionDetector::ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* viewRender){
	scene->GetBehaviors().Find(collision);
//	scene->GetBehaviors().Find(render);
	render = viewRender;
	AddListener();
}

void CollisionDetector::Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
								 Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer){

	for(Spr::CDQHPlane<Spr::CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//双対変換をしている。
		if(!it->deleted)
		coVetxs.push_back(it->normal / it->dist + conv->commonPoint);
	}
}

void CollisionDetector::ClearCoVetxs(){

	coVetxs.clear();

}

void CollisionDetector::AddListener(){
	if(collision){
		collision->AddListener(this);
		bAdd = true;
	}
}

void CollisionDetector::DrawCollisionVetxs(){
	
	if(render){
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(Spr::GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
			//DSTR << i << "=" << coVetxs[i] <<std::endl;
		}
	}
	Spr::Vec3f te;
	te = Spr::Vec3f(0.0,0.0,0.0);
	render->SetLineWidth(16);
	render->SetMaterial(Mat(Spr::Vec4f(1,1,0,1)));
	render->DrawDirect(Spr::GRRender::POINTS,&te,&te+1);
	//DSTR << coVetxs.size() << std::endl;
	ClearCoVetxs();
}
