#include "Creature.h"
#pragma hdrstop
// CRVisionMotionAnalysis.cpp: CRVisionMotionAnalysis クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRVisionMotionAnalysis.h"
#include "CRPuppet.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRVisionMotionAnalysis::CRVisionMotionAnalysis(){
	// コンストラクタ
	crInternalModel = NULL;
}

CRVisionMotionAnalysis::~CRVisionMotionAnalysis(){
	// デストラクタ
}

//------------------　基本的なインタフェース　------------------//

void CRVisionMotionAnalysis::Step(){
	ListupAttractivePoints();
}

void CRVisionMotionAnalysis::Load(SGScene* scene, CRPuppet* crPuppet, CREye* crEye, CRAttention* crAttention){
	this->scene = scene;
	this->crPuppet = crPuppet;
	this->crEye = crEye;
	this->crAttention = crAttention;

	if (crPuppet->IsLoaded()){
		userSolids.clear();
		// UserのSolidを読み込み
		///  ...これはあやしい！scene->solidsを使えー (Future Works)
		scene->FindObject(soWaistU,   "soWaistU"  ); userSolids.push_back(soWaistU);
		scene->FindObject(soAbdomenU, "soAbdomenU"); userSolids.push_back(soAbdomenU);
		scene->FindObject(soChestU,   "soChestU"  ); userSolids.push_back(soChestU);
		scene->FindObject(soHeadU,    "soHeadU"   ); userSolids.push_back(soHeadU);
		scene->FindObject(soRUArmU,   "soRUArmU"  ); userSolids.push_back(soRUArmU);
		scene->FindObject(soRFArmU,   "soRFArmU"  ); userSolids.push_back(soRFArmU);
		scene->FindObject(soRHandU,   "soRHandU"  ); userSolids.push_back(soRHandU);
		scene->FindObject(soLUArmU,   "soLUArmU"  ); userSolids.push_back(soLUArmU);
		scene->FindObject(soLFArmU,   "soLFArmU"  ); userSolids.push_back(soLFArmU);
		scene->FindObject(soLHandU,   "soLHandU"  ); userSolids.push_back(soLHandU);
		
		// 眼
		scene->FindObject(soLEye,   "soLEye"  );
		scene->FindObject(soREye,   "soREye"  );
	}
}

void CRVisionMotionAnalysis::Init(){
	// 初期化…なにもしてない
}

void CRVisionMotionAnalysis::OnKeyDown(UINT &nChar){
	// キー入力…なにもしない
}

void CRVisionMotionAnalysis::Draw(GRRender* render){
	// デバッグ用表示
	render->SetModelMatrix(Affinef());
	Vec3f v[50];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
	render->SetLineWidth(10);

	int cnt=0;
	for(int i=0; i<userSolids.size(); i++){
		if (crEye->IsVisible(userSolids[i])){
			v[cnt] = userSolids[i]->GetCenterPosition();
			cnt++;
		}
	}
	render->DrawDirect(Spr::GRRender::POINTS,  v, v+cnt);
}

//-----------------　処理　----------------//

void CRVisionMotionAnalysis::ListupAttractivePoints(){
	Vec3f visualAxis = crEye->GetVisualAxis();

	// 顔はデフォルトで一定量の注意を引き付ける
	float headAttentionAmmount = 1.0f;
	crAttention->SetAttentionSolid(soHeadU, headAttentionAmmount);

	// 各Solidの運動が注意を引き付ける強さを計算、crAttentionに渡す
	for(int i=0; i<userSolids.size(); i++){
		// 視野の内部にある物体に限定
		if (crEye->IsVisible(userSolids[i])){
			Vec3f position = userSolids[i]->GetCenterPosition();
			Vec3f velocity = userSolids[i]->GetVelocity();
			Vec3f angVelocity = userSolids[i]->GetAngularVelocity();
			float r = 1.0f + (position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5)).norm();
			
			float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() / r;
			float divAmmount = abs(PTM::dot(velocity,visualAxis)) / r;
			float rotAmmount = abs(PTM::dot(angVelocity,visualAxis));
			
			crAttention->SetAttentionSolid(userSolids[i], trnAmmount);
			crAttention->SetAttentionSolid(userSolids[i], divAmmount);
			crAttention->SetAttentionSolid(userSolids[i], rotAmmount);

			if (crInternalModel!=NULL){
				float certainty = crEye->GetVisibility(userSolids[i]);
				Affinef posture = userSolids[i]->GetFrame()->GetPosture();
				crInternalModel->UpdateCertainty(i, posture, certainty);
			}
		}
	}

}

}	// end of namespace Spr
