#include "Creature.h"
#pragma hdrstop
// CRPuppet.cpp: CRPuppet クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRPuppet.h"
#include <time.h>

#include <Creature/CRAttention.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

SGOBJECTIMP(CRPuppet, CRBallHuman);

//////////////////// PuppetSpringクラス ////////////////////
inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

Vec3f CRPuppet::PositionSpring::GetPos(){
	if (solid) return solid->GetFrame()->GetPosture() * pos;
	return Vec3f();
}

Vec3f CRPuppet::PositionSpring::GetVel(){
	if (solid){
		Vec3f gPos = solid->GetOrientation() * pos;
		Vec3f vel = solid->GetVelocity() + (solid->GetAngularVelocity() ^ gPos);
		return vel;
	};
	return Vec3f();
}

Vec3f CRPuppet::PositionSpring::GetForce(){
	return force;
}

void CRPuppet::PositionSpring::SetSolid(PHSolid* s, Vec3f p, float spr, float dmp){
	solid   = s;
	pos     = p;
	sprRate = spr;
	dmpRate = dmp;
	//DSTR << (solid ? solid->GetName() : "NULL") << " is pulled." << std::endl;
}

void CRPuppet::PositionSpring::SetTarget(Vec3f pos, Vec3f vel, bool b){
	targetPos = pos;
	targetVel = vel;
	bForce   = b;
}

void CRPuppet::PositionSpring::AddForce(Vec3f f){
	force = f;
	if (solid) solid->AddForce(f, GetPos());
}

void CRPuppet::PositionSpring::AddSpringForce(float dt){
/*	const float SPRING = 0.04f * sprRate;		//	バネ
	const float DAMPER = 0.9f  * dmpRate;		//	ダンパ*/
	const float SPRING = 0.06f * sprRate;		//	バネ
	const float DAMPER = 0.15f  * dmpRate;		//	ダンパ
	dt = 0.006f;

	if(solid){
//		DSTR << solid->GetName() << " <- AddSpringForce" << std::endl;
		float mass = solid->GetMass();
		float spring = SPRING * mass * 2 / (dt*dt);
		float damper = DAMPER * mass / (dt);

		Vec3f pos2 = GetPos(); // 現在のPos,Vel
		Vec3f vel2 = GetVel();

		if(IsValid(targetPos) && bForce){
			Vec3f dPos = targetPos - pos2;
			Vec3f dVel = targetVel - vel2;
			if (dVel.norm() > 1e5f) dVel = dVel.unit() * 1e5f;
			if (dPos.norm() > 1e2f) dPos = dPos.unit() * 1e2f;
			force = spring * dPos + damper * dVel;

			/*
			if (force.norm() / mass > 1e3f){
				force = force.unit() * mass * 1e3f;
				DSTR << "Limit Spring Force." << std::endl;
			}
			*/

			AddForce(force);
		}
	}
	/*
	if (strcmp(solid->GetName(), "soHead")==0)
		DSTR << "AddSpring" << solid << solid->GetForce() << std::endl;
		*/
	bForce = false;
}

//////////////////// PostureSpringsクラス ////////////////////
void CRPuppet::PositionSprings::Draw(GRRender* render){
	render->SetModelMatrix(Affinef());
	render->SetLineWidth(15);
	for(iterator it = begin(); it != end(); ++it){
		if(it->targetPos.norm() != 0){
			render->SetMaterial(GRMaterialData(Vec4f(0,1,1,1)));
			Vec3f pos = it->targetPos;
			render->DrawDirect(GRRender::POINTS, &pos, &pos+1);
		}
	}
}

//////////////////// PostureSpringクラス ////////////////////
Quaterniond CRPuppet::PostureSpring::GetQuaternion(){
	if (solid) return solid->GetOrientation();
	return Quaterniond();
}

Vec3f CRPuppet::PostureSpring::GetAngularVelocity(){
	if (solid) return solid->GetAngularVelocity();
	return Vec3f();
}

Vec3f CRPuppet::PostureSpring::GetTorque(){
	return torque;
}

void CRPuppet::PostureSpring::SetSolid(PHSolid* s, float spr, float dmp){
	solid   = s;
	sprRate = spr;
	dmpRate = dmp;
}

void CRPuppet::PostureSpring::SetTarget(Quaterniond quat, Vec3f angVel, bool b){
	targetQuat = quat;
	targetAngV = angVel;
	bTorque   = b;
}

void CRPuppet::PostureSpring::AddTorque(Vec3f t){
	torque = t;
	if (solid) solid->AddTorque(t);
}

void CRPuppet::PostureSpring::AddSpringTorque(float dt){
/*	const float SPRING = 0.008f * sprRate;		//	バネ
	const float DAMPER = 1.2f * dmpRate;		//	ダンパ*/
	const float SPRING = 2.0f * sprRate;		//	バネ
	const float DAMPER = 4.0f * dmpRate;		//	ダンパ
	dt = 0.006f;

	Matrix3d inertia = solid->GetInertia();
	Matrix3d spring = (SPRING * 2 / (dt*dt)) * inertia;
	Matrix3d damper = (DAMPER / dt) * inertia;
//	float det = solid->GetInertia().det();
//	float spring = (SPRING * 2 / (dt*dt)) * det;
//	float damper = (DAMPER / dt) * det;

	Quaterniond quat = GetQuaternion(); // 現在のQuat,AngV
	Vec3f angV = GetAngularVelocity();

	if(bTorque){
		Quaterniond dQuat = targetQuat * quat.inv();
		if (dQuat.W() < 0){	// dQuatの正規化
			dQuat *= -1;
		}
		Vec3f dAngV = targetAngV - angV;
		torque = spring * dQuat.rotation() + damper * dAngV;
		if(torque.norm() > 1e5f) torque = torque.unit() * 1e5f;
		AddTorque(torque);
	}
}

//////////////////// ReachingMovementクラス ////////////////////
CRPuppet::ReachingMovement::ReachingMovement(){
	Init();
}

void CRPuppet::ReachingMovement::Init(){
	bForce = false;
	bActive = false;
	bInitIfContact = false;
	state = 0;
	time = 0.0f;
	limitForce = 350.0f;
	SetTarget(Vec3f(), Vec3f(), false);
}

void CRPuppet::ReachingMovement::SetInitIfContact(){
	bInitIfContact = true;
}

void CRPuppet::ReachingMovement::SetLimitForce(float limit){
	limitForce = limit;
}

void CRPuppet::ReachingMovement::SetSpring(PHSolid* s, Vec3f r){
	solid = s;
	pos   = r;
	sprRate = 2.0f;
	dmpRate = 1.0f;

	//DSTR << (solid ? solid->GetName() : "NULL") << " is pulled by SetSpring." << std::endl;

	firstPos = s->GetFrame()->GetPosture() * r;
}

void CRPuppet::ReachingMovement::SetTimer(float t, float o){
	period = time     = t;
	offset   = o;
	bActive  = true;
}

void CRPuppet::ReachingMovement::SetTargetPos(Vec3f p, Vec3f v){
	finalPos = p;
	finalVel = v;
}

Vec3f CRPuppet::ReachingMovement::GetTargetPos(){
	return finalPos;
}

void CRPuppet::ReachingMovement::SetType(int type){
	state = type;
}

void CRPuppet::ReachingMovement::Draw(GRRender* render){
	if(bActive){
		render->SetModelMatrix(Affinef());
		render->SetLineWidth(10);
		Vec3f pos[2];
		pos[0] = finalPos;
		pos[1] = GetPos();

		render->SetLineWidth(15);
		render->SetMaterial(GRMaterialData(Vec4f(1,0,0,1)));
		render->DrawDirect(GRRender::POINTS, pos, pos+1);

		render->SetLineWidth(10);
		render->SetMaterial(GRMaterialData(Vec4f(0,1,0,1)));
		render->DrawDirect(GRRender::POINTS, pos+1, pos+2);
		render->DrawDirect(GRRender::LINES,  pos, pos+2);
	}
}

void CRPuppet::ReachingMovement::Step(SGScene* scene){
	float dt = scene->GetTimeStep();
	if(bActive){
		//DSTR << "ReachingMovement::Step: " << solid->GetForce().norm() << std::endl;
		if(bInitIfContact && (solid->GetForce().norm() > 30.0f)){	
			Init();
		}

		if(time <= -offset){
			Init();
			return;
		}

		Vec3f pos, vel;
		//	正規化した時刻 (0..1)
		float s = 1- time/period;
		//	紐の長さと速度
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0){
			length = 0;
			deltaLength = 0;
		}
		Vec3f dir = GetPos()-finalPos;
		pos = finalPos + dir*length;
		vel = dir*deltaLength;
		SetTarget(pos, vel, true);

		time -= dt;
		AddSpringForce(dt);
		//DSTR << "time : " << time << std::endl;
		//DSTR << "frce : " << force.norm() << std::endl;
		if (force.norm() >= limitForce){
			//DSTR << "Force " << force.norm() << " is too strong." << std::endl;
			Init();
		}
		//if (force.norm() >= 300.0f) Init();
	}
}

//////////////////// LocusGeneratorクラス ////////////////////
void CRPuppet::LocusGenerator::SaveLog(CRPuppet* puppet){

	for(int i = LogNum-1; i > 0; --i) logPos[0][i] = logPos[0][i-1];
	logPos[0][0] = puppet->solids[6]->GetCenterPosition();
	//logPos[0][0] = puppet->positionSprings[1].targetPos;

	for(int i = LogNum-1; i > 0; --i) logPos[1][i] = logPos[1][i-1];
	logPos[1][0] = puppet->solids[9]->GetCenterPosition();

/*	for(int i = LogNum-1; i > 0; --i) logPos[2][i] = logPos[2][i-1];
	logPos[2][0] = puppet->solids[2]->GetCenterPosition();

	for(int i = LogNum-1; i > 0; --i) logPos[3][i] = logPos[3][i-1];
	logPos[3][0] = puppet->solids[3]->GetCenterPosition();*/
}

inline float SumX(int n, int p, float* y){
	float sum = 0;
	float w = 1;
	for(int i = 0; i < n; ++i){
		w = 2 * n - i;
		//w = pow(i + 1, -2);
		if(y) sum += w * pow(i, p) * y[i];
		else  sum += w * pow(i, p);
	}
	return sum;
}

void CRPuppet::LocusGenerator::WeightedLeastSquares(float* y, int n, int m, PTM::VVector<float>* a){
	// 重み付き最小二乗法
	// n 点(y[i])から m 次近似 (x[i] は等間隔, n > m)
	PTM::VMatrixRow<float> sum_x;
	PTM::VVector<float>    sum_y;
	sum_x.resize(m+1,m+1);
	sum_y.resize(m+1);
	sum_y.clear(0);
	sum_x.clear(0);

	for(int i = 0; i <= 2 * m; ++i){
		float sumX;
		if(i % 2 == 0) sum_y[i/2] = SumX(n, m - i/2, y);
		sumX = SumX(n, 2 * m - i, NULL);
		if(i <= m) for(int j = 0; j <= i; ++j)     sum_x[j][i-j] = sumX;
		else       for(int j = i - m; j <= m; ++j) sum_x[j][i-j] = sumX;
	}
	*a = sum_x.inv() * sum_y;
}

void CRPuppet::LocusGenerator::CalFutureLocus(Vec3f* past, int n, int m, Vec3f* a){
	// 過去ｎ点から最小二乗法でｍ次近似して今後の軌跡を求める
	Vec3f p = Vec3f();
	float *y;
	PTM::VVector<float> ai;
	ai.resize(m+1);
	y  = (float *) calloc(n, sizeof(float));

	for(int i = 0; i < n; ++i) y[i] = past[i].X();
	WeightedLeastSquares(y, n, m, &ai);
	for(int i = 0; i <= m; ++i) a[i].X() = ai[i];

	for(int i = 0; i < n; ++i) y[i] = past[i].Y();
	WeightedLeastSquares(y, n, m, &ai);
	for(int i = 0; i <= m; ++i) a[i].Y() = ai[i];

	for(int i = 0; i < n; ++i) y[i] = past[i].Z();
	WeightedLeastSquares(y, n, m, &ai);
	for(int i = 0; i <= m; ++i) a[i].Z() = ai[i];

	free(y);
}

Vec3f CRPuppet::LocusGenerator::CalcFuturePosition(Vec3f* a, int t){
	Vec3f pos = Vec3f();
	for(int i = 0; i <= Deg; ++i) pos += a[i] * pow(-t, Deg-i);
	return pos;
}

Vec3f CRPuppet::LocusGenerator::CalcFutureVelocity(Vec3f*a, int t, float dt){
	// 軌跡をｍ次近似し、ステップ t での速度計算
	Vec3f vel = Vec3f();
	for(int i = 0; i <= Deg; ++i) vel += a[i] * (pow(-(t+0.5), Deg-i) - pow(-(t-0.5), Deg-i));
	return vel / dt;
}

void CRPuppet::LocusGenerator::DrawFutureLocus(Vec3f*a, int t, int k, GRRender* render, Vec4f color){
	// 軌跡をｍ次近似し、t * k スッテプ後の位置を表示
	render->SetModelMatrix(Affinef());
	render->SetLineWidth(5);
	Vec3f pos[2];
	pos[0] = a[Deg];
	for(int i = 1; i <= k; ++i){
		color.w = 1-(float)i/k;
		render->SetMaterial(GRMaterialData(color));
		pos[1] = Vec3f();
		for(int j = 0; j <= Deg; ++j) pos[1] += a[j] * pow(-t*i, Deg-j);
		render->DrawDirect(GRRender::POINTS, pos, pos+1);
		render->DrawDirect(GRRender::LINES, pos, pos+2);
		pos[0] = pos[1];
	}
}

void CRPuppet::LocusGenerator::SetLocusCoefficient(CRPuppet* puppet, SGScene* scene){
	//GRRender* render;
	//scene->GetRenderers().Find(render);

	for(int i = 0; i < 2; ++i){
		CalFutureLocus(logPos[i], LogNum, Deg, ak[i]);
		//DrawFutureLocus(ak[i], 10, 5, render, Vec4f(0,1,1,1));
	}
}

void CRPuppet::LocusGenerator::PrintLocusCoefficient(CRPuppet* puppet){
	for(int i = 0; i <= Deg ; ++i){
		//DSTR << "a[" << i << "]:"<< ak[0][i] << std::endl;
		DSTR << "a[" << i << "]:"<< std::endl;
		DSTR << ak[0][i].X() << "\t" << ak[0][i].Y() << "\t" << ak[0][i].Z() << "\t" << std::endl;
	}
}

void CRPuppet::LocusGenerator::PrintLogPos(CRPuppet* puppet){
	//for(int i = 0; i <= Deg ; ++i){
		//DSTR << "pos:" << logPos[0][0] << std::endl;
		DSTR << logPos[0][0].X() << "\t" << logPos[0][0].Y() << "\t" << logPos[0][0].Z() << "\t" << std::endl;
	//}
}


//////////////////// HumanContactInfoクラス ////////////////////
CRPuppet::HumanContactInfo::HumanContactInfo(){

}

Vec3f CRPuppet::HumanContactInfo::ContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		return fp->intersections.begin()->convexPair->commonPoint;
	}
	return Vec3f();
}

bool CRPuppet::HumanContactInfo::ContactCheckOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
	if(so2 == NULL) return false;
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;

	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp == NULL) fp = ce->GetFramePair(so2->GetFrame(), so1->GetFrame());

	//return (fp->intersections.size() > 0);
	return (fp->lastContactCount == scene->GetCount() - 1);
}

bool CRPuppet::HumanContactInfo::ContactCheckOfSolid(PHSolid* so, CRBallHuman* human, SGScene* scene){
	if(so == NULL) return false;
	Vec3f rforce = Vec3f(0,0,0);
	for(int i = 0; i < human->solids.size(); ++i){
		if(ContactCheckOfSolidPair(so, human->solids[i], scene)) return true;
	}
	return false;
}

Vec3f CRPuppet::HumanContactInfo::GetContactForceOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
	PHContactEngine* pce = NULL;
	PHContactEngine::FramePairRecord* fpr = NULL;
	Vec3f rforce = Vec3f(0,0,0);
	int sign = 1;
	if(so1 == NULL || so2 == NULL) return rforce;

	scene->GetBehaviors().Find(pce);
	fpr = pce->GetFramePairRecord(so1->GetFrame(), so2->GetFrame());
	if(fpr == NULL){
		fpr = pce->GetFramePairRecord(so2->GetFrame(), so1->GetFrame());
		sign = -1;
	}

	rforce  = fpr->GetReflexForce();
	rforce += fpr->GetFrictionForce();

	fpr->Clear();

	return sign * rforce;
}

Vec3f CRPuppet::HumanContactInfo::GetContactForceOfSolid(PHSolid* so, CRBallHuman* human, SGScene* scene){
	Vec3f rforce = Vec3f(0,0,0);
	if(so == NULL) return rforce;
	for(int i = 0; i < human->solids.size(); ++i){
		rforce += GetContactForceOfSolidPair(human->solids[i], so, scene);
	}
	return rforce;
}

void CRPuppet::HumanContactInfo::SetContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene, Vec3f* pos){
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		pos[0] = fp->intersections.begin()->convexPair->commonPoint;		// グローバル座標系
		pos[1] = fp->intersections.begin()->convexPair->closestPoint[0];	// soVH 座標系
		pos[2] = fp->intersections.begin()->convexPair->closestPoint[1];	// soUser 座標系
	}
}

//////////////////// CRPuppetクラス ////////////////////
CRPuppet::CRPuppet(){
}

void CRPuppet::LoadDerivedModel(SGScene* scene){
	if(IsLoaded()){
		SetJointBasicPos();
		SetSprings();
		for(int i = 0; i < 3; ++i) reaching[0][i].Init();
		bDraw   = false;
		bAttack = false;
		bGuard  = false;
		bAttackAttention = false;
		hittingCount = 0;
		atc = 0;
		inbetweenNotHits=0;
		inbetweenHits=0;	

		scene->FindObject(soWaistU, "soWaistU");
	}
}

bool CRPuppet::Connect(UTRef<SGScene> scene){
	solids.clear();
	joints.clear();
	jointPids.clear();
	jointBallPids.clear();

	SetLoaded(true);

	// Solid Connect
	ConnectSolid("soWaist", scene);
	ConnectSolid("soAbdomen", scene);
	ConnectSolid("soChest", scene);
	ConnectSolid("soHead", scene);

	ConnectSolid("soRUArm", scene);
	ConnectSolid("soRFArm", scene);
	ConnectSolid("soRHand", scene);

	ConnectSolid("soLUArm", scene);
	ConnectSolid("soLFArm", scene);
	ConnectSolid("soLHand", scene);
/*
//	ConnectSolid("soRThigh", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRLThigh", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRHeel", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRToe", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLThigh", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLLThigh", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLHeel", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLToe", scene);
	solids.push_back(NULL);
*/
	//Joint Connect
	ConnectJoint("joWaist", scene);
	ConnectJoint("joChest", scene);
	ConnectJoint("joNeck", scene);
	
	ConnectJoint("joRShoulder", scene);
	ConnectJoint("joRElbow", scene);
	ConnectJoint("joRWrist", scene);

	ConnectJoint("joLShoulder", scene);
	ConnectJoint("joLElbow", scene);
	ConnectJoint("joLWrist", scene);
/*
	ConnectJoint("joWaist1", scene);
	ConnectJoint("joWaist2", scene);
//	ConnectJoint("joWaist3", scene);
	joints.push_back(NULL);
//	ConnectJoint("joChest1", scene);
	joints.push_back(NULL);
//	ConnectJoint("joChest2", scene);
	joints.push_back(NULL);
	ConnectJoint("joChest3", scene);
	ConnectJoint("joNeck1", scene);
	ConnectJoint("joNeck2", scene);
//	ConnectJoint("joNeck3", scene);
	joints.push_back(NULL);
	
	ConnectJoint("joRShoulder1", scene);
	ConnectJoint("joRShoulder2", scene);
	ConnectJoint("joRShoulder3", scene);
	ConnectJoint("joRElbow1", scene);
//	ConnectJoint("joRElbow2", scene);
	joints.push_back(NULL);
	ConnectJoint("joRWrist1", scene);
//	ConnectJoint("joRWrist2", scene);
	joints.push_back(NULL);

	ConnectJoint("joLShoulder1", scene);
	ConnectJoint("joLShoulder2", scene);
	ConnectJoint("joLShoulder3", scene);
	ConnectJoint("joLElbow1", scene);
//	ConnectJoint("joLElbow2", scene);
	joints.push_back(NULL);
	ConnectJoint("joLWrist1", scene);
//	ConnectJoint("joLWrist2", scene);
	joints.push_back(NULL);
*/
/*
//	ConnectJoint("joRHip1", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRHip2", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRHip3", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRKnee", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle1", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle2", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle3", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRToe", scene);
	joints.push_back(NULL);

//	ConnectJoint("joLHip1", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLHip2", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLHip3", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLKnee", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle1", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle2", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle3", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLToe", scene);
	joints.push_back(NULL);
*/
	for(int i=0; i<joints.size(); ++i){
		if(joints[i]->GetJointDof() == 1){
			jointPids.push_back(PHJointPid::Find((PHJoint1D*)joints[i], scene));
			jointBallPids.push_back(NULL);
		}
		else if(joints[i]->GetJointDof() == 3){
			jointBallPids.push_back(PHJointBallPid::Find((PHJointBall*)joints[i], scene));
			jointPids.push_back(NULL);
		}
	}
	return IsLoaded();
}

void CRPuppet::SetSolidInfo(){
	// その内ファイルからロードするようにしよう
	// soWaist(腰)
	sinfo[0].scaleRatio = Vec3f(0.190, 0.0768, 0.137) / 2;
	sinfo[0].massRatio  = 0.17f;

	// soAbdomen(腹)
	sinfo[1].scaleRatio = Vec3f(0.151, 0.0201, 0.108) / 2;
	sinfo[1].massRatio  = 0.028f;

	// soChest(胸)
	sinfo[2].scaleRatio = Vec3f(0.184, 0.202, 0.124) / 2;
	sinfo[2].massRatio  = 0.252f;

	// soHead(頭)
	sinfo[3].scaleRatio = Vec3f(0.0949, 0.140, 0.111) / 2;
	sinfo[3].massRatio  = 0.07f;

	// soRUArm(右上腕)
	sinfo[4].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[4].massRatio  = 0.04f;

	// soRFArm(右前腕)
	sinfo[5].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[5].massRatio  = 0.02f;

	// soRHand(右手)
	sinfo[6].scaleRatio = Vec3f(0.0507, 0.055, 0.0479) / 2;
	sinfo[6].massRatio  = 0.01f;

	// soLUArm(左上腕)
	sinfo[7].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[7].massRatio  = 0.04f;

	// soLFArm(左前腕)
	sinfo[8].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[8].massRatio  = 0.02f;

	// soLHand(左手)
	sinfo[9].scaleRatio = Vec3f(0.0507, 0.055, 0.0479) / 2;
	sinfo[9].massRatio  = 0.01f;

	// soRThigh(右大腿)
	sinfo[10].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[10].massRatio  = 0.1f;

	// soRLThigh(右下腿)
	sinfo[11].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[11].massRatio  = 0.05f;

	// soRHeel(右踵)
	sinfo[12].scaleRatio = Vec3f(0.0379, 0.0408, 0.0942) / 2;
	sinfo[12].massRatio  = 0.015f;

	// soRToe(右爪先)
	sinfo[13].scaleRatio = Vec3f(0.0582, 0.0288, 0.0542) / 2;
	sinfo[13].massRatio  = 0.005f;

	// soLThigh(左大腿)
	sinfo[14].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[14].massRatio  = 0.1f;

	// soLLThigh(左下腿)
	sinfo[15].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[15].massRatio  = 0.05f;

	// soLHeel(左踵)
	sinfo[16].scaleRatio = Vec3f(0.0379, 0.0408, 0.0942) / 2;
	sinfo[16].massRatio  = 0.015f;

	// soLToe(左爪先)
	sinfo[17].scaleRatio = Vec3f(0.0582, 0.0288, 0.0542) / 2;
	sinfo[17].massRatio  = 0.005f;

	//for(int i= 0; i < solids.size(); ++i){
	for(int i= 0; i < 18; ++i){
		sinfo[i].scale = sinfo[i].scaleRatio * totalHeight;
	}
}

inline float GetChildMass(PHJointBase* j){
	if (strlen(j->solid->GetName()) != 0){
		return j->solid->GetMass();
	}
	else{
		float rv = 0;
		for(unsigned i = 0; i < j->Children().size(); ++i){
			rv += GetChildMass(j->Children()[i]);
		}
		return rv;
	}
}


void CRPuppet::SetJointSpring(float dt){
/*	//const float SAFETYRATE = 0.002f;
	const float SAFETYRATE = 0.002f;
	float k = 0.1f * SAFETYRATE;
	float b = 0.8f * SAFETYRATE;*/
	//const float SAFETYRATE = 0.01f;	//Hinge Rate
	const float SAFETYRATE = 0.015f;
	float k = 0.6f * SAFETYRATE;
	float b = 0.8f * SAFETYRATE;
	dt = 0.006f;

	for(int i=0; i<joints.size(); ++i){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
		else if(jointBallPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointBallPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointBallPids[i]->differential = b * mass / dt;
			jointBallPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}
	/*
	// 関節を柔らかめに設定(上半身)
	for(int i = 0; i < 6; ++i){
		if(jointPids[i] != NULL){
			if(i % 3 == 2) JointPIDMul(jointPids[i], 0.04f, 0.2f);
			else           JointPIDMul(jointPids[i], 0.3f, 0.8f);
		}
	}
*/
	if(jointBallPids[0] != NULL){
		//JointBallPIDMul(jointBallPids[0], 0.4f, 1.0f);
		JointBallPIDMul(jointBallPids[0], 0.4f*2.0f, 1.0f*4.0f);
	}
	if(jointPids[1] != NULL){
		//JointPIDMul(jointPids[1], 0.4f, 0.8f);
		JointPIDMul(jointPids[1], 0.4f*2.0f, 0.8f*4.0f);
	}
	// 関節を柔らかめに設定(首)
	if(jointBallPids[2] != NULL){
		JointBallPIDMul(jointBallPids[2], 0.3f, 0.8f);
	}
	// 関節を柔らかめに設定(右腕)
	if(jointBallPids[3] != NULL){
		//JointBallPIDMul(jointBallPids[3], 0.08f, 1.0f);
		JointBallPIDMul(jointBallPids[3], 0.08f*1.5f, 1.0f*10.0f);
	}
	if(jointPids[4] != NULL){
		//JointPIDMul(jointPids[4], 0.06f, 0.1f);
		JointPIDMul(jointPids[4], 0.06f*3.0f, 0.1f*40.0f);
	}

	// 関節を柔らかめに設定(左腕)
	if(jointBallPids[6] != NULL){
		//JointBallPIDMul(jointBallPids[6], 0.08f, 1.0f);
		JointBallPIDMul(jointBallPids[6], 0.08f*1.5f, 1.0f*10.0f);
	}
	if(jointPids[7] != NULL){
		//JointPIDMul(jointPids[7], 0.06f, 0.1f);
		JointPIDMul(jointPids[7], 0.06f*3.0f, 0.1f*40.0f);
	}
}

void CRPuppet::SetJointBasicPos(){
	/*
	if(jointPids[0])  jointPids[0]->goal  = jinfo[0].initPos  = 0.2f;
//	if(jointPids[5])  jointPids[5]->goal  = jinfo[5].initPos  = -0.2f;
	jinfo[5].rangeMin	= -49.00f;
	jinfo[5].rangeMax	= 49.00f;
	if(jointPids[9])  jointPids[9]->goal  = jinfo[9].initPos  = 0.8f;	//X
	if(jointPids[10]) jointPids[10]->goal = jinfo[10].initPos = 0.0f;	//Z
	if(jointPids[11]) jointPids[11]->goal = jinfo[11].initPos = 0.5f;	//Y
	if(jointPids[12]) jointPids[12]->goal = jinfo[12].initPos = 2.5f;

	if(jointPids[16]) jointPids[16]->goal = jinfo[16].initPos = 1.2f;	//X
	if(jointPids[17]) jointPids[17]->goal = jinfo[17].initPos = 0.0f;	//Z
	if(jointPids[18]) jointPids[18]->goal = jinfo[18].initPos = 0.5f;	//Y
	if(jointPids[19]) jointPids[19]->goal = jinfo[19].initPos = 2.3f;
	*/

	// Waist
	if(jointBallPids[0]) jointBallPids[0]->goal = jinfo[0].initQt = Quaternionf(cosf(-0.1f), sinf(-0.1f), 0.0f, 0.0f);

	// Shoulder(R,L)
	//if(jointBallPids[3]) jointBallPids[3]->goal = jinfo[3].initQt = Quaternionf(cosf(0.4f), sinf(0.4f), 0.0f, 0.0f) * Quaternionf(cosf(-0.25f), 0.0f, 0.0f, sinf(-0.25f));
	//if(jointBallPids[6]) jointBallPids[6]->goal = jinfo[6].initQt = Quaternionf(cosf(0.4f), sinf(0.4f), 0.0f, 0.0f) * Quaternionf(cosf(0.25f), 0.0f, 0.0f, sinf(0.25f));

	//if(jointBallPids[3]) jointBallPids[3]->goal = jinfo[3].initQt = Quaternionf(0.893092f, 0.326261f, 0.108078f, -0.290275f);
	//if(jointBallPids[6]) jointBallPids[6]->goal = jinfo[6].initQt = Quaternionf(0.763403f, 0.622709f, 0.000002f, 0.171611f);
	if(jointBallPids[3]) jointBallPids[3]->goal = jinfo[3].initQt = Quaternionf(0.829798f, 0.532525f, 0.066818f, -0.152931f);
	if(jointBallPids[6]) jointBallPids[6]->goal = jinfo[6].initQt = Quaternionf(0.881232f, 0.403101f, -0.089433f, 0.230092f);

	// Elbow(R,L)
	//if(jointPids[4]) jointPids[4]->goal = jinfo[4].initPos = 2.3f;
	//if(jointPids[7]) jointPids[7]->goal = jinfo[7].initPos = 2.3f;

	//if(jointPids[4]) jointPids[4]->goal = jinfo[4].initPos = 2.11f;
	//if(jointPids[7]) jointPids[7]->goal = jinfo[7].initPos = 1.73f;
	if(jointPids[4]) jointPids[4]->goal = jinfo[4].initPos = 1.828327f * 1.2f;
	if(jointPids[7]) jointPids[7]->goal = jinfo[7].initPos = 2.020205f * 1.2f;

}

void CRPuppet::Draw(GRRender* render){
	positionSprings.Draw(render);
	reaching[0][0].Draw(render);
	reaching[0][1].Draw(render);
	reaching[0][2].Draw(render);
}

void CRPuppet::SetSprings(){
	if(!IsLoaded()) return;
	positionSprings.clear();

	// [0] 腰(基本立ち位置に固定)
	postureSpring.SetSolid(solids[0], 0.6f, 1.2f);
	PositionSpring positionSpr;
	positionSpr.SetSolid(solids[0], Vec3f(0, 0, 0), 0.2f, 10.0f);
	positionSprings.push_back(positionSpr);

	positionSprings[0].SetTarget(Vec3f(0.0f, 1.0f,-0.95f), Vec3f(), true);
}

void CRPuppet::SetFixedPos(){
	if(!IsLoaded()) return;

	postureSpring.SetTarget(Quaterniond(0,0,1,0), Vec3f(), true);
	//positionSprings[0].SetTarget(Vec3f(0.0f, 1.0f,-0.95f), Vec3f(), true);

	Vec3f goalPos = soWaistU->GetCenterPosition() + Vec3f(0.0f, 0.0f, -0.95f);
	goalPos[1] = 1.0f;

	/*
	Vec3f currPos = solids[0]->GetCenterPosition();
	Vec3f dir = (goalPos - currPos); if (dir.norm()!=0){ dir = dir / dir.norm(); }
	float ds = 0.001f;
	Vec3f pos = positionSprings[0].targetPos + (dir*ds);
	pos[1] = 1.0f;
	*/

	positionSprings[0].SetTarget(goalPos, Vec3f(), true);
}

void CRPuppet::SetExpectedPos(float dt){
	for(int i = 0; i < positionSprings.size(); ++i) 
		positionSprings[i].targetPos += positionSprings[i].targetVel * dt;
}

void CRPuppet::SetSpringForce(float dt){
	SetFixedPos();
	for(int i = 0; i < positionSprings.size(); ++i){
		positionSprings[i].AddSpringForce(dt);
	}
	postureSpring.AddSpringTorque(dt);
}

void CRPuppet::Step(SGScene* scene){
	locus.SaveLog(this);
	SetSpringForce(scene->GetTimeStep());
	for(int i = 0; i < 3; ++i) reaching[0][i].Step(scene);

	//if (strcmp(reaching[0][2].solid->GetName(), "soHead")==0)
	//	DSTR << "AddSpring" << solid->GetForce() << std::endl;

	if(bDraw){ 
		GRRender* render;
		scene->GetRenderers().Find(render);
		Draw(render);
		for(int i = 2; i < 3; ++i) reaching[0][i].Draw(render);
	}
}

inline float random1(){
	return (float)rand()/RAND_MAX;
}

void CRPuppet::Attack(CRPuppet* puppet){
	if(bAttack){
		int wait = 0;
		//for(int i = 0; i < 3; ++i) wait += reaching[0][i].state;
		for(int i = 0; i < 2; ++i) wait += reaching[0][i].state;
		if(wait == 0){
			//if(random1() < 0.002f){
			if(random1() < 0.004f){
				//srand((unsigned) time(NULL));
				int hand = 0;
				if(random1() > 0.7f) hand = 1;
				int body = 0;
				float dist0 = pow((solids[3*hand+6]->GetCenterPosition() - puppet->solids[2]->GetCenterPosition()).norm(), 2);
				float dist1 = pow((solids[3*hand+6]->GetCenterPosition() - puppet->solids[3]->GetCenterPosition()).norm(), 2);
				if(random1() > dist1/(dist0+dist1)) body = 1;
				if(random1() > 0.2f) body = 1;

				reaching[0][hand].SetSpring(solids[3*hand+6], Vec3f(0,0,0));
				Vec3f pos = puppet->solids[body+2]->GetFrame()->GetPosture() 
					* Vec3f((random1()-0.5f) * 1.8f * puppet->GetSolidInfo(body+2).scale.X(), 
							//(random1()-0.5f) * 1.8f * puppet->GetSolidInfo(body+2).scale.Y(),
							((1.5f*random1() - 0.5f*body) - 0.5f) * puppet->GetSolidInfo(body+2).scale.Y(),
							0.03f);
				reaching[0][hand].SetTargetPos(pos, Vec3f());
				reaching[0][hand].SetTimer(0.5f, -0.1f);
				//reaching[0][hand].SetTimer(0.5f, -0.1f);
				reaching[0][hand].SetType(1);
				reaching[0][hand].SetSprRate(2.0f*2.0f);
				reaching[0][hand].SetDmpRate(1.0f*2.0f);
				reaching[0][hand].SetInitIfContact();
				targetPos = pos; bAttackAttention = true;
			}else{
				bAttackAttention = false;
			}
		}
	}
}

void CRPuppet::TopDownAttention(CRAttention* crAttention){
	if (bAttack && bAttackAttention){
		crAttention->SetAttentionPoint(targetPos, 100.0f); // 嘘っぽい数字
	}
}

void CRPuppet::AttackTest(CRPuppet* puppet){
	int hand[20]   = { 0,0,1, 1, 0,1,0, 1,0, 1,1, 0, 0,1, 1, 0, 1,1,0,1};
	int body[20]   = { 1,0,1, 1, 0,0,1, 0,1, 1,0, 0, 1,1, 1, 0, 0,1,0,0};
	int target[20] = {-1,1,1,-1,-1,1,1,-1,1,-1,1,-1,-1,1,-1,-1,-1,1,1,1};
	//                 1 2 3  4  5 6 7  8 9 1011 12 1314 15 16 17181920
	reaching[0][hand[atc]].SetSpring(solids[3*hand[atc]+6], Vec3f(0,0,0));
	Vec3f r   = Vec3f(0.8f * puppet->GetSolidInfo(body[atc]+2).scale.X() * target[atc],
		              0.1f * puppet->GetSolidInfo(body[atc]+2).scale.Y(),
					  0.05f);
	if(body[atc] == 1) r.y *= -1.0f;
	Vec3f pos = puppet->solids[body[atc]+2]->GetFrame()->GetPosture() * r;
	reaching[0][hand[atc]].SetTargetPos(pos, Vec3f());
	reaching[0][hand[atc]].SetTimer(0.2f, 0.1f);
	reaching[0][hand[atc]].SetType(1);
	atc++;
	atc %= 20;
}

inline bool CheckDirection(Vec3f a, Vec3f b, Vec3f v){
	// 点 a から速度 v の物体は 点 b に向かっている？
	bool bTarget = true;
	Vec3f ab = b - a;
	bTarget &= (v.norm() > 2.0f);
	bTarget &= ((ab.norm() * ab.norm()) / (ab * v) < 0.5f);
	bTarget &= (acos(ab.unit() * v.unit()) < M_PI/8);
	return bTarget;
}

bool CRPuppet::IsAimed(CRPuppet* puppet, SGScene* scene){
	// 相手に狙われているかチェック
	bool bStart = false;
	for(int i = 0; i < 2; ++i){
		bool bTrigger = false;
		Vec3f posA = puppet->locus.logPos[i][0];
		Vec3f vel  = puppet->locus.CalcFutureVelocity(puppet->locus.ak[i], 0, scene->GetTimeStep());
		bTrigger |= CheckDirection(posA, GetCOG(), vel);
		bTrigger |= CheckDirection(posA, solids[2]->GetCenterPosition(), vel);
		bTrigger |= CheckDirection(posA, solids[3]->GetCenterPosition(), vel);
		puppet->locus.bPrediction[i] = bTrigger;
		bStart |= bTrigger;
	}
	return bStart;
}

void CRPuppet::GuardTest(CRPuppet* puppet, SGScene* scene){
	if(bGuard){
		puppet->locus.SetLocusCoefficient(puppet, scene);
		if(IsAimed(puppet, scene)){
			for(int i = 0; i < 2; ++i){
				if(puppet->locus.bPrediction[i]){
					Vec3f vel  = puppet->locus.CalcFutureVelocity(puppet->locus.ak[i], 0, scene->GetTimeStep());
					Vec3f dist = GetCOG() - puppet->locus.logPos[i][0];
					float time = dist.norm() / vel.norm();
					
					Vec3f pos = puppet->locus.CalcFuturePosition(puppet->locus.ak[i], (int)(0.4f*time / scene->GetTimeStep()));
					if((pos - solids[2]->GetCenterPosition()).norm() < (pos - solids[3]->GetCenterPosition()).norm()){
						reaching[0][1-i].SetSpring(solids[3*(1-i)+5], Vec3f(0,-0.05f,0));
					}
					else{
						reaching[0][1-i].SetSpring(solids[3*(1-i)+5], Vec3f(0,0.05f,0));
					}
					reaching[0][1-i].SetTargetPos(pos, Vec3f());
					reaching[0][1-i].SetTimer(0.3f*time, 0.1f);
					reaching[0][1-i].SetType(2);

					targetPos = pos; bAttackAttention = true;

					if(bDraw){
						GRRender* render;
						scene->GetRenderers().Find(render);
						render->SetModelMatrix(Affinef());
						render->SetLineWidth(15);
						render->SetMaterial(GRMaterialData(Vec4f(1,0,0,1)));
						render->DrawDirect(GRRender::POINTS, &pos, &pos+1);
						//puppet->locus.DrawFutureLocus(puppet->locus.ak[i], 10, 10, render, Vec4f(0,1,1,1));
					}
				}
			}
		}
	}
}

void CRPuppet::ReachingMovemantsSafetyCheck(CRPuppet* puppet, SGScene* scene){
	for(int i = 0; i < 2; ++i){
		if(reaching[0][i].state != 0){
			Vec3f force;
			force = humanContactInfo.GetContactForceOfSolid(reaching[0][i].solid, puppet, scene);
			if(force.norm() > 100) reaching[0][i].Init();
		}
	}
}
void CRPuppet::resetHits() {
	bHitted=false;
	inbetweenHits = 0;
	inbetweenNotHits = 0;
	hittingCount = 0;
}
	

void CRPuppet::HittedCheck(CRPuppet* puppet, SGScene* scene, bool prediction){
	bool now =false;
	for(int i = 0; i < 2; ++i){
		now |= humanContactInfo.ContactCheckOfSolid(solids[i+2], puppet, scene);
		now = now && prediction;
		//if(humanContactInfo.GetContactForceOfSolid(solids[i+2], puppet, scene).norm() > 50) bHitted = true;
	}
	if (now && (inbetweenNotHits > 5)) {
		hittingCount++;		
		bHitted = true;
		PlayHitSound();
	}else {
		bHitted = false;
	}
	if (now == true) {
		inbetweenHits++;	
		inbetweenNotHits = 0;
	} else {		
		inbetweenHits = 0;	
		inbetweenNotHits++;
	}

}


void CRPuppet::PlayHitSound(){
	GRSound::instance()->play( SAMPLE4 );
}


DEF_RECORD(XPuppet,{
	GUID Guid(){ return WBGuid("1EA16CFD-B12D-479c-8F18-80AFB33012FE"); }
});
class CRPuppetLoader : public FIObjectLoader<CRPuppet>{
public:
	CRPuppetLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XPuppet);
	}
	bool LoadData(FILoadScene* ctx, CRPuppet* h){
		return true;
	}
};
class CRPuppetSaver : public FIObjectSaver<CRPuppet>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRPuppet* h){
	}
};

DEF_REGISTER_BOTH(CRPuppet);


}
