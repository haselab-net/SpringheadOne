#include "Creature.h"
#pragma hdrstop
// CRNeckController.cpp: CRNeckController クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRNeckController.h"
#include "CRPuppet.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRNeckController::CRNeckController(){
	// コンストラクタ
}

CRNeckController::~CRNeckController(){
	// デストラクタ
}

//------------------　基本的なインタフェース　------------------//

void CRNeckController::Step(){
	if (bAttention){
		ControlNeck();
	}

	bAttention = false;
}

void CRNeckController::Load(SGScene* scene,CRPuppet* crPuppet){
	this->scene = scene;
	this->crPuppet = crPuppet;

	if (crPuppet->IsLoaded()){
		// VHの首とか頭
		//// Solid
		scene->FindObject(soHead, "soHead");
		//// Frame
		scene->FindObject(frHead, "frHead");
		scene->FindObject(frChest, "frChest");
		//// BallJoint
		scene->FindObject(joNeck, "joNeck");
		scene->FindObject(jpNeck, "jpNeck");
	}

	Init();
}

void CRNeckController::Init(){
	// 初期化
	bAttention = false;
}

void CRNeckController::OnKeyDown(UINT &nChar){
	// キー入力
}

void CRNeckController::Draw(GRRender* render){
	// デバッグ用表示
	render->SetModelMatrix(Affinef());
	Vec3f v[2];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,0,1,1)));
	render->SetLineWidth(10);

	//// 頭の正面方向の表示
	v[0] = frHead->GetPosture().Pos();
	v[1] = v[0] + frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -100.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);
}

//------------　拡張インタフェース　-----------//

void CRNeckController::SetAttentionPoint(Vec3f position){
	goalVisualAxis = position - frHead->GetPosture().Pos();
	goalVisualAxis = goalVisualAxis / goalVisualAxis.norm();
}

void CRNeckController::SetAttentionMode(){
	bAttention = true;
}

float CRNeckController::GetErrAbs(){
	Vec3f goal = goalVisualAxis;
	Vec3f current = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	return PTM::cross(goal,current).norm();
}

//-----------------　処理　----------------//

void CRNeckController::ControlNeck(){
	// 首を制御しているPHJointBallPidのパラメータをここで調整する
	// (こんな所に書くと後の人が混乱する気がする)
	jpNeck->proportional = 30.0f;
	jpNeck->differential = 10.0f;
	jpNeck->integral     = 0.0f;

	Vec3f goal = goalVisualAxis;
	Vec3f current = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);

	// Yaw
	Vec3f goalY = goal; 	 goalY[1] = 0.0f;
	Vec3f currY = current; currY[1] = 0.0f;
	float errorY = PTM::cross(goalY, currY)[1];
	float derrorY = joNeck->GetVelocity()[1];

	// Pitch
	Vec3f goalP = goal; 	 goalP[0] = 0.0f;
	Vec3f currP = current; currP[0] = 0.0f;
	float errorP = PTM::cross(goalP, currP)[0];
	float derrorP = joNeck->GetVelocity()[0];

	// Roll
	Vec3f goalR = Vec3f(0.0f, 1.0f, 0.0f);
	Vec3f currR = current; currR[2] = 0.0f;
	float errorR = PTM::cross(goalR, currR)[2];
	float derrorR = joNeck->GetVelocity()[2];

	float Kp = 100.0f;
	float Kd = 0.0f;
	Vec3f torque = Vec3f(
											 -((Kp * errorP) - (Kd * derrorP)),
											 0.0f, //-((Kp * errorR) - (Kd * derrorR)),
											 -((Kp * errorY) - (Kd * derrorY))
											 );
	torque = frChest->GetPosture().Rot() * torque;
	torque[1] = -torque[1];
	torque[2] = -torque[2];

	// For Debug
	//torque[0] = 0.0f;
	//torque[2] = 0.0f;

	joNeck->AddTorque(torque);
}

}	// end of namespace Spr
