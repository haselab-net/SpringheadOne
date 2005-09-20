// THuman.cpp: THuman クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "THuman.h"
#include <Graphics/GRMaterial.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace Spr;

Vec3f TSpring::GetPos(){
	if (solid) return solid->GetFrame()->GetPosture() * pos;
	return Vec3f();
}
Vec3f TSpring::GetVel(){
	if (solid){
		Vec3f gPos = solid->GetOrientation() * pos;
		Vec3f vel = solid->GetVelocity() + (solid->GetAngularVelocity() ^ gPos);
		return vel;
	};
	return Vec3f();
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

THuman::THuman(){
	soBody  = NULL;
	soHead  = NULL;
	soRUArm = NULL;
	soLUArm = NULL;
	soRFArm = NULL;
	soLFArm = NULL;
	soRHand = NULL;
	soLHand = NULL;
	soWaist = NULL;
	soRULeg = NULL;
	soLULeg = NULL;
	soRLLeg = NULL;
	soLLLeg = NULL;
	soRFoot = NULL;
	soLFoot = NULL;
	
	joWaist1 = NULL;
	joWaist2 = NULL;
	joWaist3 = NULL;
	joNeck1 = NULL;
	joNeck2 = NULL;
	joNeck3 = NULL;
	joRShoulder1 = NULL;
	joRShoulder2 = NULL;
	joRShoulder3 = NULL;
	joRElbow1 = NULL;
	joRWrist1 = NULL;
	joRWrist2 = NULL;
	joLShoulder1 = NULL;
	joLShoulder2 = NULL;
	joLShoulder3 = NULL;
	joLElbow1 = NULL;
	joLWrist1 = NULL;
	joLWrist2 = NULL;
	joRHip1 = NULL;
	joRHip2 = NULL;
	joRHip3 = NULL;
	joRKnee1 = NULL;
	joRAnkle1 = NULL;
	joLHip1 = NULL;
	joLHip2 = NULL;
	joLHip3 = NULL;
	joLKnee1 = NULL;
	joLAnkle1 = NULL;

	joLShin = NULL;
	joRShin = NULL;

	bLoaded = false;
	//checkPart
	checkPart = false;

}

void THuman::ConnectSolid(Spr::PHSolid*& solid, const char* name, Spr::UTRef<Spr::SGScene> scene){
	scene->FindObject(solid, name);
	bLoaded &= (solid != NULL);
	solids.push_back(solid);
}

void THuman::ConnectJoint(Spr::PHJoint*& joint, const char* name, Spr::UTRef<Spr::SGScene> scene){
	scene->FindObject(joint, name);
	bLoaded &= (joint != NULL);
	joints.push_back(joint);
}
bool THuman::Connect(Spr::UTRef<Spr::SGScene> scene){
	solids.clear();
	joints.clear();
	bLoaded = true;
	ConnectSolid(soBody, "soBody", scene);
	ConnectSolid(soHead, "soHead", scene);
	ConnectSolid(soRUArm, "soRUArm", scene);
	ConnectSolid(soLUArm, "soLUArm", scene);
	ConnectSolid(soRFArm, "soRFArm", scene);
	ConnectSolid(soLFArm, "soLFArm", scene);
	ConnectSolid(soRHand, "soRHand", scene);
	ConnectSolid(soLHand, "soLHand", scene);
	ConnectSolid(soWaist, "soWaist", scene);
	ConnectSolid(soRULeg, "soRULeg", scene);
	ConnectSolid(soLULeg, "soLULeg", scene);
	ConnectSolid(soRLLeg, "soRLLeg", scene);
	ConnectSolid(soLLLeg, "soLLLeg", scene);
	ConnectSolid(soRFoot, "soRFoot", scene);
	ConnectSolid(soLFoot, "soLFoot", scene);

	ConnectJoint(joWaist1, "joWaist1", scene);
	ConnectJoint(joWaist2, "joWaist2", scene);
	ConnectJoint(joWaist3, "joWaist3", scene);
	ConnectJoint(joNeck1, "joNeck1", scene);
	ConnectJoint(joNeck2, "joNeck2", scene);
	ConnectJoint(joNeck3, "joNeck3", scene);
	ConnectJoint(joLShoulder1, "joLShoulder1", scene);
	ConnectJoint(joLShoulder2, "joLShoulder2", scene);
	ConnectJoint(joLShoulder3, "joLShoulder3", scene);
	ConnectJoint(joLElbow1, "joLElbow1", scene);
	ConnectJoint(joLWrist1, "joLWrist1", scene);
	ConnectJoint(joLWrist2, "joLWrist2", scene);
	ConnectJoint(joRShoulder1, "joRShoulder1", scene);
	ConnectJoint(joRShoulder2, "joRShoulder2", scene);
	ConnectJoint(joRShoulder3, "joRShoulder3", scene);
	ConnectJoint(joRElbow1, "joRElbow1", scene);
	ConnectJoint(joRWrist1, "joRWrist1", scene);
	ConnectJoint(joRWrist2, "joRWrist2", scene);
	ConnectJoint(joLHip1, "joLHip1", scene);
	ConnectJoint(joLHip2, "joLHip2", scene);
	ConnectJoint(joLHip3, "joLHip3", scene);
	ConnectJoint(joLKnee1, "joLKnee1", scene);
	ConnectJoint(joLAnkle1, "joLAnkle1", scene);
//	ConnectJoint(joLAnkle2, "joLAnkle2", scene);
	ConnectJoint(joRHip1, "joRHip1", scene);
	ConnectJoint(joRHip2, "joRHip2", scene);
	ConnectJoint(joRHip3, "joRHip3", scene);
	ConnectJoint(joRKnee1, "joRKnee1", scene);
	ConnectJoint(joRAnkle1, "joRAnkle1", scene);
//	ConnectJoint(joRAnkle2, "joRAnkle2", scene);
	ConnectJoint(joLShin, "joLShin", scene);
	ConnectJoint(joRShin, "joRShin", scene);
	return bLoaded;
}

THuman::~THuman(){
}
void THuman::SaveOneAngle(THuman::TAngle& angle, Spr::PHJoint* j){
	angle.angle[angle.nAngle] = j->GetPosition();
	angle.angVel[angle.nAngle] = j->GetVelocity();
	angle.nAngle ++;
}
void THuman::SaveJointAngle(){
	TAngle angle;
	angle.bodyOri = soBody->GetOrientation();
	angle.bodyAngVel = soBody->GetAngularVelocity();
	angle.bodyVel = soBody->GetVelocity();
	for(int i=0; i<joints.size(); ++i){
		SaveOneAngle(angle, joints[i]);
	}
	angles.push_back(angle);
}

void THuman::LoadOneAngle(THuman::TAngle& a, int c, Spr::PHJoint* j){
	j->SetPosition(a.angle[c]);
}
void THuman::SaveOneTorque(THuman::TAngle& a, int c, Spr::PHJoint* j){
	a.jointTorque[c] = j->GetTorque();
}
void THuman::SaveForce(float time, float dt){
	int c = time / dt;
	if (c >= angles.size()) return;
	TAngle& angle = angles[c];
	angle.force = Vec3f();
	angle.torque = Vec3f();
	angle.force += soLFoot->GetForce();
//	angle.force += soRFoot->GetForce();
	angle.torque += soLFoot->GetTorque();
//	angle.torque += soRFoot->GetTorque();
	angle.torque += soLFoot->GetCenterPosition() ^ soLFoot->GetForce();
//	angle.torque += soRFoot->GetCenterPosition() ^ soRFoot->GetForce();
	for(int i=0; i<joints.size(); ++i){
		SaveOneTorque(angle, i, joints[i]);
	}
}
bool THuman::LoadJointAngle(float time, float dt){
	int c = time / dt;
	if (c >= angles.size()) return false;
	TAngle& angle = angles[c];
	for(int i=0; i<joints.size(); ++i){
		//checkPart
		if(checkPart == true && (i == 18 /*|| i == 21 || i == 22 || i == 28*/)) {
		}else{
			LoadOneAngle(angle, i, joints[i]);
		}
	}
	
	const float SAFETYRATE = 0.1f;			//	安全率 小さいほど安定でやわらかく
	const float SPRING = 0.2f*SAFETYRATE;		//	バネ
	const float DAMPER = 0.6f*SAFETYRATE;		//	ダンパ
	float mass = soBody->GetMass();
	float spring = SPRING * mass / (2*dt*dt);
	float damper = DAMPER * mass / (dt);
	Quaternionf dOri = angle.bodyOri * soBody->GetOrientation().inv();
	Vec3f dAng = dOri.rotation();
	Vec3f dAVel = angle.bodyAngVel - soBody->GetAngularVelocity();
	soBody->AddTorque(spring* dAng + damper* dAVel);
	Vec3f dVel = angle.bodyVel - soBody->GetVelocity();
	if (dVel.Y() > 0) dVel.Y() = 0;
	if (time < 0.1f) soBody->AddForce(0.5f * damper * dVel);
	
	return true;
}
inline float GetChildMass(PHJoint* j){
	if (strlen(j->solid->GetName()) != 0){
		return j->solid->GetMass();
	}else{
		float rv = 0;
		for(int i=0; i<j->Children().size(); ++i){
			rv += GetChildMass(j->Children()[i]);
		}
		return rv;
	}
}
static void JointPIDMul(PHJoint* jo, float mul){
	jo->GetInfo().fPValue *= mul;
	jo->GetInfo().fIValue *= mul;
	jo->GetInfo().fDValue *= mul;
}
void THuman::SetSpring(float dt){
	const float SAFETYRATE = 0.01f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f * SAFETYRATE;
	for(int i=0; i<joints.size(); ++i){
		float mass = GetChildMass(joints[i]);
		joints[i]->GetInfo().fPValue = k * mass / (2*dt*dt);
		joints[i]->GetInfo().fDValue = b * mass / dt;
		joints[i]->GetInfo().fIValue = k * mass / (2*dt*dt) / 5000.0f;
	}
	/*
	const float AnkleMul = 0.1f;
	JointPIDMul(joLAnkle1, AnkleMul);
	JointPIDMul(joRAnkle1, AnkleMul);
*/
	const float HIPMUL = 2.0f;
	JointPIDMul(joLHip1, HIPMUL);
	JointPIDMul(joLHip2, HIPMUL);
	JointPIDMul(joLHip3, HIPMUL);
	JointPIDMul(joRHip1, HIPMUL);
	JointPIDMul(joRHip2, HIPMUL);
	JointPIDMul(joRHip3, HIPMUL);

	const float WaistMul = 0.3f;
	JointPIDMul(joWaist1, WaistMul);
	JointPIDMul(joWaist2, WaistMul);
	JointPIDMul(joWaist3, WaistMul);
	
	const float ShinMul = 10.0f;
	JointPIDMul(joLShin, ShinMul);
	JointPIDMul(joRShin, ShinMul);
}
void THuman::SetTotalMass(float tm){
	totalMass = tm;
	for(int i=0; i<solids.size(); ++i){
		solids[i]->SetMass(solids[i]->GetMass()*totalMass);
		solids[i]->SetInertia(solids[i]->GetInertia()*totalMass);
	}
}
void THuman::SetOneInertia(Spr::PHSolid* solid, float longAxis, float rate){
	float shortAxis = longAxis*rate;
	float longMorment;
	float shortMorment;
	Matrix3f solidInertia;
	
	longMorment = (solid->GetMass()*(shortAxis*shortAxis+shortAxis*shortAxis))/5;
	shortMorment = (solid->GetMass()*(longAxis*longAxis+shortAxis*shortAxis))/5;

	if(strcmp(solid->GetName(),"soWaist" )== 0){
		solidInertia[0][0] = longMorment;
		solidInertia[1][1] = shortMorment;
		solidInertia[2][2] = longMorment;
	}else if(strcmp(solid->GetName(), "soRFoot")==0 || strcmp(solid->GetName(), "soLFoot")== 0){
		solidInertia[0][0] = shortMorment;
		solidInertia[1][1] = shortMorment;
		solidInertia[2][2] = longMorment;
	}else{
		solidInertia[0][0] = shortMorment;
		solidInertia[1][1] = longMorment;
		solidInertia[2][2] = shortMorment;
	}
	solid->SetInertia(solidInertia);
}

void THuman::SetScale(Spr::Vec2f head, Spr::Vec2f shoulder, Spr::Vec2f body, Spr::Vec2f hip,
	float armLen, float forearmLen, float handLen, float thighLen, float shinLen,
	Spr::Vec3f foot, Spr::Vec3f pos6, Spr::Vec3f pos7){

	springs[0].resize(13);
	springs[1].resize(13);
	float neckOffset = shoulder.Y()*2/3;
	joNeck3->info.v3fPositionChild.Y() = -shoulder.Y()/2-neckOffset;
	joNeck3->info.v3fPositionChild.Z() = shoulder.Y()/2;
	springs[0][0] = TSpring(soHead, Vec3f(0, head.Y() -neckOffset, 0));
	springs[1][0] = TSpring(soHead, Vec3f(0, head.Y() -neckOffset, 0));
	springs[0][1] = TSpring(soHead, Vec3f(-head.X()/2, -neckOffset, 0));
	springs[1][1] = TSpring(soHead, Vec3f( head.X()/2, -neckOffset, 0));	
	joNeck1->info.v3fPositionParent.Y() = body.Y()/2 + shoulder.Y()/2;
	joRShoulder1->info.v3fPositionParent.Y() = body.Y()/2;
	joRShoulder1->info.v3fPositionParent.X() = shoulder.X()/2;
	joLShoulder1->info.v3fPositionParent.Y() = body.Y()/2;
	joLShoulder1->info.v3fPositionParent.X() = -shoulder.X()/2;
	springs[0][2] = TSpring(soBody, Vec3f(-shoulder.X()/2, body.Y()/2, 0));
	springs[1][2] = TSpring(soBody, Vec3f( shoulder.X()/2, body.Y()/2, 0));
	joWaist3->info.v3fPositionChild.Y() = -body.Y()/2;
	springs[0][3] = TSpring(soWaist, Vec3f(-body.X()/2, 0, 0));
	springs[1][3] = TSpring(soWaist, Vec3f( body.X()/2, 0, 0));
	joLHip1->info.v3fPositionParent.Y() = -hip.Y();
	joLHip1->info.v3fPositionParent.X() = -hip.X()/3;
	joRHip1->info.v3fPositionParent.Y() = -hip.Y();
	joRHip1->info.v3fPositionParent.X() = hip.X()/3;
	springs[0][4] = TSpring(soWaist, Vec3f(-hip.X()/2, -hip.Y(),0));
	springs[1][4] = TSpring(soWaist, Vec3f(+hip.X()/2, -hip.Y(),0));
	//	腿の長さ
	joRHip3->info.v3fPositionChild.Y() = thighLen/2;
	joLHip3->info.v3fPositionChild.Y() = thighLen/2;
	joRKnee1->info.v3fPositionParent.Y() = -thighLen/2;
	joLKnee1->info.v3fPositionParent.Y() = -thighLen/2;
	springs[0][5] = TSpring(soLULeg, Vec3f(-hip.X()/6,-thighLen/2,0));
	springs[1][5] = TSpring(soRULeg, Vec3f(+hip.X()/6,-thighLen/2,0));
	//	脛の長さ
	joRKnee1->info.v3fPositionChild.Y() = shinLen/2;
	joLKnee1->info.v3fPositionChild.Y() = shinLen/2;
	joRAnkle1->info.v3fPositionParent.Y() = -shinLen/2;
	joLAnkle1->info.v3fPositionParent.Y() = -shinLen/2;
	//	8と9を結ぶ線を足の向きとする．
	//	8と9の中点を原点，8の向きをZ+，右向きをX+として右足の6と7の位置を求める
	Affinef af;
	af.Ex() *= foot.X() / 2;
	af.Ey() *= foot.Y() / 2;
	af.Ez() *= foot.Z() / 2;
	SGFrame* frL;
	soLFoot->GetFrame()->FindObject(frL);
	frL->FindObject(frL);
	frL->SetPosture(af);	
	SGFrame* frR;
	soRFoot->GetFrame()->FindObject(frR);
	frR->FindObject(frR);
	frR->SetPosture(af);
	//	剛体の中心から見た位置に変換
	Vec3f footOffset(-pos6.X()/2, -foot.Y()/2, 0);	//	剛体の中心から見た，89の中点
	pos6 += footOffset;
	pos7 += footOffset;
	
	joLAnkle1->info.v3fPositionChild = Vec3f(-pos6.X()+foot.X()/2, pos6.Y(), pos6.Z());
	joRAnkle1->info.v3fPositionChild = Vec3f( pos6.X()-foot.X()/2, pos6.Y(), pos6.Z());
	springs[0][6] = TSpring(soLFoot, Vec3f(-pos6.X(), pos6.Y(), pos6.Z()));
	springs[1][6] = TSpring(soRFoot, Vec3f( pos6.X(), pos6.Y(), pos6.Z()));
	springs[0][7] = TSpring(soLFoot, Vec3f(-pos7.X(), pos7.Y(), pos7.Z()));
	springs[1][7] = TSpring(soRFoot, Vec3f( pos7.X(), pos7.Y(), pos7.Z()));
	springs[0][8] = TSpring(soLFoot, Vec3f(-footOffset.X(), footOffset.Y(), foot.Z()/2));
	springs[1][8] = TSpring(soRFoot, Vec3f( footOffset.X(), footOffset.Y(), foot.Z()/2));
	springs[0][9] = TSpring(soLFoot, Vec3f(-footOffset.X(), footOffset.Y(),-foot.Z()/2));
	springs[1][9] = TSpring(soRFoot, Vec3f( footOffset.X(), footOffset.Y(),-foot.Z()/2));

	//	上腕
	joLShoulder3->info.v3fPositionChild.Y() = armLen/2;
	joRShoulder3->info.v3fPositionChild.Y() = armLen/2;
	joLElbow1->info.v3fPositionParent.Y() = -armLen/2;
	joRElbow1->info.v3fPositionParent.Y() = -armLen/2;
	springs[0][10] = TSpring(soLUArm, Vec3f(0,-armLen/2,0));
	springs[1][10] = TSpring(soRUArm, Vec3f(0,-armLen/2,0));
	//	前腕
	joLElbow1->info.v3fPositionChild.Y() = forearmLen/2;
	joRElbow1->info.v3fPositionChild.Y() = forearmLen/2;
	joLWrist1->info.v3fPositionParent.Y() = -forearmLen/2;
	joRWrist1->info.v3fPositionParent.Y() = -forearmLen/2;
	springs[0][11] = TSpring(soLFArm, Vec3f(0,-forearmLen/2,0));
	springs[1][11] = TSpring(soRFArm, Vec3f(0,-forearmLen/2,0));
	//	手
	joLWrist2->info.v3fPositionChild.Y() = handLen/2;
	joRWrist2->info.v3fPositionChild.Y() = handLen/2;
	springs[0][12] = TSpring(soLHand, Vec3f(0,-handLen/2,0));
	springs[1][12] = TSpring(soRHand, Vec3f(0,-handLen/2,0));

	//	質量
	SetTotalMass(60.0f);
	//	慣性モーメント
	SetOneInertia(soHead, head.Y(), 1.0f/2.0f);
	SetOneInertia(soBody, body.Y(), 2.0f/3.0f);

	SetOneInertia(soLUArm, armLen, 1.0f/3.0f);
	SetOneInertia(soRUArm, armLen, 1.0f/3.0f);
	SetOneInertia(soLFArm, forearmLen, 1.0f/3.0f);
	SetOneInertia(soRFArm, forearmLen, 1.0f/3.0f);
	SetOneInertia(soLHand, handLen, 1.0f);
	SetOneInertia(soRHand, handLen, 1.0f);

	SetOneInertia(soWaist, hip.Y(), 3.0f/2.0f);

	SetOneInertia(soLULeg, thighLen, 5.0f/8.0f);
	SetOneInertia(soRULeg, thighLen, 5.0f/8.0f);
	SetOneInertia(soLLLeg, shinLen, 1.0f/2.0f);
	SetOneInertia(soRLLeg, shinLen, 1.0f/2.0f);
	SetOneInertia(soLFoot, foot.Z(), 1.0f/3.0f);
	SetOneInertia(soRFoot, foot.Z(), 1.0f/3.0f);
}

//各関節のトルクを時系列で保存する。
void THuman::SaveTorque(float time, float dt){
		jojo[0].push_back(joWaist1->GetTorque());
		jojo[1].push_back(joWaist2->GetTorque());
		jojo[2].push_back(joWaist3->GetTorque());
		jojo[3].push_back(joNeck1->GetTorque());
		jojo[4].push_back(joNeck2->GetTorque());
		jojo[5].push_back(joNeck3->GetTorque());
		jojo[6].push_back(joLShoulder1->GetTorque());
		jojo[7].push_back(joLShoulder2->GetTorque());
		jojo[8].push_back(joLShoulder3->GetTorque());
		jojo[9].push_back(joLElbow1->GetTorque());
		jojo[10].push_back(joLWrist1->GetTorque());
		jojo[11].push_back(joLWrist2->GetTorque());
		jojo[12].push_back(joRShoulder1->GetTorque());
		jojo[13].push_back(joRShoulder2->GetTorque());
		jojo[14].push_back(joRShoulder3->GetTorque());
		jojo[15].push_back(joRElbow1->GetTorque());
		jojo[16].push_back(joRWrist1->GetTorque());
		jojo[17].push_back(joRWrist2->GetTorque());
		jojo[18].push_back(joLHip1->GetTorque());
		jojo[19].push_back(joLHip2->GetTorque());
		jojo[20].push_back(joLHip3->GetTorque());
		jojo[21].push_back(joLKnee1->GetTorque());
		jojo[22].push_back(joLAnkle1->GetTorque());
		jojo[23].push_back(joRHip1->GetTorque());
		jojo[24].push_back(joRHip2->GetTorque());
		jojo[25].push_back(joRHip3->GetTorque());
		jojo[26].push_back(joRKnee1->GetTorque());
		jojo[27].push_back(joRAnkle1->GetTorque());
		jojo[28].push_back(joLShin->GetTorque());
		jojo[29].push_back(joRShin->GetTorque());
}

//VHの重心を計算する。
Spr::Vec3f THuman::GetMassCenterPosition(){
	Spr::Vec3f gc;
	for(int i=0; i<solids.size(); ++i){
		gc += solids[i]->GetMass() * solids[i]->GetCenterPosition();
	}
	return gc / totalMass;
}


extern Spr::GRMaterialData Mat(Vec4f c);
//重心を描画する。
bool THuman::Draw(Spr::GRRender* render){
	if(!IsLoaded()) return false;
	render->SetMaterial(Mat(Vec4f(1,0,0,1)));
	render->SetLineWidth(10);
	Spr::Vec3f pos;
	pos = GetMassCenterPosition();
	render->DrawDirect(GRRender::POINTS, &pos , &pos+1);
	return true;
}

