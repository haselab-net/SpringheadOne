#include "CRBeetle.h"
#include <assert.h>
#include <Physics/PHContactEngine.h>
#include <Physics/PHJoint.h>

namespace Spr{;

SGOBJECTIMP(CRBeetle, SGBehaviorEngine);

CRBeetle::CRBeetle(void)
{
	nState = FORWARD;
	//fCounterL = 1.0;
	//fCounterR = 0.5;
	fCounter = 0.0;
	
	Legs[LEFT][FRONT].pBody = Legs[LEFT][MIDDLE].pBody = Legs[LEFT][BACK].pBody =
	Legs[RIGHT][FRONT].pBody = Legs[RIGHT][MIDDLE].pBody = Legs[RIGHT][BACK].pBody = this;
	
	bReady = false;
	bUseAi = false;
}

CRBeetle::~CRBeetle(void){
}
void CRBeetle::Loaded(SGScene* scene){
	bReady = false;
#define CHECK_DCAST(x) if(!(x)){ exit(0); return; }
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegLFY")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[PITCH]		= DCAST(PHJoint1D, scene->FindObject("jntLegLFP")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[KNEE]		= DCAST(PHJoint1D, scene->FindObject("jntLegLFK")));
	CHECK_DCAST(Legs[LEFT][FRONT].pJoint[HAND]		= DCAST(PHJoint1D, scene->FindObject("jntLegLFH")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegLMY")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[PITCH]	= DCAST(PHJoint1D, scene->FindObject("jntLegLMP")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[KNEE]		= DCAST(PHJoint1D, scene->FindObject("jntLegLMK")));
	CHECK_DCAST(Legs[LEFT][MIDDLE].pJoint[HAND]		= DCAST(PHJoint1D, scene->FindObject("jntLegLMH")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegLBY")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[PITCH]		= DCAST(PHJoint1D, scene->FindObject("jntLegLBP")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[KNEE]		= DCAST(PHJoint1D, scene->FindObject("jntLegLBK")));
	CHECK_DCAST(Legs[LEFT][BACK].pJoint[HAND]		= DCAST(PHJoint1D, scene->FindObject("jntLegLBH")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegRFY")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[PITCH]	= DCAST(PHJoint1D, scene->FindObject("jntLegRFP")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[KNEE]		= DCAST(PHJoint1D, scene->FindObject("jntLegRFK")));
	CHECK_DCAST(Legs[RIGHT][FRONT].pJoint[HAND]		= DCAST(PHJoint1D, scene->FindObject("jntLegRFH")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegRMY")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[PITCH]	= DCAST(PHJoint1D, scene->FindObject("jntLegRMP")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[KNEE]	= DCAST(PHJoint1D, scene->FindObject("jntLegRMK")));
	CHECK_DCAST(Legs[RIGHT][MIDDLE].pJoint[HAND]	= DCAST(PHJoint1D, scene->FindObject("jntLegRMH")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[YAW]		= DCAST(PHJoint1D, scene->FindObject("jntLegRBY")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[PITCH]		= DCAST(PHJoint1D, scene->FindObject("jntLegRBP")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[KNEE]		= DCAST(PHJoint1D, scene->FindObject("jntLegRBK")));
	CHECK_DCAST(Legs[RIGHT][BACK].pJoint[HAND]		= DCAST(PHJoint1D, scene->FindObject("jntLegRBH")));
	CHECK_DCAST(pjntNeck[YAW]						= DCAST(PHJoint1D, scene->FindObject("jntNeckY")));
	CHECK_DCAST(pjntNeck[PITCH]						= DCAST(PHJoint1D, scene->FindObject("jntNeckP")));
	CHECK_DCAST(pjntTail							= DCAST(PHJoint1D, scene->FindObject("jntTail")));
	for(int i=0; i<2; ++i){
		for(int j=0; j<3; ++j){
			for(int k=0; k<4; ++k){
				Legs[i][j].pidJoint[k] = Spr::PHJointPid::Find(Legs[i][j].pJoint[k], scene);
			}
		}
	}
	pidNeck[0] = PHJointPid::Find(pjntNeck[0], scene);
	pidNeck[1] = PHJointPid::Find(pjntNeck[1], scene);
	pidTail = PHJointPid::Find(pjntTail, scene);

#undef CHECK_DCAST
	//関節のPIDパラメータは使ってないのでそこを間借り
/*	fPValue			= Legs[LEFT][FRONT].pidJoint[YAW]->proportional;
	fIValue			= Legs[LEFT][FRONT].pidJoint[YAW]->integral;
	fDValue			= Legs[LEFT][FRONT].pidJoint[YAW]->differential;
	fSpeed			= Legs[LEFT][FRONT].pidJoint[PITCH]->proportional;
	fStride			= Legs[LEFT][FRONT].pidJoint[PITCH]->integral;
	fClearance		= Legs[LEFT][FRONT].pidJoint[PITCH]->differential;
	fSwingBackRate	= Legs[LEFT][FRONT].pidJoint[KNEE]->proportional;
	fTurnAngle		= Legs[LEFT][FRONT].pidJoint[KNEE]->integral;
	toe[FRONT]	= Vec3f(Legs[LEFT][MIDDLE].pidJoint[YAW]  ->proportional, Legs[LEFT][MIDDLE].pidJoint[YAW]	->integral, Legs[LEFT][MIDDLE].pidJoint[YAW]  ->differential);
	toe[MIDDLE] = Vec3f(Legs[LEFT][MIDDLE].pidJoint[PITCH]->proportional, Legs[LEFT][MIDDLE].pidJoint[PITCH]->integral, Legs[LEFT][MIDDLE].pidJoint[PITCH]->differential);
	toe[BACK]	= Vec3f(Legs[LEFT][MIDDLE].pidJoint[KNEE] ->proportional, Legs[LEFT][MIDDLE].pidJoint[KNEE] ->integral, Legs[LEFT][MIDDLE].pidJoint[KNEE] ->differential);
*/
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
	scene->GetBehaviors().Find(contactEngine);

	//	AIのためのSolidの接続
	beetle = NULL;
	attractors.clear();
	fpRecords.clear();
	PHSolid* solid=NULL;
	char* names[] = {"soBlock1", "soBlock2", "soBlock3"};
	for(int i=0; i<sizeof(names)/sizeof(names[0]); ++i){
		PHSolid* solid = DCAST(PHSolid, scene->FindObject(names[i]));
		if (solid){
			attractors.push_back(solid);
		}else{
			DSTR << "Can not connect to " << names[i] << std::endl;
		}
	}
	beetle = DCAST(PHSolid, scene->FindObject("soBody"));
	
	bReady = true;
}

void CRBeetle::SetState(CRBeetleState stat)
{
	nState = stat;
	if(stat == NEUTRAL)
		nNeutralStep = 0;
}
std::vector< UTRef<PHSolid> > bullets;
void CRBeetle::CheckForce(){
	if(!bReady)return;
	for(int i=0; i<2; ++i){
		for(int j=0; j<3; ++j){
			for(int k=1; k<4; ++k){
				PHJoint1D* joint = Legs[i][j].pJoint[k];
				Vec3f f;
				for(unsigned i=0; i<bullets.size(); ++i){
					if (bullets[i]){
						PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(joint->solid->GetFrame(), bullets[i]->GetFrame());
						f += fpr->reflexForce;
					}
				}
				if (f.norm() > 40.0f){
					Legs[i][j].pidJoint[k]->proportional = 0.0f;
					Legs[i][j].pidJoint[k]->integral = 0.0f;
					Legs[i][j].pidJoint[k]->differential = 0.0f;
					if (k==1){
						Legs[i][j].pidJoint[0]->proportional = 0.0f;
						Legs[i][j].pidJoint[0]->integral = 0.0f;
						Legs[i][j].pidJoint[0]->differential = 0.0f;
					}
				}
			}
		}
	}
}
void CRBeetle::AiStep(){
	//	一番近くのアトラクターを探す。
	float minDist = 1e10f;
	PHSolid* nearest=NULL;
	for(unsigned i=0; i<attractors.size(); ++i){
		float dist = (float)(beetle->GetFramePosition() - attractors[i]->GetFramePosition()).norm();
		if (dist < minDist){
			minDist = dist;
			nearest = attractors[i];
		}
	}
	//	一番近くのアトラクターに向かう。
	CRBeetleState aiInst = STOP;
	CRBeetleState walk = STOP;
	static bool bMove;
	if (bMove && rand() < 0.001*RAND_MAX){
		bMove = !bMove;
	}
	if (!bMove && rand() < 0.005*RAND_MAX){
		bMove = !bMove;
	}
	if (bMove && nearest){
		Affinef afBeetle = beetle->GetFrame()->GetPosture();
		Affinef afGoal = afBeetle.inv() * nearest->GetFrame()->GetPosture();
		Vec3f v3Goal = afGoal.Pos();
		if (v3Goal.norm() > 1.0f){
			float angGoal = atan2(v3Goal.X(), v3Goal.Z());
			if (abs(angGoal) > Rad(180-10)){
				walk = FORWARD;
			}else{
				if (angGoal > 0){
					walk = RIGHTTURN;
				}else{
					walk = LEFTTURN;
				}
			}
		}
	}
	//	触られているときの処理（最優先）
	for(unsigned i=0; i<fpRecords.size(); ++i){
		if (fpRecords[i]->reflexForce.norm() > 1e-5){
			if (i<2) walk = BACKWARD;
			else walk = FORWARD;
			fpRecords[i]->Clear();
		}
	}
	//	指令
	if (walk){
		aiInst = walk;
	}else{	//	ないときはランダム
		if (rand() < 0.2*RAND_MAX){
			int r = rand();
			if (r < 0.2*RAND_MAX){
				aiInst = LEFTTURN;
			}else if (r < 0.4*RAND_MAX){
				aiInst = RIGHTTURN;
			}else if (r < 0.6*RAND_MAX){
				aiInst = FORWARD;
			}else{
				aiInst = BACKWARD;
			}
		}else{
			aiInst = STOP;
		}
	}
	SetState(aiInst);
}
void CRBeetle::Step(SGScene* s){
	if(!bReady) return;
	if(bUseAi){
		static int count;
		count ++;
		if (count > 40){
			count = 0;
			AiStep();
		}
	}

	double dt = s->GetTimeStep();
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
	float cnt[2][3];
	cnt[0][0] = fCounter;
	cnt[1][0] = fCounter - 0.5f;
	for(int i=0; i<2; ++i){
		for(int j=1; j<3; ++j){
			cnt[i][j] = cnt[i][0] + j*0.15f;
		}
	}
	for(int i=0; i<2; ++i){
		for(int j=0; j<3; ++j){
			if(cnt[i][j] < 0.0f) cnt[i][j] += 1.0f;
			if(cnt[i][j] > 1.0f) cnt[i][j] -= 1.0f;
		}
	}

	if(nState == FORWARD || nState == BACKWARD){
		Vec3f pos[2][3];
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 3; j++){
				//往路
				if(cnt[i][j] < th)
					pos[i][j] = Vec3f(0.0f, 0.0f, fStride * (0.5f - cnt[i][j] / th));
				//復路
				else{
					float tmp = ((cnt[i][j] - th) / (1 - th) - 0.5f);
					pos[i][j] = Vec3f(0, fClearance * cos(tmp * M_PI), fStride * tmp);
				}
			}
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[0][0]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[1][1]);
		Legs[LEFT][BACK]	.SetPosition(pos[0][2]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[1][0]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[0][1]);
		Legs[RIGHT][BACK]	.SetPosition(pos[1][2]);
	}
	else if(nState == LEFTTURN || nState == RIGHTTURN){
		Vec3f pos[2][3];
		float angle, dir, y;
		float L[3] = {Legs[LEFT][FRONT].fDistance, Legs[LEFT][MIDDLE].fDistance, Legs[LEFT][BACK].fDistance};

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 3; j++){
				if(cnt[i][j] < th){
					angle = fTurnAngle * (0.5f - cnt[i][j] / th);
					y = 0;
				}
				else{
					float tmp = ((cnt[i][j] - th) / (1 - th) - 0.5f);
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
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[LEFT][FRONT]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[LEFT][MIDDLE]);
		Legs[LEFT][BACK]	.SetPosition(pos[LEFT][BACK]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[RIGHT][FRONT]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[RIGHT][MIDDLE]);
		Legs[RIGHT][BACK]	.SetPosition(pos[RIGHT][BACK]);
	}
	else if(nState == NEUTRAL){
		Vec3f pos[2][3];
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 3; j++){
				//往路
				if(cnt[i][j] < th)
					pos[i][j] = Vec3f(0, fClearance * (0.5 - cnt[i][j] / th), 0);
				//復路
				else{
					float tmp = ((cnt[i][j] - th) / (1 - th) - 0.5f);
					pos[i][j] = Vec3f(0, fClearance * tmp, 0);
				}
			}
		}
		Legs[LEFT][FRONT]	.SetPosition(pos[0][0]);
		Legs[LEFT][MIDDLE]	.SetPosition(pos[1][1]);
		Legs[LEFT][BACK]	.SetPosition(pos[0][2]);
		Legs[RIGHT][FRONT]	.SetPosition(pos[1][0]);
		Legs[RIGHT][MIDDLE]	.SetPosition(pos[0][1]);
		Legs[RIGHT][BACK]	.SetPosition(pos[1][2]);
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

void CRBeetle::Leg::Reset()
{
	v3BasePosition = pJoint[YAW]->GetPostureFromParent().Pos();
	fLength[0] = (pJoint[PITCH]->GetPostureFromChild().Pos() - pJoint[KNEE]->GetPostureFromParent().Pos()).norm();
	fLength[1] = (pJoint[KNEE]->GetPostureFromChild().Pos() - pJoint[HAND]->GetPostureFromParent().Pos()).norm();
	Matrix3f rot = pJoint[YAW]->GetPostureFromParent().Rot();
	fTilt = atan2(rot.Ez().Z(), rot.Ez().Y());
	fDirection = atan2(v3ToePosition.Z(), v3ToePosition.X());
	fDistance = v3ToePosition.norm();
	pidJoint[YAW]->proportional = pBody->fPValue;
	pidJoint[YAW]->integral = pBody->fIValue;
	pidJoint[YAW]->differential = pBody->fDValue;
	pidJoint[PITCH]->proportional = pBody->fPValue;
	pidJoint[PITCH]->integral = pBody->fIValue;
	pidJoint[PITCH]->differential = pBody->fDValue;
	pidJoint[KNEE]->proportional = pBody->fPValue;
	pidJoint[KNEE]->integral = pBody->fIValue;
	pidJoint[KNEE]->differential = pBody->fDValue;
	//ニュートラル位置へ
	SetPosition(Vec3f());
	Step(0.001f);
	for(int i=0; i<4; ++i){
		pJoint[i]->position = pidJoint[i]->goal;
	}
}

void CRBeetle::Leg::SetPosition(const Vec3f& pos)
{
	//入力は足先のニュートラル位置からのオフセット（ボディフレーム）
	Vec3f tmp = v3ToePosition + pos - v3BasePosition;	//足の付け根から足先目標位置まで（ボディフレーム）
	tmp.y *= -1;
	//チルト角分回転して脚のローカルフレームへ
	v3DesiredPosition = Matrix3f::Rot(fTilt, 'x') * tmp;
}

void CRBeetle::Leg::Step(double dt)
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
	pidJoint[YAW]  ->goal = atan2(d.X(), d.Z());
	pidJoint[PITCH]->goal = -a - b;
	pidJoint[KNEE] ->goal = b + c;
}


}	//	namespace Spr