// ModelUnder.cpp: ModelUnder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModelUnder.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

ModelUnder::ModelUnder()
{

}

ModelUnder::~ModelUnder()
{

}

void ModelUnder::Load(SGScene* scene){
	Connect(scene);
	SetLoaded(true);
	SetModel(scene);
	RegistSupportParts(scene);
	LoadDerivedModel(scene);

	stayPosi[0].SetSolid(solids[12],Vec3f(0.0,-GetSolidInfo(12).scale.y,0.0),1.1,1.1);
	stayPosi[1].SetSolid(solids[16],Vec3f(0.0,-GetSolidInfo(16).scale.y,0.0),1.1,1.1);
	stayPost[0].SetSolid(solids[12],100.0,100.0);
	stayPost[1].SetSolid(solids[16],100.0,100.0);

	stayPosi[0].SetTarget(Vec3f(0.1,0.0,0.0),Vec3f(),true);
	stayPosi[1].SetTarget(Vec3f(-0.1,0.0,0.0),Vec3f(),true);
	stayPost[0].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);
	stayPost[1].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);


}

void ModelUnder::LoadDerivedModel(SGScene* scene){
	balance.Load(scene,this);
}

inline float GetChildMass(PHJointBase* j){
	if (strlen(j->solid->GetName()) != 0){
		return j->solid->GetMass();
	}
	else{
		float rv = 0;
		for(unsigned i=0; i<j->Children().size(); ++i){
			rv += GetChildMass(j->Children()[i]);
		}
		return rv;
	}
}

void ModelUnder::SetJointSpring(float dt){
	const float SAFETYRATE = 0.01f;
	float k = 0.2f/2 * SAFETYRATE;
	float b = 0.6f * SAFETYRATE;
	for(int i=0; i<jointPids.size(); ++i){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}
	// 爪先を硬くする(右足)
	if(jointPids[30] != NULL)
			JointPIDMul(jointPids[30], 10.0f, 10.0f);
	
	// 爪先を硬くする(左足)
	if(jointPids[38] != NULL)
			JointPIDMul(jointPids[38], 10.0f, 10.0f);

}

void ModelUnder::RegistSupportParts(UTRef<SGScene> scene){

	if(IsLoaded()){
		supportObject.clear();
		supportArea.contacts.clear();
		UTRef<CREnumContactVertex> contacts[2];

		// 体を支持するオブジェクト(右足,左足)の数だけCREnumContactVertexとCRSupportAreaを作る
		supportObject.resize(2);

		// 右足で地面と接触するSolid
		contacts[0] = new CREnumContactVertex;
		contacts[0]->AddSubject(solids[12]->GetFrame());	// 踵
		contacts[0]->AddSubject(solids[13]->GetFrame());	// 爪先

		// 左足で地面と接触するSolid
		contacts[1] = new CREnumContactVertex;
		contacts[1]->AddSubject(solids[16]->GetFrame());	// 踵
		contacts[1]->AddSubject(solids[17]->GetFrame());	// 爪先

		for(unsigned i = 0; i < 2; i++){
			ExceptContactParts(contacts[i]);
			contacts[i]->Loaded(scene);
		}

		for(unsigned i = 0; i < 2; i++){
			// 片足ずつ
			supportObject[i].SetContacts(contacts[i]);
			// 体全体なので両足をセットする
			supportArea.SetContacts(contacts[i]);
		}
	}
}

void ModelUnder::ExceptContactParts(CREnumContactVertex* contact){
	for(unsigned i = 0; i < solids.size(); i++){
		if(!(solids[i] == NULL)){
			contact->AddExcept(solids[i]->GetFrame());
		}
	}
}

void ModelUnder::MaintainBalance(SGScene* scene,GRRender* render){
	// 安定領域計算
	//for(unsigned i = 0; i < supportObject.size(); i++){
	for(unsigned i = 0; i < 2; i++){
		supportObject[i].CalcSupportArea();
	}
	supportArea.CalcSupportArea();

	balance.Step(scene,this);

}

void ModelUnder::SetJointInitAngle(){
	CRHuman::SetJointInitAngle();
/*
	float angle = Rad(30);
	joints[23]->position = jointPids[23]->goal = angle;
	joints[26]->position = jointPids[26]->goal = 2*angle;
	joints[27]->position = jointPids[27]->goal = angle;

	joints[31]->position = jointPids[31]->goal = angle;
	joints[34]->position = jointPids[34]->goal = 2*angle;
	joints[35]->position = jointPids[35]->goal = angle;
*/

}

}		// end namespace