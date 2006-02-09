#define DSTRCHK(var) DSTR << #var << " : " << (var) << GetTickCount() << std::endl;

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
	}else{
		jpNeck->proportional = 30.0f;
		jpNeck->differential = 10.0f;
		jpNeck->integral     = 0.0f;
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

		// 目
		scene->FindObject(soLEye, "soLEye");
		scene->FindObject(soREye, "soREye");
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
	if ((position - attentionPoint).norm() > 0.1f) {
		startVisualAxis = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
		s=0.0f;
	}

	goalVisualAxis = position - frHead->GetPosture().Pos();
	goalVisualAxis = goalVisualAxis / goalVisualAxis.norm();
	attentionPoint = position;
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
	// Re-set JointPID Parameter
	jpNeck->proportional =  0.0f;  // 30.0f
	jpNeck->differential = 30.0f;  // 3.0f
	jpNeck->integral     =  0.0f;
	
	double t = (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
	Vec3f goal = startVisualAxis + (goalVisualAxis - startVisualAxis)*t;
	//goal = frChest->GetPosture().Rot().inv() * goal;
	//DSTR << goal << std::endl;

	// Head Up
	float Kup = 400.0f;
	float errUp = asin((joNeck->GetOrientation() * Vec3f(0.0f, 1.0f, 0.0f))[0]);
	joNeck->AddTorque(frChest->GetPosture().Rot() * Vec3f(0.0f, errUp * Kup, 0.0f));

	// Horizontal Move
	float Khoriz = 400.0f;
	currHoriz = asin((joNeck->GetOrientation() * Vec3f(0.0f, 0.0f, 1.0f))[0]);
	goalHoriz = -atan2(goal[0], goal[2]);
	if (goalHoriz >  0.96f) {goalHoriz =  0.96f;}
	if (goalHoriz < -0.96f) {goalHoriz = -0.96f;}
	float errHoriz = goalHoriz - currHoriz;
	joNeck->AddTorque(frChest->GetPosture().Rot() * Vec3f(0.0f, 0.0f, errHoriz * Khoriz));

	// Vertical Move
	float Kvert = 400.0f;
	currVert = asin((joNeck->GetOrientation() * Vec3f(0.0f, 0.0f, 1.0f))[1]);
	goalVert = -atan2(goal[1], goal[2]);
	if (goalVert >  0.55f) {goalVert =  0.55f;}
	if (goalVert < -0.55f) {goalVert = -0.55f;}
	float errVert = goalVert - currVert;
	joNeck->AddTorque(frChest->GetPosture().Rot() * Vec3f(errVert * Kvert, 0.0f, 0.0f));

	
	// Inclement Time
	s += 0.1f;
	if(s > 1.0f){s=1.0f;}

	headpos = currHoriz;
	headposgoal = goalHoriz;

}

Matrix3f CRNeckController::GetHeadOrientation(){
	Matrix3f zConv = Matrix3f::Unit();	zConv[2][2] = -1;
	return(frHead->GetPosture().Rot() * zConv);
}

Vec3f CRNeckController::LimitRange(Vec3f goal, float vertLimit, float horizLimit){
	Vec3f goalRelHead = GetHeadOrientation().inv() * goal;
	if(goalRelHead[0] < 0.0f){
		return(GetHeadOrientation() * (Affinef::Rot(Rad( horizLimit),'Y')*goalRelHead));
	}else{
		return(GetHeadOrientation() * (Affinef::Rot(Rad(-horizLimit),'Y')*goalRelHead));
	}
}

}	// end of namespace Spr
