#include "Creature.h"
#pragma hdrstop
// CRBallUser.cpp: CRBallUser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRBallUser.h"

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

SGOBJECTIMP(CRBallUser, CRBallPuppet);

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////// CRBallUserクラス ////////////////////
CRBallUser::CRBallUser(){
}

void CRBallUser::Load(SGScene* scene){
	scene->FindObject(jointEngine, "jeUser");
	if (!jointEngine){
		DSTR << "JointEngen 'jeUser' is not found." << std::endl;
		scene->GetBehaviors().Find(jointEngine);
	}
	Loaded(scene);
	bSpidarForce = false;
}

bool CRBallUser::Connect(UTRef<SGScene> scene){
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

void CRBallUser::SetSprings(){
	if(!IsLoaded()) return;
	positionSprings.clear();
	PositionSpring positionSpr;

	// [0] 腰(基本立ち位置に固定)
//	postureSpring.SetSolid(solids[0], 0.1f, 1.0f);
	postureSpring.SetSolid(solids[0], 0.01f, 0.6f);
	positionSpr.SetSolid(solids[0], Vec3f(0, 0, 0), 0.05f, 1.0f);
	positionSprings.push_back(positionSpr);
	// [1] 右手(Spidarに対応)
	positionSpr.SetSolid(solids[6],  Vec3f(0, 0, 0), 1.0f, 1.0f);
	positionSprings.push_back(positionSpr);
	// [2] 左手
	positionSpr.SetSolid(solids[9],  Vec3f(0, 0, 0), 1.0f, 1.0f);
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

void CRBallUser::SetFixedPos(){
	if(!IsLoaded()) return;
	postureSpring.SetTarget(Quaterniond(1,0,0,0), Vec3f(), true);
	positionSprings[0].SetTarget(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0,0,0), true);
}

void CRBallUser::SetSpidarPos(std::vector<HISpidar4*> spidars){
	// Spidar のスケール調整込み 右手[1] 左手[2]
	for(int i = 0; i < spidars.size(); ++i){
		const float SCALE = 2.0f;
		//const float SCALE = 1.25f;	// ミーティングルーム
		Vec3f tPos, tVel;
		tPos = spidars[i]->GetPos() * SCALE;
		tVel = spidars[i]->GetVel() * SCALE;
		tPos.y += 1.2f;
		tPos.z -= 0.6f;

		positionSprings[i+1].SetTarget(tPos, tVel, true);
	}
	for(int i = 0; i < 2 - spidars.size(); ++i){
		positionSprings[2-i].SetTarget(Vec3f(), Vec3f(), false);
	}
}

void CRBallUser::SetExpectedPos(int count, float dt){
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

void CRBallUser::SetSpidarForce(CRBallPuppet* puppet, SGScene* scene, std::vector<HISpidar4*> spidars){
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
			float maxForce = 2.5f;
			if(force.norm() > maxForce) force = force.unit() * maxForce;
			spidars[i]->SetForce(-force);
		}
	}
}

DEF_RECORD(XUser,{
	GUID Guid(){ return WBGuid("2E2E7E73-162A-4b36-9FA0-89E2BCEB7102"); }
});
class CRBallUserLoader : public FIObjectLoader<CRBallUser>{
public:
	CRBallUserLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XUser);
	}
	bool LoadData(FILoadScene* ctx, CRBallUser* h){
		return true;
	}
};
class CRBallUserSaver : public FIObjectSaver<CRBallUser>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRBallUser* h){
	}
};
DEF_REGISTER_BOTH(CRBallUser);
}
