#include "Creature.h"
#pragma hdrstop
// CRUser.cpp: CRUser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRUser.h"

/*#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

//using namespace Spr;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

SGOBJECTIMP(CRUser, CRPuppet);

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////// CRUserクラス ////////////////////
CRUser::CRUser(){
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

void CRUser::SetJointSpring(float dt){
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

	/*
	if(jointBallPids[0] != NULL){
		JointBallPIDMul(jointBallPids[0], 0.4f, 1.0f);
	}
	if(jointPids[1] != NULL){
		JointPIDMul(jointPids[1], 0.4f, 0.8f);
	}
	// 関節を柔らかめに設定(首)
	if(jointBallPids[2] != NULL){
		JointBallPIDMul(jointBallPids[2], 0.3f, 0.8f);
	}
	// 関節を柔らかめに設定(右腕)
	if(jointBallPids[3] != NULL){
		JointBallPIDMul(jointBallPids[3], 0.04f, 0.5f);
	}
	if(jointPids[4] != NULL){
		JointPIDMul(jointPids[4], 0.01f, 0.25f);
	}

	// 関節を柔らかめに設定(左腕)
	if(jointBallPids[6] != NULL){
		JointBallPIDMul(jointBallPids[6], 0.04f, 0.5f);
	}
	if(jointPids[7] != NULL){
		JointPIDMul(jointPids[7], 0.01f, 0.25f);
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
		JointBallPIDMul(jointBallPids[3], 0.08f*1.5f, 1.0f*3.0f);
	}
	if(jointPids[4] != NULL){
		//JointPIDMul(jointPids[4], 0.06f, 0.1f);
		JointPIDMul(jointPids[4], 0.06f*3.0f, 0.1f*10.0f);
	}

	// 関節を柔らかめに設定(左腕)
	if(jointBallPids[6] != NULL){
		//JointBallPIDMul(jointBallPids[6], 0.08f, 1.0f);
		JointBallPIDMul(jointBallPids[6], 0.08f*1.5f, 1.0f*3.0f);
	}
	if(jointPids[7] != NULL){
		//JointPIDMul(jointPids[7], 0.06f, 0.1f);
		JointPIDMul(jointPids[7], 0.06f*3.0f, 0.1f*10.0f);
	}
}

void CRUser::Load(SGScene* scene){
	scene->FindObject(jointEngine, "jeUser");
	if (!jointEngine){
		DSTR << "JointEngine 'jeUser' is not found." << std::endl;
		scene->GetBehaviors().Find(jointEngine);
	}
	Loaded(scene);
	bSpidarForce = false;
}

bool CRUser::Connect(UTRef<SGScene> scene){
	solids.clear();
	joints.clear();
	jointPids.clear();
	jointBallPids.clear();

	SetLoaded(true);

	// Solid Connect
	ConnectSolid("soWaistU", scene);
	ConnectSolid("soAbdomenU", scene);
	ConnectSolid("soChestU", scene);
	ConnectSolid("soHeadU", scene);

	ConnectSolid("soRUArmU", scene);
	ConnectSolid("soRFArmU", scene);
	ConnectSolid("soRHandU", scene);

	ConnectSolid("soLUArmU", scene);
	ConnectSolid("soLFArmU", scene);
	ConnectSolid("soLHandU", scene);
/*
//	ConnectSolid("soRThighU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRLThighU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRHeelU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soRToeU", scene);
	solids.push_back(NULL);

//	ConnectSolid("soLThighU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLLThighU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLHeelU", scene);
	solids.push_back(NULL);
//	ConnectSolid("soLToeU", scene);
	solids.push_back(NULL);
*/
	//Joint Connect
	ConnectJoint("joWaistU", scene);
	ConnectJoint("joChestU", scene);
	ConnectJoint("joNeckU", scene);
	
	ConnectJoint("joRShoulderU", scene);
	ConnectJoint("joRElbowU", scene);
	ConnectJoint("joRWristU", scene);

	ConnectJoint("joLShoulderU", scene);
	ConnectJoint("joLElbowU", scene);
	ConnectJoint("joLWristU", scene);
/*
	ConnectJoint("joWaist1U", scene);
	ConnectJoint("joWaist2U", scene);
//	ConnectJoint("joWaist3U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joChest1U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joChest2U", scene);
	joints.push_back(NULL);
	ConnectJoint("joChest3U", scene);
	ConnectJoint("joNeck1U", scene);
	ConnectJoint("joNeck2U", scene);
//	ConnectJoint("joNeck3U", scene);
	joints.push_back(NULL);
	
	ConnectJoint("joRShoulder1U", scene);
	ConnectJoint("joRShoulder2U", scene);
	ConnectJoint("joRShoulder3U", scene);
	ConnectJoint("joRElbow1U", scene);
//	ConnectJoint("joRElbow2U", scene);
	joints.push_back(NULL);
	ConnectJoint("joRWrist1U", scene);
//	ConnectJoint("joRWrist2U", scene);
	joints.push_back(NULL);

	ConnectJoint("joLShoulder1U", scene);
	ConnectJoint("joLShoulder2U", scene);
	ConnectJoint("joLShoulder3U", scene);
	ConnectJoint("joLElbow1U", scene);
//	ConnectJoint("joLElbow2U", scene);
	joints.push_back(NULL);
	ConnectJoint("joLWrist1U", scene);
//	ConnectJoint("joLWrist2U", scene);
	joints.push_back(NULL);
*/
/*
//	ConnectJoint("joRHip1U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRHip2U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRHip3U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRKneeU", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle1U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle2U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRAnkle3U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joRToeU", scene);
	joints.push_back(NULL);

//	ConnectJoint("joLHip1U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLHip2U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLHip3U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLKneeU", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle1U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle2U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLAnkle3U", scene);
	joints.push_back(NULL);
//	ConnectJoint("joLToeU", scene);
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

void CRUser::SetSprings(){
	if(!IsLoaded()) return;
	positionSprings.clear();
	PositionSpring positionSpr;

	// [0] 腰(基本立ち位置に固定)
//	postureSpring.SetSolid(solids[0], 0.1f, 1.0f);
	postureSpring.SetSolid(solids[0], 0.1f, 2.0f);
	positionSpr.SetSolid(solids[0], Vec3f(0, 0, 0), 0.6f, 10.0f);
	positionSprings.push_back(positionSpr);
	// [1] 右手(Spidarに対応)
	positionSpr.SetSolid(solids[6],  Vec3f(0, 0, 0), 10.0f, 10.0f);
	positionSprings.push_back(positionSpr);
	// [2] 左手
	positionSpr.SetSolid(solids[9],  Vec3f(0, 0, 0), 10.0f, 10.0f);
	positionSprings.push_back(positionSpr);
	// [3] 頭頂(ラベリングカメラに対応)
//	positionSpr.SetSolid(solids[3],  Vec3f(0,  GetSolidInfo(3).scale.Y(),  0), 0.1f, 0.1f);
//	positionSprings.push_back(positionSpr);
	// [4] 右肘
//	positionSpr.SetSolid(solids[4],  Vec3f(0, -GetSolidInfo(4).scale.Y(),  0), 0.01f, 0.01f);
//	positionSprings.push_back(positionSpr);
	// [5] 左肘
//	positionSpr.SetSolid(solids[7],  Vec3f(0, -GetSolidInfo(7).scale.Y(),  0), 0.01f, 0.01f);
//	positionSprings.push_back(positionSpr);
}

void CRUser::SetFixedPos(){
	if(!IsLoaded()) return;
	Vec3f pos = (solids[6]->GetCenterPosition() + solids[9]->GetCenterPosition()) * 0.5f;
	pos[1] = 1.0f;
	pos += Vec3f(0.0f, 0.0f, 0.3f);
	positionSprings[0].SetTarget(pos, Vec3f(0,0,0), true);
	postureSpring.SetTarget(Quaterniond(1,0,0,0), Vec3f(), true);

	/*
	positionSprings[0].SetTarget(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0,0,0), true);
	*/
}

void CRUser::SetSpidarPos(std::vector<HISpidar4*> spidars){
	// Spidar のスケール調整込み 右手[1] 左手[2]
	for(int i = 0; i < spidars.size(); ++i){
		const float SCALE = 1.0f;
		//const float SCALE = 1.25f;	// ミーティングルーム
		Vec3f tPos, tVel;
		tPos = spidars[i]->GetPos() * SCALE;
		tVel = spidars[i]->GetVel() * SCALE;
		tPos.y += 1.4f;
		tPos.z -= 0.3f;

		if (tVel.norm() > 1.0f){
			tVel.unitize();
		}

		positionSprings[i+1].SetTarget(tPos, tVel, true);
	}
	for(int i = 0; i < 2 - spidars.size(); ++i){
		positionSprings[2-i].SetTarget(Vec3f(), Vec3f(), false);
	}
}

void CRUser::SetExpectedPos(int count, float dt){
	// 右手と左手の count ステップ後の位置制御
	for(int i = 0; i < 2; ++i){
		if(locus.bPrediction[i] && locus.ak[i]){
			Vec3f tPos, tVel;
			tPos = locus.CalcFuturePosition(locus.ak[i], count);
			tVel = locus.CalcFutureVelocity(locus.ak[i], count, dt/2);
			positionSprings[i+1].SetTarget(tPos, tVel, true);
		}
	}
}

void CRUser::SetSpidarForce(CRPuppet* puppet, SGScene* scene, std::vector<HISpidar4*> spidars){
	if(bSpidarForce){
		for(int i = 0; i < 2; ++i){
			Vec3f force = Vec3f();
			force += humanContactInfo.GetContactForceOfSolid(solids[3*i+6], puppet, scene);
			for(int j = 0; j < 2; ++j){
			//	force += 0.4f * humanContactInfo.GetContactForceOfSolid(solids[3*i+j+4], puppet, scene);
			}
			for(int j = 0; j < 2; ++j){
				force += 0.1f * humanContactInfo.GetContactForceOfSolid(solids[j+2], puppet, scene);
			}
			float maxForce = 4.0f;
			if(force.norm() > maxForce) force = force.unit() * maxForce;
			spidars[i]->SetForce(-force);
		}
	}
}

void CRUser::SetJointBasicPos(){
	// Waist
	if(jointBallPids[0]) jointBallPids[0]->goal = jinfo[0].initQt = Quaternionf(cosf(-0.1f), sinf(-0.1f), 0.0f, 0.0f);

	// Shoulder(R,L)
	if(jointBallPids[3]) jointBallPids[3]->goal = jinfo[3].initQt = Quaternionf(cosf(0.4f), sinf(0.4f), 0.0f, 0.0f) * Quaternionf(cosf(-0.25f), 0.0f, 0.0f, sinf(-0.25f));
	if(jointBallPids[6]) jointBallPids[6]->goal = jinfo[6].initQt = Quaternionf(cosf(0.4f), sinf(0.4f), 0.0f, 0.0f) * Quaternionf(cosf(0.25f), 0.0f, 0.0f, sinf(0.25f));

	// Elbow(R,L)
	if(jointPids[4]) jointPids[4]->goal = jinfo[4].initPos = 1.9f; //2.3f;
	if(jointPids[7]) jointPids[7]->goal = jinfo[7].initPos = 1.9f; //2.3f;
}


void CRUser::PlayHitSound(){
	// GRSound::instance()->play( SAMPLE2 );
}

DEF_RECORD(XUser,{
	GUID Guid(){ return WBGuid("2E2E7E73-162A-4b36-9FA0-89E2BCEB7102"); }
});
class CRUserLoader : public FIObjectLoader<CRUser>{
public:
	CRUserLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XUser);
	}
	bool LoadData(FILoadScene* ctx, CRUser* h){
		return true;
	}
};
class CRUserSaver : public FIObjectSaver<CRUser>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRUser* h){
	}
};
DEF_REGISTER_BOTH(CRUser);
}
