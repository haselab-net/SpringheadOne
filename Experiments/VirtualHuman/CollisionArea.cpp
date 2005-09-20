// CollisionArea.cpp: CollisionArea クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "virtualhuman.h"
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

	if(bAdd){
		collision = NULL;
		render = NULL;
		bAdd = false;
		ClearCoVetxs();
	}
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

void CollisionArea::AddListener(){
	if(collision){
		collision->AddListener(this);
		bAdd = true;
	}
}

void CollisionArea::DrawCollisionVetxs(){
	
	if(render && coVetxs.size() > 0){
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,1,0,1)));
		
		//接触点を描画
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(Spr::GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
			//DSTR << coVetxs[i] <<std::endl;
		}
		
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
	}

	bClear = true;	//描画後にcoVetxsをクリアする。そのbool
}

void CollisionArea::CalcSupportArea(){

	supportVetxs.clear();

	// coVetxsを二次元化
	for(int i = 0; i < coVetxs.size(); i++){
		coVetxs[i].y = 0.0f;
	}

	// coVetxsの重心座標を計算
	Spr::Vec3f cog = Spr::Vec3f();
	for(int i = 0; i < coVetxs.size(); i++){
		cog += coVetxs[i];
	}
	cog /= coVetxs.size();

	// 重心から一番離れている点
	float maxPos = 0;				// 比較に使われる絶対値の最大値
	int startVetx = 0;				// 安定領域の最初の点
	for(int i = 0; i < coVetxs.size(); i++){
		float c = (cog - coVetxs[i]).norm();
		if(maxPos < c){
			maxPos = c;
			startVetx = i;
		}
	}
	supportVetxs.push_back(coVetxs[startVetx]);	//重心から離れている点を外形の第一の点とする
	

	// 重心と一番離れている点からの角度が一番大きい点を探す
	Spr::Vec3f newVetx;	//次の安定領域の点

	Spr::Vec3f zero_cog = cog - supportVetxs[0]; //第一の点から重心へのベクトル
	float cos_min=1;				//余弦の最小値
	for(int i = 0; i < coVetxs.size(); i++){
		if(!(i == startVetx)){	// 第一点以外
		//if(!(coVetxs[i] == supportVetxs[0])){	// 第一点以外
			//余弦を求める
			Spr::Vec3f zero_other = coVetxs[i] - supportVetxs[0];	//第一の点からそれぞれの点へのベクトル
			float cos_buf = (zero_cog*zero_other)/(abs(zero_cog.norm())*abs(zero_other.norm()));
			//余弦最小値
			if(cos_buf < cos_min){
				newVetx = coVetxs[i];
				cos_min = cos_buf;
			}
		}
	}
	supportVetxs.push_back(newVetx);	//第二点目


	//他の安定領域の点を検索
	for(int i = 0; i < coVetxs.size(); i++){
		Spr::Vec3f supportAreaLine = supportVetxs[i+1] - supportVetxs[i];	//外形の基準となるベクトル
		cos_min=1.0f;	//cos_min 初期化
		
		//ある基準点から見て
		for(int j = 0; j < coVetxs.size(); j++){
			//すでに見つけた安定領域の点を除く
			if(coVetxs[i] != supportVetxs[i+1] && coVetxs[i] != supportVetxs[i]){
				// 基準の点から他の点へのベクトル
				Spr::Vec3f  supportAreaLineBuf = coVetxs[i] - supportVetxs[i+1];
				//余弦を求める
				double cos_buf = (supportAreaLineBuf*supportAreaLine)/(supportAreaLineBuf.norm()*supportAreaLine.norm());
				//余弦最小値
				if(cos_buf<cos_min)
				{
					newVetx = coVetxs[i];
					cos_min=cos_buf;
				}
			}
		}
		supportVetxs.push_back(newVetx);
		if(newVetx ==supportVetxs[0])
			break;
	}
}
