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
	CalcMaxAttentionPoint();
	if (bFoundAttention){
		crEye->SetAttentionPoint(maxAttentionPoint);
		crEye->SetAttentionMode();
		crNeckController->SetAttentionMode();
		if (crEye->IsOverRange()){
			crNeckController->SetAttentionPoint(maxAttentionPoint);
			bHeadControl = true;
		}else{
			float err = crNeckController->GetErrAbs();
			float thresh = 4.0f * pow(2.718f, -(err*err*9.0f));
			if (maxAttentionAmmount > thresh){
				crNeckController->SetAttentionPoint(maxAttentionPoint);
				bHeadControl = true;
			}else{
				bHeadControl = false;
			}
		}
	}

	/*
	float time = (GetTickCount() * 1.0f / 1000.0f);
	Vec3f attentionPoint = Vec3f(sin(time)*0.5f, 1.5f+cos(time)*0.001f, 0.0f);
	crEye->SetAttentionPoint(attentionPoint);
	crEye->SetAttentionMode();
	*/

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
}

void CRAttention::OnKeyDown(UINT &nChar){
	// キー入力
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
