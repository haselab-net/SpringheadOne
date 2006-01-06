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
		scene->FindObject(soHead, "soHead");
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
	attentionDirL = frHead->GetPosture().Rot() * Vec3f(0.0f,0.0f,-1.0f);
	attentionDirR = frHead->GetPosture().Rot() * Vec3f(0.0f,0.0f,-1.0f);

	last_t1_a = 0.0f;
	last_t2_a = 0.0f;

	integrator_L = 0.0f; integrator_R = 0.0f;

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
	if(bEyeMode==2){
		if (!IsOverRange()){
			DeterminAttentionDir();
		}
	}else{
		attentionDirL = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
		attentionDirR = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	}

	// 目標視線へ向けて制御
	ControlEyePD(frREye, soREye, attentionDirR);
	ControlEyePD(frLEye, soLEye, attentionDirL);
}

void CREye::ControlEyePD(SGFrame* frEye, PHSolid* soEye, Vec3f aim){
	Vec3f current = frEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
	current /= current.norm(); aim /= aim.norm();
	Vec3f error  = PTM::cross(current, aim);
	Vec3f derror = soEye->GetAngularVelocity();
	float Kp = 2000.0f;
	float Kd = 40.0f;
	Vec3f torque = (Kp * error) - (Kd * derror);
 	soEye->AddTorque(torque);
}

void CREye::DeterminAttentionDir(){
	const float alpha1 = 0.5f;
	const float rho1 = 1.5f;
	const float rho2 = 0.5f;
	const float sigma = 1.0f;
	const float kappa = 0.5f;
	const float nu = 0.5f;
	const float eta = 0.2f;

	float PI = 3.141592653f;
	float w  = (frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f))[1];
	float t1 = (frLEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f,  1.0f))[1];;
	float t2 = (frREye->GetPosture().Rot() * Vec3f(0.0f, 0.0f,  1.0f))[1];;

	float dt = scene->GetTimeStep();

	Vec3f vecL = attentionPoint - soLEye->GetCenterPosition();;
	float t1_a = atan2(vecL[0],vecL[2]);
	float eL   = t1_a - t1;
	float vL   = (t1_a - last_t1_a) / dt;
	last_t1_a = t1_a;

	Vec3f vecR = attentionPoint - soREye->GetCenterPosition();;
	float t2_a = atan2(vecR[0],vecR[2]);
	float eR   = t2_a - t2;
	float vR   = (t2_a - last_t2_a) / dt;
	last_t2_a = t2_a;

	if (/**/true/*/(abs(eL) > Rad(5.0f)) || (abs(eR) > Rad(5.0f))/**/){
		// Saccade
		attentionDirL = attentionPoint - frLEye->GetPosture().Pos();
		attentionDirR = attentionPoint - frREye->GetPosture().Pos();
		integrator_L = 0.0f; integrator_R = 0.0f;
	}else{
		// Smooth Pursuit
		float head_ang_vel = soHead->GetAngularVelocity()[1];
		float node_L_1 = -(sigma*eL + nu*vL) + (kappa*eR + eta*vR) + head_ang_vel;
		float node_R_1 =  (sigma*eR + nu*vR) - (kappa*eL + eta*vL) - head_ang_vel;
		
		integrator_L += node_L_1 * dt;
		integrator_R += node_R_1 * dt;
		
		float out_t1 = -(integrator_L * rho1) + (integrator_R * rho2);
		float out_t2 = -(integrator_L * rho2) + (integrator_R * rho1);

		out_t1 += w;
		out_t2 += w;
		
		attentionDirL = Vec3f(-cos(out_t1), 0.0f, sin(out_t1));
		attentionDirR = Vec3f(-cos(out_t2), 0.0f, sin(out_t2));
	}
}

}	// end of namespace Spr
