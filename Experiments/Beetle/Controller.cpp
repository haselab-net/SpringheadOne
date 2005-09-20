#include "Controller.h"
#include <assert.h>
#include <Physics/PHContactEngine.h>

using namespace Spr;
extern UTRef<SGScene> g_Scene;

Beetle::Beetle(void)
{
	nState = STOP;
	//fCounterL = 1.0;
	//fCounterR = 0.5;
	fCounter = 0.0;
	
	Legs[LEFT][FRONT].pBody = Legs[LEFT][MIDDLE].pBody = Legs[LEFT][BACK].pBody =
	Legs[RIGHT][FRONT].pBody = Legs[RIGHT][MIDDLE].pBody = Legs[RIGHT][BACK].pBody = this;
	
	bReady = false;
}

Beetle::~Beetle(void)
{
}

bool Beetle::ConnectToSpringhead()
{
	bReady = false;
#define CHECK_DCAST(x) if(!(x))return false
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLFY")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[PITCH]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLFP")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[KNEE]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLFK")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLFH")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLMY")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[PITCH]	= DCAST(PHJoint, g_Scene->FindObject("jntLegLMP")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[KNEE]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLMK")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLMH")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLBY")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[PITCH]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLBP")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[KNEE]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLBK")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegLBH")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRFY")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[PITCH]	= DCAST(PHJoint, g_Scene->FindObject("jntLegRFP")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[KNEE]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRFK")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRFH")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRMY")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[PITCH]	= DCAST(PHJoint, g_Scene->FindObject("jntLegRMP")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[KNEE]	= DCAST(PHJoint, g_Scene->FindObject("jntLegRMK")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRMH")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[YAW]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRBY")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[PITCH]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRBP")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[KNEE]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRBK")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[HAND]		= DCAST(PHJoint, g_Scene->FindObject("jntLegRBH")));
	CHECK_DCAST(pjntNeck[YAW]						= DCAST(PHJoint, g_Scene->FindObject("jntNeckY")));
	CHECK_DCAST(pjntNeck[PITCH]						= DCAST(PHJoint, g_Scene->FindObject("jntNeckP")));
	CHECK_DCAST(pjntTail							= DCAST(PHJoint, g_Scene->FindObject("jntTail")));
#undef CHECK_DCAST
	//関節のPIDパラメータは使ってないのでそこを間借り
	fPValue			= Legs[LEFT][FRONT].pJoint[YAW]->GetInfo().fPValue;
	fIValue			= Legs[LEFT][FRONT].pJoint[YAW]->GetInfo().fIValue;
	fDValue			= Legs[LEFT][FRONT].pJoint[YAW]->GetInfo().fDValue;
	fSpeed			= Legs[LEFT][FRONT].pJoint[PITCH]->GetInfo().fPValue;
	fStride			= Legs[LEFT][FRONT].pJoint[PITCH]->GetInfo().fIValue;
	fClearance		= Legs[LEFT][FRONT].pJoint[PITCH]->GetInfo().fDValue;
	fSwingBackRate	= Legs[LEFT][FRONT].pJoint[KNEE]->GetInfo().fPValue;
	fTurnAngle		= Legs[LEFT][FRONT].pJoint[KNEE]->GetInfo().fIValue;
	Vec3f	toe[3];
	toe[FRONT]	= Vec3f(Legs[LEFT][MIDDLE].pJoint[YAW]	->GetInfo().fPValue, Legs[LEFT][MIDDLE].pJoint[YAW]	 ->GetInfo().fIValue, Legs[LEFT][MIDDLE].pJoint[YAW]	->GetInfo().fDValue);
	toe[MIDDLE] = Vec3f(Legs[LEFT][MIDDLE].pJoint[PITCH]->GetInfo().fPValue, Legs[LEFT][MIDDLE].pJoint[PITCH]->GetInfo().fIValue, Legs[LEFT][MIDDLE].pJoint[PITCH]  ->GetInfo().fDValue);
	toe[BACK]	= Vec3f(Legs[LEFT][MIDDLE].pJoint[KNEE]	->GetInfo().fPValue, Legs[LEFT][MIDDLE].pJoint[KNEE] ->GetInfo().fIValue, Legs[LEFT][MIDDLE].pJoint[KNEE]   ->GetInfo().fDValue);
	toe[0].Z() *= -1;
	toe[1].Z() *= -1;
	toe[2].Z() *= -1;
	Legs[LEFT][FRONT]	.v3ToePosition = toe[FRONT];
	Legs[LEFT][MIDDLE]	.v3ToePosition = toe[MIDDLE];
	Legs[LEFT][BACK]	.v3ToePosition = toe[BACK];
	toe[FRONT].X() *= -1;
	toe[MIDDLE].X() *= -1;
	toe[BACK].X() *= -1;
	Legs[RIGHT][FRONT]	.v3ToePosition = toe[FRONT];
	Legs[RIGHT][MIDDLE]	.v3ToePosition = toe[MIDDLE];
	Legs[RIGHT][BACK]	.v3ToePosition = toe[BACK];
	Legs[LEFT][FRONT]	.Reset();
	Legs[LEFT][MIDDLE]	.Reset();
	Legs[LEFT][BACK]	.Reset();
	Legs[RIGHT][FRONT]	.Reset();
	Legs[RIGHT][MIDDLE]	.Reset();
	Legs[RIGHT][BACK]	.Reset();

	bReady = true;
	return true;
}

void Beetle::SetState(BeetleState stat)
{
	nState = stat;
	if(stat == NEUTRAL)
		nNeutralStep = 0;
}
extern std::vector< UTRef<PHSolid> > bullets;
extern PHContactEngine* contactEngine;
void Beetle::CheckForce(){
	if(!bReady)return;
	for(int i=0; i<2; ++i){
		for(int j=0; j<3; ++j){
			for(int k=1; k<4; ++k){
				PHJoint* joint = Legs[i][j].pJoint[k];
				Vec3f f;
				for(int i=0; i<bullets.size(); ++i){
					if (bullets[i]){
						PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(joint->solid->GetFrame(), bullets[i]->GetFrame());
						f += fpr->reflexForce;
					}
				}
				if (f.norm() > 40.0f){
					Legs[i][j].pJoint[k]->GetInfo().fPValue = 0.0f;
					Legs[i][j].pJoint[k]->GetInfo().fIValue = 0.0f;
					Legs[i][j].pJoint[k]->GetInfo().fDValue = 0.0f;
					if (k==1){
						Legs[i][j].pJoint[0]->GetInfo().fPValue = 0.0f;
						Legs[i][j].pJoint[0]->GetInfo().fIValue = 0.0f;
						Legs[i][j].pJoint[0]->GetInfo().fDValue = 0.0f;
					}
				}
			}
		}
	}
}
void Beetle::Step(double dt)
{
	if(!bReady)return;
	switch(nState){
	case BACKWARD : case RIGHTTURN :
		fCounter += (float)(fSpeed * dt);
		if(fCounter > 1.0)fCounter -= 1.0f;
		break;
	case FORWARD: case LEFTTURN:
		fCounter -= fSpeed * (float)dt;
		if(fCounter < 0.0)fCounter += 1.0f;
		break;
	case NEUTRAL:
		fCounter += fSpeed * (float)dt;
		if(fCounter > 1.0){
			fCounter -= 1.0;
			//４回足踏みしたら停止
			if(++nNeutralStep == 4)
				nState = STOP;
		}
		break;
	}
	
	float th = fSwingBackRate / (1 + fSwingBackRate);
	float cnt[2] = {fCounter, fCounter - 0.5f};
	if(cnt[1] < 0.0f)cnt[1] += 1.0f;

	if(nState == FORWARD || nState == BACKWARD){
		Vec3f pos[2];
		for(int i = 0; i < 2; i++){
			//往路
			if(cnt[i] < th)
				pos[i] = Vec3f(0.0f, 0.0f, fStride * (0.5f - cnt[i] / th));
			//復路
			else{
				float tmp = ((cnt[i] - th) / (1 - th) - 0.5f);
				pos[i] = Vec3f(0, fClearance * cos(tmp * M_PI), fStride * tmp);
			}
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[0]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[1]);
		Legs[LEFT][BACK]	.SetPosition(pos[0]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[1]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[0]);
		Legs[RIGHT][BACK]	.SetPosition(pos[1]);
	}
	else if(nState == LEFTTURN || nState == RIGHTTURN){
		Vec3f pos[2][3];
		float angle, dir, y;
		float L[3] = {Legs[LEFT][FRONT].fDistance, Legs[LEFT][MIDDLE].fDistance, Legs[LEFT][BACK].fDistance};

		for(int i = 0; i < 2; i++){
			if(cnt[i] < th){
				angle = fTurnAngle * (0.5f - cnt[i] / th);
				y = 0;
			}
			else{
				float tmp = ((cnt[i] - th) / (1 - th) - 0.5f);
				angle = fTurnAngle * tmp;
				y = fClearance * cos(tmp * (float)M_PI);
			}
			if(i == 0){
				dir = Legs[LEFT][FRONT].fDirection + angle;
				pos[LEFT][FRONT] = Vec3f(
					L[FRONT] * cos(dir) - Legs[LEFT][FRONT].v3ToePosition.X(),
					y,
					L[FRONT] * sin(dir) - Legs[LEFT][FRONT].v3ToePosition.Z());
				dir = Legs[RIGHT][MIDDLE].fDirection + angle;
				pos[RIGHT][MIDDLE] = Vec3f(
					L[MIDDLE] * cos(dir) - Legs[RIGHT][MIDDLE].v3ToePosition.X(),
					y,
					L[MIDDLE] * sin(dir) - Legs[RIGHT][MIDDLE].v3ToePosition.Z());
				dir = Legs[LEFT][BACK].fDirection + angle;
				pos[LEFT][BACK] = Vec3f(
					L[BACK] * cos(dir) - Legs[LEFT][BACK].v3ToePosition.X(),
					y,
					L[BACK] * sin(dir) - Legs[LEFT][BACK].v3ToePosition.Z());
			}
			else{
				dir = Legs[RIGHT][FRONT].fDirection + angle;
				pos[RIGHT][FRONT] = Vec3f(
					L[FRONT] * cos(dir) - Legs[RIGHT][FRONT].v3ToePosition.X(),
					y,
					L[FRONT] * sin(dir) - Legs[RIGHT][FRONT].v3ToePosition.Z());
				dir = Legs[LEFT][MIDDLE].fDirection + angle;
				pos[LEFT][MIDDLE] = Vec3f(
					L[MIDDLE] * cos(dir) - Legs[LEFT][MIDDLE].v3ToePosition.X(),
					y,
					L[MIDDLE] * sin(dir) - Legs[LEFT][MIDDLE].v3ToePosition.Z());
				dir = Legs[RIGHT][BACK].fDirection + angle;
				pos[RIGHT][BACK] = Vec3f(
					L[BACK] * cos(dir) - Legs[RIGHT][BACK].v3ToePosition.X(),
					y,
					L[BACK] * sin(dir) - Legs[RIGHT][BACK].v3ToePosition.Z());
			}
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[LEFT][FRONT]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[LEFT][MIDDLE]);
		Legs[LEFT][BACK]	.SetPosition(pos[LEFT][BACK]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[RIGHT][FRONT]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[RIGHT][MIDDLE]);
		Legs[RIGHT][BACK]	.SetPosition(pos[RIGHT][BACK]);
	}
	else if(nState == NEUTRAL){
		Vec3f pos[2];
		for(int i = 0; i < 2; i++){
			//往路
			if(cnt[i] < th)
				pos[i] = Vec3f(0, fClearance * (0.5 - cnt[i] / th), 0);
			//復路
			else{
				float tmp = ((cnt[i] - th) / (1 - th) - 0.5f);
				pos[i] = Vec3f(0, fClearance * tmp, 0);
			}
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[0]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[1]);
		Legs[LEFT][BACK]	.SetPosition(pos[0]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[1]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[0]);
		Legs[RIGHT][BACK]	.SetPosition(pos[1]);
	}
	else{ //nState == STOP
		Vec3f pos;
		Legs[LEFT][FRONT]	.SetPosition(pos);
		Legs[LEFT][MIDDLE]	.SetPosition(pos);
		Legs[LEFT][BACK]	.SetPosition(pos);
		Legs[RIGHT][FRONT]	.SetPosition(pos);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos);
		Legs[RIGHT][BACK]	.SetPosition(pos);
	}
	Legs[LEFT][FRONT]	.Step(dt);
	Legs[LEFT][MIDDLE]	.Step(dt);
	Legs[LEFT][BACK]	.Step(dt);
	Legs[RIGHT][FRONT]	.Step(dt);
	Legs[RIGHT][MIDDLE]	.Step(dt);
	Legs[RIGHT][BACK]	.Step(dt);
}

void Beetle::Leg::Reset()
{
	v3BasePosition = pJoint[YAW]->GetInfo().v3fPositionParent;
	fLength[0] = (pJoint[PITCH]->GetInfo().v3fPositionChild - pJoint[KNEE]->GetInfo().v3fPositionParent).norm();
	fLength[1] = (pJoint[KNEE]->GetInfo().v3fPositionChild - pJoint[HAND]->GetInfo().v3fPositionParent).norm();
	Matrix3f rot = pJoint[YAW]->GetInfo().m3fRotationParent;
	fTilt = atan2(rot.Ez().Z(), rot.Ez().Y());
	fDirection = atan2(v3ToePosition.Z(), v3ToePosition.X());
	fDistance = v3ToePosition.norm();
	pJoint[YAW]->SetInfo(INFO_PVALUE, false, &(pBody->fPValue));
	pJoint[YAW]->SetInfo(INFO_IVALUE, false, &(pBody->fIValue));
	pJoint[YAW]->SetInfo(INFO_DVALUE, false, &(pBody->fDValue));
	pJoint[PITCH]->SetInfo(INFO_PVALUE, false, &(pBody->fPValue));
	pJoint[PITCH]->SetInfo(INFO_IVALUE, false, &(pBody->fIValue));
	pJoint[PITCH]->SetInfo(INFO_DVALUE, false, &(pBody->fDValue));
	pJoint[KNEE]->SetInfo(INFO_PVALUE, false, &(pBody->fPValue));
	pJoint[KNEE]->SetInfo(INFO_IVALUE, false, &(pBody->fIValue));
	pJoint[KNEE]->SetInfo(INFO_DVALUE, false, &(pBody->fDValue));
	//ニュートラル位置へ
	SetPosition(Vec3f());
}

void Beetle::Leg::SetPosition(const Vec3f& pos)
{
	//入力は足先のニュートラル位置からのオフセット（ボディフレーム）
	Vec3f tmp = v3ToePosition + pos - v3BasePosition;	//足の付け根から足先目標位置まで（ボディフレーム）
	tmp.y *= -1;
	//チルト角分回転して脚のローカルフレームへ
	v3DesiredPosition = Matrix3f::Rot(fTilt, 'x') * tmp;
}

void Beetle::Leg::Step(double dt)
{
	Vec3f& d = v3DesiredPosition;
	float tmp = d.X() * d.X() + d.Y() * d.Y() + d.Z() * d.Z();
	float a, b, c;
	a = atan2(d.Y(), sqrt(d.X() * d.X() + d.Z() * d.Z()));
	b = (fLength[0] * fLength[0] - fLength[1] * fLength[1] + tmp) / (2.0f * fLength[0] * sqrt(tmp));
	if(b > 1.0f)b = 1.0f;
	b = abs(acos(b));
	c = (fLength[1] * fLength[1] - fLength[0] * fLength[0] + tmp) / (2.0f * fLength[1] * sqrt(tmp));
	if(c > 1.0f)c = 1.0f;
	c = abs(acos(c));
	pJoint[YAW]	 ->SetPosition(atan2(d.X(), d.Z()));
	pJoint[PITCH]->SetPosition(-a - b);
	pJoint[KNEE] ->SetPosition(b + c);

	/*
	//一時変数
	float q[3] = {pJoint[YAW]->GetInfo().fPosition, pJoint[PITCH]->GetInfo().fPosition, pJoint[KNEE]->GetInfo().fPosition};
	float c[3] = {cos(q[0]), cos(q[1]), cos(q[1] + q[2])};
	float s[3] = {sin(q[0]), sin(q[1]), sin(q[1] + q[2])};
	float r = fLength[0] * c[1] + fLength[1] * c[2];
	float y = -fLength[0] * s[1] - fLength[1] * s[2];
	//足先位置
	Vec3f pos(r * s[0], y, r * c[0]);
	//制御誤差とその積分と微分
	Vec3f e = v3DesiredPosition - pos;
	ErrorDiff = (e - Error) / dt;
	ErrorInt += e;
	Error = e;
	//ヤコビアン
	Matrix3f J(
		 r * c[0], y * s[0], -fLength[1] * s[0] * s[2],
				0,       -r, -fLength[1] * c[2],
		-r * s[0], y * c[0], -fLength[1] * c[0] * s[2]);
		
	//先端で出すべき力
	Vec3f f = pBody->fPValue * Error + pBody->fIValue * ErrorInt + pBody->fDValue * ErrorDiff;
	//Vec3f f(0.01, 0.0, 0.0);
	//関節トルク
	Vec3f t = J.trans() * f;
	pJoint[YAW]		->SetTorque(t[0]);
	pJoint[PITCH]	->SetTorque(t[1]);
	pJoint[KNEE]	->SetTorque(t[2]);
	*/
}