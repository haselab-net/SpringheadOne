#include "Creature.h"
#pragma hdrstop
// CRInternalModel.cpp: CRInternalModel クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRInternalModel.h"
#include "CRPuppet.h"
#include "CRBrain.h"
#include "CRAttention.h"
//#include <WinBasis/WBIniFile.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRInternalModel::CRInternalModel(){
	// コンストラクタ
	iPuppetBrain = new CRBaseBrain();
}

CRInternalModel::~CRInternalModel(){
	// デストラクタ
}

//------------------　基本的なインタフェース　------------------//

void CRInternalModel::Step(){
	//// シミュレーション
	internalModel->ClearForce();
	//
	iPuppetBrain->Step();
	internalUser.Step(internalModel);
	internalPuppet.Step(internalModel);
	DecayCertainty();
	if (bVisualUpdate){
		//// 視覚の適用
		bool stability = ApplyVision();
		if (!bStability){ bStability = stability; }
		ApplyRealPuppet();
	}
	internalModel->GenerateForce();
	internalModel->Integrate();

	PHSolid* soHead;
	internalModel->FindObject(soHead, "soHead");
	//DSTR << "Internal soHead:" << soHead << std::endl;
}

void CRInternalModel::Load(std::string fn, GRRender* render, CRPuppet* realPuppet){
	this->realPuppet = realPuppet;

	internalModel = new SGScene();

	// シーンをロード
	internalModel->Clear();
	internalModel->GetRenderers().Add(render);
	//// シミュレーション用のファイルが必要 (元のファイルの先頭に「Simulation_」を付ける )
	//// ∵　計算量削減のため。簡略化したモデルを使用する
	/**/
	std::string loadFileName = fn.insert(fn.find_last_of('\\')+1, "Simulation_");
	/*/
	std::string loadFileName = fn;
	/**/
	fileIO.Load(loadFileName.c_str(), internalModel);
	//internalModel->SetTimeStep();
	//DSTR << "InternalModel->GetTimeStep : " << internalModel->GetTimeStep() << std::endl;

	// ユーザの内部モデルをロード
	internalUser.Load(internalModel);
	// VHの内部モデルをロード
	internalPuppet.Load(internalModel);
	// VHの脳の内部モデルをロード
	iPuppetBrain->Load(internalModel, &internalPuppet, &internalUser);

	internalModel->FindObject(soRFArmU, "soRFArmU");
	internalModel->FindObject(soRHandU, "soRHandU");
	internalModel->FindObject(soLFArmU, "soLFArmU");
	internalModel->FindObject(soLHandU, "soLHandU");

	internalModel->FindObject(soRFArm, "soRFArm");
	internalModel->FindObject(soRHand, "soRHand");
	internalModel->FindObject(soLFArm, "soLFArm");
	internalModel->FindObject(soLHand, "soLHand");
	
	Init();
}

void CRInternalModel::Init(){
	// 初期化
	bVisualUpdate = true;
	bStability = true;

	// 確信テーブルの初期化
	certaintyTable.clear();
	for(int i=0; i<internalUser.solids.size(); i++){
		certaintyTable.push_back(SolidCertainty(internalUser.solids[i]->GetFrame()->GetPosture(), 0.0f));
	}

	// Iniファイル読み込み
	//Kp = WBIniFile::getValue<float>("InternalModel","Kp",2000.0f);
	//Kd = WBIniFile::getValue<float>("InternalModel","Kd",10.0f);
	Kp  = 1000.0f;
	Kd  = 3.0f;
	Ktp = 10.0f;
	Ktd = 5.0f;
}

void CRInternalModel::OnKeyDown(UINT &nChar){
	// キー入力
}

void CRInternalModel::Draw(GRRender* render){
	/*
	// デバッグ用表示
	render->SetModelMatrix(Affinef());
	Vec3f v[50];

	// User
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,1,1)));
	for (int i=0; i<internalUser.solids.size(); i++){
		int certainty = 1 + int(certaintyTable[i].certainty * 10.0);
		if(certainty > 30){certainty = 30;}
		render->SetLineWidth(certainty);
		v[0] = internalUser.solids[i]->GetCenterPosition();
		render->DrawDirect(Spr::GRRender::POINTS,  v, v+1);
	}

	v[0] = soLHandU->GetCenterPosition();
	v[1] = soLFArmU->GetCenterPosition();
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);
	
	v[0] = soRHandU->GetCenterPosition();
	v[1] = soRFArmU->GetCenterPosition();
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	// Puppet
	render->SetLineWidth(5);
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,1,1,1)));
	for (int i=0; i<internalPuppet.solids.size(); i++){
		v[i] = internalPuppet.solids[i]->GetCenterPosition();
	}
	render->DrawDirect(Spr::GRRender::POINTS,  v, v+(internalPuppet.solids.size()));

	v[0] = soLHand->GetCenterPosition();
	v[1] = soLFArm->GetCenterPosition();
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);
	
	v[0] = soRHand->GetCenterPosition();
	v[1] = soRFArm->GetCenterPosition();
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	GRFont font(50, "MS ゴシック");
	if (!bStability){
		render->DrawText(Vec2f(100, 100), "Unstable", font);
	}
	*/
}


//------------　拡張インタフェース　-----------//
//// Obsolete予定
void CRInternalModel::SetPosition(int i, Vec3f pos){
	DSTR << "[CAUTION] Using Obsolete Method : CRInternalModel::SetPosition" << std::endl;
}

void CRInternalModel::EnableVisualUpdate(){
	bVisualUpdate = true; 
	bStability = false;
	internalModel->LoadState(stateTemp);
}

void CRInternalModel::DisableVisualUpdate(){
	bVisualUpdate = false;
	stateTemp.clear();
	internalModel->SaveState(stateTemp);
}

void CRInternalModel::UpdateCertainty(int i, Affinef posture, float certainty){
	certaintyTable[i].posture   = posture;
	certaintyTable[i].certainty = 0.0f;
}

void CRInternalModel::AttentionFromCertainty(CRAttention* crAttention){
	// 確信度に由来する注目
	/*
	for (int i=0; i<certaintyTable.size(); i++){
		// 未実装
	}
	*/
}

//-----------------　処理　----------------//
bool CRInternalModel::ApplyVision(){
	bool stability = true;
	for(int i=0; i<certaintyTable.size(); i++){
		Vec3f goalPos = certaintyTable[i].posture.Pos();
		Vec3f goalPosture = certaintyTable[i].posture.Rot();
		
		Vec3f currPos = internalUser.solids[i]->GetCenterPosition();
		Vec3f currPosture = internalUser.solids[i]->GetFrame()->GetPosture().Rot();
		Vec3f currVel = internalUser.solids[i]->GetVelocity();

		float Kpm = 1.0f, Kdm = 1.0f;

		if (i==5 || i==8){
			Kpm = 3.0f; Kdm = 3.0f;
		}

		if (i==6 || i==9){
			Kpm = 3.0f; Kdm = 3.0f;
		}

		Vec3f force  = ((Kpm*Kp*(goalPos - currPos)) - (Kdm*Kd*currVel));// * internalUser.solids[i]->GetMass();
		Vec3f torque = (Ktp*(goalPosture - currPosture)) - (Ktd * internalUser.solids[i]->GetAngularVelocity());

		internalUser.solids[i]->AddForce(force, currPos);
		//internalUser.solids[i]->AddTorque(torque);

		// DSTR << i << " : " << (currPos - goalPos).norm() << std::endl;

		stability = stability && ((currPos - goalPos).norm() < 0.03f);
	}
	return stability;
}

bool CRInternalModel::ApplyRealPuppet(){
	bool stability = true;
	for (int i=0; i < (realPuppet->solids.size()); i++){
		Vec3f goalPos  = realPuppet->solids[i]->GetCenterPosition();
		Vec3f goalPosture = realPuppet->solids[i]->GetFrame()->GetPosture().Rot();
		
		Vec3f currPos = internalPuppet.solids[i]->GetCenterPosition();
		Vec3f currPosture = internalPuppet.solids[i]->GetFrame()->GetPosture().Rot();
		Vec3f currVel = internalPuppet.solids[i]->GetVelocity();

		Vec3f force = (Kp*(goalPos - currPos)) - (Kd*currVel);
		Vec3f torque = (Ktp*(goalPosture - currPosture)) - (Ktd * internalPuppet.solids[i]->GetAngularVelocity());

		internalPuppet.solids[i]->AddForce(force, currPos);
		//internalPuppet.solids[i]->AddTorque(torque);

		stability = stability && ((currPos - goalPos).norm() < 0.03f);
	}
	return stability;
}

void CRInternalModel::DecayCertainty(){
	float maxCertainty = 5.0f;
	float decayRate = 0.99f;
	for (int i=0; i<certaintyTable.size(); i++){
		certaintyTable[i].certainty = ((certaintyTable[i].certainty - maxCertainty) * decayRate) + maxCertainty;
	}
}

}	// end of namespace Spr
