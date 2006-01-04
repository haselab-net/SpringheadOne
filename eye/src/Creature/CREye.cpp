#include "Creature.h"
#pragma hdrstop
// CREye.cpp: CREye クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CREye.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CREye::CREye(){
	// コンストラクタ
}

CREye::~CREye(){
	// デストラクタ
}

//------------------　基本的なインタフェース　------------------//

void CREye::Step(){
	if (bAttention){
		ControlEyes();
	}

	bAttention = false;
}

void CREye::Load(SGScene* scene,CRPuppet* crPuppet){
	this->scene = scene;
	this->crPuppet = crPuppet;

	if (crPuppet->IsLoaded()){
		// VHの目に関する要素
		//// Solid
		scene->FindObject(soREye, "soREye");
		scene->FindObject(soLEye, "soLEye");
		//// Frame
		scene->FindObject(frREye, "frREye");
		scene->FindObject(frLEye, "frLEye");
		scene->FindObject(frHead, "frHead");
		//// BallJoint PID Controller
		scene->FindObject(jpREye, "jpREye");
		scene->FindObject(jpLEye, "jpLEye");
	}

	Init();
}

void CREye::Init(){
	// 初期化
	bAttention = false;
	bVHPointOfView = false;
	lastInRangeDir[0] = lastInRangeDir[1] = frHead->GetPosture().Rot() * Vec3f(0.0f,0.0f,-1.0f);

	bEyeMode = 2;
}

void CREye::OnKeyDown(UINT &nChar){
	// キー入力
}

void CREye::Draw(GRRender* render){
	// デバッグ用表示
	render->SetModelMatrix(Affinef());
	Vec3f v[2];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,0,1)));
	render->SetLineWidth(10);

	//// Attention Pointを表示
	v[0] = attentionPoint;
	render->DrawDirect(Spr::GRRender::POINTS,  v, v+1);

	//// 視線表示
	v[0] = soLEye->GetCenterPosition();
	v[1] = v[0] + frLEye->GetPosture().Rot()*Vec3f(0.0f,0.0f,5.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	//// 視線表示
	v[0] = soREye->GetCenterPosition();
	v[1] = v[0] + frREye->GetPosture().Rot()*Vec3f(0.0f,0.0f,5.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);
}

//------------　拡張インタフェース　-----------//

void CREye::SetAttentionPoint(Vec3f position){
	attentionPoint = position;
}

Vec3f CREye::GetVisualAxis(){
	Vec3f visualAxis = (frREye->GetPosture() + frLEye->GetPosture()) * Vec3f(0.0f, 0.0f, 0.5f);
	visualAxis = visualAxis / visualAxis.norm();
	return visualAxis;
}

void CREye::SetAttentionMode(){
	bAttention = true;
}

bool CREye::IsOverRange(){
	return(IsOverRange(attentionPoint - frHead->GetPosture().Pos()));
}

bool CREye::IsOverRange(Vec3f goal){
	// 単位：度
	float vertLimit  = 30.0f;
	float horizLimit = 60.0f;

	//Vec3f goal = attentionPoint - frHead->GetPosture().Pos();
	Vec3f head = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	goal = frHead->GetPosture().Rot().inv() * (goal / goal.norm());
	head = frHead->GetPosture().Rot().inv() * (head / head.norm());

	goal[0] *= (horizLimit / vertLimit);
	goal /= goal.norm();

	bool result = PTM::dot(goal,head) < cos(Rad(vertLimit));

	return(result);
}

bool CREye::IsVisible(PHSolid* solid){
	float limit = 60.0f;

	Vec3f pos, eye;

	// for Left Eye
	pos = solid->GetCenterPosition() - frLEye->GetPosture().Pos();
	pos /= pos.norm();
	eye = frLEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
	
	bool result = PTM::dot(pos,eye) > cos(Rad(limit));

	// for Right Eye
	pos = solid->GetCenterPosition() - frREye->GetPosture().Pos();
	pos /= pos.norm();
	eye = frREye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);

	result = result || (PTM::dot(pos,eye) > cos(Rad(limit)));

	return(result);
}

float CREye::GetVisibility(PHSolid* solid){
	float fovea = 15.0f;
	float limit = 60.0f;
	Vec3f pos,eye;
	float result1, result2;

	// for Left Eye
	pos = solid->GetCenterPosition() - frLEye->GetPosture().Pos();
	pos /= pos.norm();
	eye = frLEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
	if       (PTM::dot(pos,eye) < cos(Rad(limit))){
		result1 = 999.9f;
	}else if (PTM::dot(pos,eye) < cos(Rad(fovea))){
		result1 = 0.05f;
	}else{
		result1 = 0.0f;
	}
	
	// for Right Eye
	pos = solid->GetCenterPosition() - frLEye->GetPosture().Pos();
	pos /= pos.norm();
	eye = frLEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
	if       (PTM::dot(pos,eye) < cos(Rad(limit))){
		result2 = 999.9f;
	}else if (PTM::dot(pos,eye) < cos(Rad(fovea))){
		result2 = 0.05f;
	}else{
		result2 = 0.0f;
	}

	return((result1 < result2) ? result1 : result2);
}

//-----------------　処理　----------------//

void CREye::ControlEyes(){
	ControlEye(frREye, soREye, jpREye, 0);
	ControlEye(frLEye, soLEye, jpLEye, 1);
}

void CREye::ControlEye(SGFrame* frEye, PHSolid* soEye, PHJointBallPid* jpEye, const int eyeType){
	if(bEyeMode==2){
		if (IsOverRange()){
			attentionDir[eyeType] = lastInRangeDir[eyeType];
		}else{
			attentionDir[eyeType] = attentionPoint - frEye->GetPosture().Pos();
			attentionDir[eyeType] = attentionDir[eyeType]/attentionDir[eyeType].norm();
			lastInRangeDir[eyeType] = attentionDir[eyeType];
		}
	}else{
		attentionDir[eyeType] = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	}
	Vec3f current = frEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
	Vec3f error  = PTM::cross(current, attentionDir[eyeType]);
	Vec3f derror = soEye->GetAngularVelocity();
	float Kp = 2000.0f;
	float Kd = 40.0f;
	Vec3f torque = (Kp * error) - (Kd * derror);
 	soEye->AddTorque(torque);
}

}	// end of namespace Spr
