#define DSTRCHK(var) DSTR << #var << " : " << (var) << "[" << GetTickCount() << "]" << std::endl;

#include "Creature.h"
#pragma hdrstop
// CREye.cpp: CREye クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CREye.h"
#include "CRNeckController.h"

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

void CREye::Load(SGScene* scene,CRPuppet* crPuppet,CRNeckController* crNeck){
	this->scene = scene;
	this->crPuppet = crPuppet;
	this->crNeck = crNeck;

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
	last_t3_a = 0.0f;
	last_t4_a = 0.0f;

	integrator_L = 0.0f; integrator_R = 0.0f;

	bEyeMode = 2;
	bSaccade = 0;
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

	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,0,1)));
	//// Attention Pointを表示
	v[0] = attentionPoint;
	render->DrawDirect(Spr::GRRender::POINTS,  v, v+1);

	//// Attention Dirを表示
	v[0] = soLEye->GetCenterPosition();
	v[1] = v[0] + attentionDirL * 5.0f;
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	//// Attention Dirを表示
	v[0] = soREye->GetCenterPosition();
	v[1] = v[0] + attentionDirR * 5.0f;
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,0,1)));
	//// 視線表示
	v[0] = soLEye->GetCenterPosition();
	v[1] = v[0] + frLEye->GetPosture().Rot()*Vec3f(0.0f,0.0f,5.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	//// 視線表示
	v[0] = soREye->GetCenterPosition();
	v[1] = v[0] + frREye->GetPosture().Rot()*Vec3f(0.0f,0.0f,5.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);

	// デバッグ用表示
	GRFont font(50, "MS ゴシック");
	if (bSaccade){
		font.color = RGB(255,255,255)|0xFF000000;
		render->DrawText(Vec2f(0, 80), "Saccade", font);
	}else{
		font.color = RGB(100,100,100)|0xFF000000;
		render->DrawText(Vec2f(0, 80), "Saccade", font);
	}
}

//------------　拡張インタフェース　-----------//

void CREye::SetAttentionPoint(Vec3f position){
	if (/*/!bSaccade/**/true/**/){
		attentionPoint = position;
	}
}

Vec3f CREye::GetVisualAxis(){
	Vec3f dirL = soLEye->GetFrame()->GetPosture().Rot() * Vec3f(0,0,1);
	Vec3f dirR = soREye->GetFrame()->GetPosture().Rot() * Vec3f(0,0,1);
	Vec3f axis = ((dirL + dirR) * 0.5f);
	return(axis / axis.norm());
}

void CREye::SetAttentionMode(){
	bAttention = true;
}

bool CREye::IsOverRange(float vertLimit, float horizLimit){
	bool result = IsOverRange(attentionPoint - frHead->GetPosture().Pos(), vertLimit, horizLimit);
	return(result);
}
	
bool CREye::IsOverRange(){
	bool result = IsOverRange(attentionPoint - frHead->GetPosture().Pos());
	return(result);
}

bool CREye::IsOverRange(Vec3f goal){
	// 単位：度
	float vertLimit  = 20.0f;
	float horizLimit = 55.0f;
	return(IsOverRange(goal, vertLimit, horizLimit));
}

bool CREye::IsOverRange(Vec3f goal, float vertLimit, float horizLimit){
	goal = frHead->GetPosture().Rot().inv() * (goal / goal.norm());
	goal[2] = -goal[2];

	if(PTM::dot(Vec3f(0.0f,0.0f,1.0f),goal) < cos(Rad(horizLimit))){
		//DSTR << "true : " << std::endl;
		return(true);
	}
	//if(goal[2]<0){return(true);}

	goal /= goal[2];
	float x = goal[0], y = goal[1];
	float a = tan(Rad(horizLimit)), b = tan(Rad(vertLimit));
	bool result = ((((x*x)/(a*a)) + ((y*y)/(b*b))) >= 1.0f);

	//DSTR << "!! : " << (((x*x)/(a*a)) + ((y*y)/(b*b))) << std::endl;

	return(result);	
}

#define sgn(x) (((x)>=0) ? (+1) : (-1))

Vec3f CREye::LimitRange(Vec3f goal){
	float vertLimit  = 30.0f;
	float horizLimit = 55.0f;

	return(LimitRange(goal, vertLimit, horizLimit));
}

Vec3f CREye::LimitRange(Vec3f goal, float vertLimit, float horizLimit){
	goal = crNeck->GetHeadOrientation().inv() * (goal / goal.norm());

	//goal = frHead->GetPosture().Rot().inv() * (goal / goal.norm());
	//goal[0] = -goal[0];
	//goal[2] = -goal[2];

	float ax = goal[0], ay = goal[1];
	float T1 = tan(Rad(horizLimit)), T2 = tan(Rad(vertLimit));
	float x = sgn(ax)/sqrt((1/(T1*T1)) + (ay/(ax*T2))*(ay/(ax*T2)));
	float y = sgn(ay)/sqrt((ax/(ay*T1))*(ax/(ay*T1)) + (1/(T2*T2)));

	//DSTR << "G:" << goal << std::endl;
	//DSTR << "X:" << x << std::endl;
	//DSTR << "M:" << Vec3f(x,y,1) << std::endl;

	//Vec3f result = frHead->GetPosture().Rot() * Vec3f(-x, y, -1.0f);
	Vec3f result = crNeck->GetHeadOrientation() * Vec3f(x, y, 1.0f);

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
		DeterminAttentionDir();
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
	current /= current.norm(); 

	if (IsOverRange(aim)){
		aim = LimitRange(aim);
	}
	aim /= aim.norm();

	//if (IsOverRange()){overrange=1.0f;}else{overrange=0.0f;}

	Vec3f error  = PTM::cross(current, aim);
	Vec3f derror = soEye->GetAngularVelocity();
	float Kp = 5000.0f;
	float Kd = 50.0f;
	Vec3f torque = (Kp * error) - (Kd * derror);

 	soEye->AddTorque(torque);
}

void CREye::DeterminAttentionDir(){
	/**/
	const float alpha1 = 0.5f;
	const float rho1 = 1.50f;
	const float rho2 = 0.50f;
	// Error
	const float sigma = 100.0f;
	const float kappa = 50.0f;
	// Velocity
	const float nu =  0.5f;
	const float eta = 0.2f;
	/*/
	const float alpha1 = 0.5f;
	const float rho1 = 1.50f;
	const float rho2 = 0.50f;
	const float sigma = 1.0f;
	const float kappa = 0.5f;
	const float nu =  0.5f;
	const float eta = 0.2f;
	/**/

	Vec3f apHoriz = frHead->GetPosture().inv() * attentionPoint;
	apHoriz[1] = 0.0f;
	apHoriz = frHead->GetPosture() * apHoriz;

	Vec3f apVert = frHead->GetPosture().inv() * attentionPoint;
	apVert[0] = 0.0f;
	apVert = frHead->GetPosture() * apVert;

	float PI = 3.141592653f;
	float dt = scene->GetTimeStep();

	Vec3f headDirVec = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	float w = atan2(headDirVec.Z(), -headDirVec.X());
	float wv = atan2(headDirVec.Y(),  headDirVec.Z());
	Vec3f eyeDirRelL = frHead->GetPosture().Rot().inv() * frLEye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	Vec3f eyeDirRelR = frHead->GetPosture().Rot().inv() * frREye->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	float t1 =  atan2(eyeDirRelL.Z(), -eyeDirRelL.X());
	float t2 =  atan2(eyeDirRelR.Z(), -eyeDirRelR.X());
	float t3 =  -atan2(eyeDirRelL.Y(),  eyeDirRelL.Z());
	float t4 =  -atan2(eyeDirRelR.Y(),  eyeDirRelR.Z());
	float dw = soHead->GetAngularVelocity().Y();
	float dwv = soHead->GetAngularVelocity().X();

	Vec3f vecL = apHoriz - soLEye->GetCenterPosition();
	vecL = frHead->GetPosture().Rot().inv() * vecL;
	float t1_a = atan2(-vecL.Z(),vecL.X());
	float eL   = t1_a - t1;
	float vL   = (t1_a - last_t1_a) / dt;
	last_t1_a = t1_a;

	Vec3f vecR = apHoriz - soREye->GetCenterPosition();
	vecR = frHead->GetPosture().Rot().inv() * vecR;
	float t2_a = atan2(-vecR.Z(),vecR.X());
	float eR   = t2_a - t2;
	float vR   = (t2_a - last_t2_a) / dt;
	last_t2_a = t2_a;

	Vec3f vecLV = apVert - soLEye->GetCenterPosition();
	vecLV = frHead->GetPosture().Rot().inv() * vecLV;
	float t3_a = atan2(vecLV.Y(),-vecLV.Z());
	float eLV  = t3_a - t3;
	float vLV  = (t3_a - last_t3_a) / dt;
	last_t3_a = t3_a;

	Vec3f vecRV = apVert - soREye->GetCenterPosition();
	vecRV = frHead->GetPosture().Rot().inv() * vecRV;
	float t4_a = atan2(vecRV.Y(),-vecRV.Z());
	float eRV  = t4_a - t4;
	float vRV  = (t4_a - last_t4_a) / dt;
	last_t4_a = t4_a;
	
	if (bSaccade){
		// Saccade終了条件
		if ((
				 (abs(eL) < Rad(0.1f)) && (abs(eR)  < Rad(0.1f)) 
				 && 
				 (abs(eLV) < Rad(0.1f)) && (abs(eRV) < Rad(0.1f))
				 ))
				 /*if (saccadeCount < 0)*/
			{
				bSaccade = false;
			}
	}else{
		// Saccade開始条件
		if (( 
				 (abs(eL) > Rad(5.0f)) || (abs(eR)  > Rad(5.0f)) 
				 ||
				 (abs(eLV) > Rad(5.0f)) || (abs(eRV) > Rad(5.0f)) 
				 ) || bForceSaccade)
			{
				saccadeStartTime = GetTickCount();
				saccadeStartDirL = frLEye->GetPosture().Rot() * Vec3f(0.0f,0.0f,1.0f);
				saccadeStartDirR = frREye->GetPosture().Rot() * Vec3f(0.0f,0.0f,1.0f);
				saccadeGoalDirL  = attentionPoint - frLEye->GetPosture().Pos();
				saccadeGoalDirR  = attentionPoint - frREye->GetPosture().Pos();
				saccadeCount = 10;
				bSaccade = true;
			}	
	}

	if (bSaccade){
		// Saccade
		/*
		attentionDirL = attentionPoint - frLEye->GetPosture().Pos();
		attentionDirR = attentionPoint - frREye->GetPosture().Pos();
		*/
		/*
		float s = 1.0f - (saccadeCount / 10.0f);
		double length = (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		Matrix3d headori     = crNeck->GetHeadOrientation();
		Matrix3d headori_inv = headori.inv();
		attentionDirL = headori*((headori_inv*saccadeGoalDirL)*length) + saccadeStartDirL*(1-length); 
		attentionDirR = headori*((headori_inv*saccadeGoalDirR)*length) + saccadeStartDirR*(1-length); 
		saccadeCount--;
		*/
		float aL = acos(PTM::dot(saccadeGoalDirL/saccadeGoalDirL.norm(), saccadeStartDirL/saccadeStartDirL.norm()));
		float aR = acos(PTM::dot(saccadeGoalDirR/saccadeGoalDirR.norm(), saccadeStartDirR/saccadeStartDirR.norm()));
		float t = (float)(GetTickCount() - saccadeStartTime) / 1000.0f;
		float L = Rad(500.0f);
		float T = 0.05f;
		float lengthL = 1.0f;
		float lengthR = 1.0f;
#define theta(x)   (6*pow(((x)/T),5) - 15*pow(((x)/T),4) + 10*pow(((x)/T),3))
#define dtheta(x)  (30*pow((x),4)/pow(T,5) - 60*pow((x),4)/pow(T,4) + 30*pow((x),2)/pow(T,3))
		if (aL*dtheta(T/2.0f) >= L) {
			float a_ = L/dtheta(T/2.0f);
			if (0 <= t && t < T/2.0f) {
				lengthL = (a_*theta(t))/aL;
			} else if (t < ((aL-a_)/L + T/2.0f)) {
				lengthL = (L*t + a_/2.0 -L*T/2.0f)/aL;
			} else if (t < ((aL-a_)/L + T     )) {
				lengthL = (a_*theta(t-(aL-a_)/L) + (aL-a_))/aL;
			}
		} else {
			if (0 <=t && t < T) {
				lengthL = (aL*theta(t))/aL;
			}
		}
		if (aR*dtheta(T/2.0f) >= L) {
			float a_ = L/dtheta(T/2.0f);
			if (0 <= t && t < T/2.0f) {
				lengthR = (a_*theta(t))/aR;
			} else if (t < ((aR-a_)/L + T/2.0f)) {
				lengthR = (L*t + a_/2.0 -L*T/2.0f)/aR;
			} else if (t < ((aR-a_)/L + T     )) {
				lengthR = (a_*theta(t-(aR-a_)/L) + (aR-a_))/aR;
			}
		} else {
			if (0 <=t && t < T) {
				lengthR = (aR*theta(t))/aR;
			}
		}

		Matrix3d headori     = crNeck->GetHeadOrientation();
		Matrix3d headori_inv = headori.inv();
		attentionDirL = headori*((headori_inv*saccadeGoalDirL)*lengthL) + saccadeStartDirL*(1-lengthL); 
		attentionDirR = headori*((headori_inv*saccadeGoalDirR)*lengthR) + saccadeStartDirR*(1-lengthR); 

		if (lengthL>=1.0f && lengthR>=1.0f) { bSaccade = false; }

		integrator_L  = 0.0f; integrator_R  = 0.0f;
		integrator_Lv = 0.0f; integrator_Rv = 0.0f;
	}else{
		// Smooth Pursuit

		//// Horizontal
		float node_L_1 = -(sigma*eL + nu*vL) - (kappa*eR + eta*vR) + (dw*alpha1);
		float node_R_1 =  (sigma*eR + nu*vR) + (kappa*eL + eta*vL) - (dw*alpha1);
		
		integrator_L += node_L_1 * dt;
		integrator_R += node_R_1 * dt;
		
		float out_t1 = -(integrator_L * rho1) + (integrator_R * rho2) + t1;
		float out_t2 = -(integrator_L * rho2) + (integrator_R * rho1) + t2;

		//// Vertical
		float node_L_2 = (sigma*eLV + nu*vLV);// + (dwv*alpha1);
		float node_R_2 = (sigma*eRV + nu*vRV);// + (dwv*alpha1);

		integrator_Lv += node_L_2 * dt;
		integrator_Rv += node_R_2 * dt;
		
		float out_t3 = (integrator_Lv * rho1) + t3;
		float out_t4 = (integrator_Rv * rho1) + t4;

		if (cos(out_t3)!=0){
			attentionDirL = Vec3f(-cos(out_t1), -sin(out_t1)*tan(out_t3), sin(out_t1));
		}else{
			attentionDirL = Vec3f(-cos(out_t1), 0.0f, 0.0f);
		}

		if (cos(out_t4)!=0){
			attentionDirR = Vec3f(-cos(out_t2), -sin(out_t2)*tan(out_t4), sin(out_t2));
		}else{
			attentionDirR = Vec3f(-cos(out_t2), 0.0f, 0.0f);
		}

		attentionDirL = frHead->GetPosture().Rot() * attentionDirL;
		attentionDirR = frHead->GetPosture().Rot() * attentionDirR;

		attentionDirL = -attentionDirL;
		attentionDirR = -attentionDirR;

		//

	}

	if(-PI <= t1 && t1 <= -(PI/2.0f)){ t1 += 2*PI; }
	if(-PI <= t2 && t2 <= -(PI/2.0f)){ t2 += 2*PI; }
	eyeposL = t1 - (PI/2.0f);
	eyeposR = t2 - (PI/2.0f);

	bForceSaccade = false;
}

}	// end of namespace Spr
