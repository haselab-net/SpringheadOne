#include "Creature.h"
#pragma hdrstop
// CRAttention.cpp: CRAttention クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRAttention.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRAttention::CRAttention(){
	// コンストラクタ
}

CRAttention::~CRAttention(){
	// デストラクタ
}

//------------------　基本的なインタフェース　------------------//

void CRAttention::Step(){
	if (bExperimentMode){
		int currentTimeInSec = (GetTickCount() - startTiming)/1000;
		maxAttentionPoint = poslist[counter];
		if (currentTimeInSec >= timinglist[counter]){counter++;}
		if (counter > timinglist.size()){
			bExperimentMode = false;
			ofs_head.close();
			ofs_hedg.close();
			ofs_leye.close();
			ofs_reye.close();
			ofs_gaze.close();
			ofs_eyes.close();
			ofs_info.close();
		}
		bFoundAttention = true;
	}else{
		CalcMaxAttentionPoint();
	}

	if (bFoundAttention){
		crEye->SetAttentionPoint(maxAttentionPoint);
		crEye->SetAttentionMode();
		crNeckController->SetAttentionMode();
		if (crEye->IsOverRange(10.0f,30.0f)){
			Vec3f headgoalDir = maxAttentionPoint - crNeckController->frHead->GetPosture().Pos();
			headgoalDir = crNeckController->LimitRange(headgoalDir,10.0f,30.0f);
			crNeckController->SetAttentionPoint(crNeckController->frHead->GetPosture().Pos() + headgoalDir);
			bHeadControl = true;
		}else{
			bHeadControl = false;
		}
	}

	if (bExperimentMode){
		ofs_head << crNeckController->headpos << std::endl;
		ofs_hedg << crNeckController->headposgoal << std::endl;
		ofs_leye << crEye->eyeposL << std::endl;
		ofs_reye << crEye->eyeposR << std::endl;
		ofs_gaze << (crNeckController->headpos + (crEye->eyeposL + crEye->eyeposR)/2.0f) << std::endl;
		ofs_eyes << ((crEye->eyeposR + crEye->eyeposL)/2.0f) << std::endl;
		ofs_info << crEye->overrange << std::endl;

		DSTR << "E:" << ((crEye->eyeposR + crEye->eyeposL)/2.0f) << std::endl;
	}

	attentionList.clear();
}


void CRAttention::Load(SGScene* scene,CRPuppet* crPuppet, CREye* crEye,CRNeckController* crNeckController){
	this->scene = scene;
	this->crPuppet = crPuppet;
	this->crEye = crEye;
	this->crNeckController = crNeckController;

	if (crPuppet->IsLoaded()){
	}

	Init();
}

void CRAttention::Init(){
	// 初期化
	bHeadControl = false;
	bExperimentMode = false;
}

void CRAttention::OnKeyDown(UINT &nChar){
}

void CRAttention::Draw(GRRender* render){
	// デバッグ用表示
	GRFont font(50, "MS ゴシック");
	if (bHeadControl){
		font.color = RGB(255,255,255)|0xFF000000;
		render->DrawText(Vec2f(0, 40), "MoveHead", font);
	}else{
		font.color = RGB(100,100,100)|0xFF000000;
		render->DrawText(Vec2f(0, 40), "MoveHead", font);
	}
}

//------------　拡張インタフェース　-----------//

void CRAttention::SetAttentionPoint(Vec3f position, float ammount){
	attentionList.insert(AttentionListItem(NULL,AttentionInfo(ammount,position)));
}

void CRAttention::SetAttentionSolid(PHSolid* solid, float ammount){
	AttentionListIter findResult = attentionList.find(solid);
	if (findResult==attentionList.end()){
		attentionList.insert(AttentionListItem(solid,AttentionInfo(ammount,solid->GetCenterPosition())));
	}else{
		findResult->second.ammount += ammount;
	}
}

void CRAttention::StartExperiment(){
	std::ifstream ifs("att_expr.txt");
	
	timinglist.clear();
	poslist.clear();

	int t; Vec3f p;
	while (ifs >> t){
		ifs >> p;
		timinglist.push_back(t);
		poslist.push_back(p);
	}
	
	bExperimentMode = true;
	startTiming = GetTickCount();
	counter = 0;

	ofs_head.open("head.plt");
	ofs_hedg.open("hedg.plt");
	ofs_leye.open("leye.plt");
	ofs_reye.open("reye.plt");
	ofs_gaze.open("gaze.plt");
	ofs_eyes.open("eyes.plt");
	ofs_info.open("info.plt");
}

//-----------------　処理　----------------//

void CRAttention::CalcMaxAttentionPoint(){
	maxAttentionSolid   = NULL;
	maxAttentionAmmount = -99999.0f;
	bFoundAttention = false;

	for(AttentionListIter it=attentionList.begin(); it!=attentionList.end(); it++){
		PHSolid* solid    = it->first;
		float    ammount  = it->second.ammount;
		Vec3f    position = it->second.position;
		if (ammount > maxAttentionAmmount){
			maxAttentionPoint   = position;
			maxAttentionSolid   = solid;
			maxAttentionAmmount = ammount;
			bFoundAttention = true;
		}
	}
}

}	// end of namespace Spr
