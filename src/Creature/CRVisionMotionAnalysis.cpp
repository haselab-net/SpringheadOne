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
	/*
	render->SetModelMatrix(Affinef());
	Vec3f v[50];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
	render->SetLineWidth(10);

	int cnt=0;
	for(int i=0; i<userSolids.size(); i++){
		if (crEye->IsVisible(userSolids[i])){
			//Vec3f x = userSolids[i]->GetCenterPosition();
			//Vec3f a = crEye->GetVisualAxis();
			//a = a / a.norm();
			//v[cnt] = x - (a * PTM::dot(a,x));
			//cnt++;

			Vec3f visualAxis = crEye->GetVisualAxis();
			Vec3f position = userSolids[i]->GetCenterPosition();
			Vec3f dir = position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5);
			dir /= dir.norm();
			float d = acos(PTM::dot(dir, visualAxis));
			d = 2.0f * pow(2.71828f, -(d*d*4.0f));
			render->SetLineWidth((int)(10.0f*d));

			v[0] = userSolids[i]->GetCenterPosition();
			render->DrawDirect(Spr::GRRender::POINTS,  v, v+1);
		}
	}
	*/
}

//-----------------　処理　----------------//

void CRVisionMotionAnalysis::ListupAttractivePoints(){
	Vec3f visualAxis = crEye->GetVisualAxis();

	/////
	// デバッグ用
	//crAttention->SetAttentionSolid(soLHandU, 100.0f);
	//crAttention->SetAttentionSolid(soRHandU, 100.0f);
	/////

//#define MEXICANHAT(a,x)  ((1-2*(x)*(x)*(a)*(a))*pow(2.718,-1*(x)*(x)*(a)*(a)))
	
	/*/
	// 各Solidの運動が注意を引き付ける強さを計算、crAttentionに渡す
	for(int i=0; i<userSolids.size(); i++){
		if (crEye->IsVisible(userSolids[i])){
			Vec3f position = userSolids[i]->GetCenterPosition();
			Vec3f letinalPos = position - (visualAxis * PTM::dot(visualAxis,position));
			Vec3f velocity = userSolids[i]->GetVelocity();
			float r = 1.0f + (position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5)).norm();
			float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() / r;
			DSTR << trnAmmount << std::endl;
			
			for(int j=0; j<userSolids.size(); j++){
				if (j!=i && crEye->IsVisible(userSolids[j])){
					Vec3f position2 = userSolids[j]->GetCenterPosition();
					Vec3f letinalPos2 = position2 - (visualAxis * PTM::dot(visualAxis,position2));
					Vec3f velocity2 = userSolids[j]->GetVelocity();
					float r2 = 1.0f + (position2 - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5)).norm();
					float trnAmmount2 = (velocity2 - (visualAxis * PTM::dot(velocity2,visualAxis))).norm() / r2;
					float t = (letinalPos2 - letinalPos).norm();
					trnAmmount += (trnAmmount2 * MEXICANHAT(10, t));
					DSTR << (trnAmmount2 * MEXICANHAT(10, t)) << std::endl;
				}
			}

			DSTR << trnAmmount << std::endl;
			DSTR << std::endl;
			crAttention->SetAttentionSolid(userSolids[i], trnAmmount * 20);
		}
	}

	
	/*/
	// 各Solidの運動が注意を引き付ける強さを計算、crAttentionに渡す
	for(int i=0; i<userSolids.size(); i++){
		// 顔の引き付ける注意
		float headAtt = 1.5f;
		if (userSolids[i]==soHeadU){
			float headAttentionAmmount = headAtt;
			crAttention->SetAttentionSolid(userSolids[i], headAttentionAmmount);
		}else{
			float nonHeadSupression = -headAtt;
			crAttention->SetAttentionSolid(userSolids[i], nonHeadSupression);
		}
		
		// 視野の内部にある物体に限定
		if (crEye->IsVisible(userSolids[i])){
			Vec3f position = userSolids[i]->GetCenterPosition();
			Vec3f velocity = userSolids[i]->GetVelocity();
			Vec3f angVelocity = userSolids[i]->GetAngularVelocity();
			float r = 1.0f + (position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5)).norm();
			Vec3f dir = position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5);
			if (dir.norm()!=0){ dir /= dir.norm(); }
			float d = PTM::dot(dir, visualAxis);
			if (d>=0.999f) { d = 0.0f; } else { d = acos(d); }
			d = 1.0f * pow(2.71828f, -(d*d*2.0f));
			//d = 1.0f;

			Vec3f letinalPos = position - (visualAxis * PTM::dot(visualAxis,position));
			
			float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() * 3 / r;
			float divAmmount = abs(PTM::dot(velocity,visualAxis)) * 2 / r;
			float rotAmmount = abs(PTM::dot(angVelocity,visualAxis));

			crAttention->SetAttentionSolid(userSolids[i], trnAmmount * d);
			crAttention->SetAttentionSolid(userSolids[i], divAmmount * d);
			crAttention->SetAttentionSolid(userSolids[i], rotAmmount * d);

			if (crInternalModel!=NULL){
				float certainty = crEye->GetVisibility(userSolids[i]);
				Affinef posture = userSolids[i]->GetFrame()->GetPosture();
				crInternalModel->UpdateCertainty(i, posture, certainty);
			}
		}
	}
	/**/

}

}	// end of namespace Spr
