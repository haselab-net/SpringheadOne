// Model.cpp: Model クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Model.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

Model::Model()
{

}

Model::~Model()
{

}

void Model::Load(SGScene* scene){
	Connect(scene);
	SetLoaded(true);
	SetModel(scene);
	RegistSupportParts(scene);
	LoadDerivedModel(scene);
}
void Model::LoadDerivedModel(SGScene* scene){
	balance.Load(scene,this);
}

bool Model::Connect(UTRef<SGScene> scene){
	solids.clear();
	joints.clear();
	jointPids.clear();

	SetLoaded(true);

	// Solid Connect
	ConnectSolid("soBox0", scene);
	ConnectSolid("soBox1", scene);
	ConnectSolid("soBox2", scene);
	ConnectSolid("soBox3", scene);

	ConnectSolid("soRHeel", scene);
	ConnectSolid("soRToe", scene);


	//Joint Connect	
	ConnectJoint("jo0", scene);
	ConnectJoint("jo1", scene);
	ConnectJoint("jo2", scene);

	ConnectJoint("joRAnkle1", scene);
	ConnectJoint("joRAnkle2", scene);
	ConnectJoint("joRAnkle3", scene);
	ConnectJoint("joRToe", scene);

	for(int i = 0; i<joints.size(); ++i){
		jointPids.push_back(PHJointPid::Find(joints[i], scene));
	}

	//	接触エンジンとの接続
	contactPairs.clear();
	PHContactEngine* pe;
	scene->GetBehaviors().Find(pe);
	if (pe){
		CDCollisionEngine* ce = pe->GetCollisionEngine();
		for(CDCollisionEngine::TFrameIt it = ce->FramePairBegin(); it != ce->FramePairEnd(); ++it){
			if (!*it) continue;
			bool b0 = (*it)->frame[0] && HasFrame((*it)->frame[0]->frame);
			bool b1 = (*it)->frame[1] && HasFrame((*it)->frame[1]->frame);
			if ((b0 && b1) || (!b0 && !b1)) continue;
			float sign = b0 ? 1.0f : -1.0f;
			PHContactEngine::FramePairRecord* fpr = UTRef<PHContactEngine::FramePairRecord>((*it)->records[pe->GetFramePairRecordPos()]);
			contactPairs.push_back(CDContact(fpr, sign));
		}
	}	
	return IsLoaded();
}

void Model::SetInertia(){
	for(unsigned i=0; i<solids.size(); ++i){
		if(solids[i] != NULL){
			if(i == solids.size()-2)		// 踵
				SetOneInertia(solids[i], sinfo[12].scale);
			else if(i == solids.size()-1)	// 土踏まず
				SetOneInertia(solids[i], sinfo[13].scale);
			else							// 太もものframe,solidを使っている。
				SetOneInertia(solids[i], sinfo[10].scale);
		}
	}
}
void Model::SetJointRange(){
	for(unsigned i = 0; i < joints.size(); i++){
		if(joints[i] != NULL){
			if(i == joints.size()-4)		// 足首X
				SetOneJointRange(joints[i], jinfo[27].rangeMin, jinfo[27].rangeMax);
			else if(i == joints.size()-3)	// 足首Z
				SetOneJointRange(joints[i], jinfo[28].rangeMin, jinfo[28].rangeMax);
			else if(i == joints.size()-2)	// 足首-Y
				SetOneJointRange(joints[i], jinfo[29].rangeMin, jinfo[29].rangeMax);
			else if(i == joints.size()-1)	// 土踏まず
				SetOneJointRange(joints[i], jinfo[30].rangeMin, jinfo[30].rangeMax);
			else
				SetOneJointRange(joints[i], -100, 100);
		}
	}
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

void Model::SetJointSpring(float dt){
	const float SAFETYRATE = 0.02f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f * SAFETYRATE;
	for(int i=0; i<jointPids.size(); ++i){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}
}
void Model::MaintainBalance(SGScene* scene,GRRender* render){
	// 安定領域計算
	for(unsigned i = 0; i < supportObject.size(); i++){
		supportObject[i].CalcSupportArea();
	}
	supportArea.CalcSupportArea();

	balance.Step(scene,this);

}

void Model::RegistSupportParts(UTRef<SGScene> scene){

	if(IsLoaded()){
		supportArea.contacts.clear();
		supportObject.clear();

		// 体を支持するオブジェクト(右足,左足)の数だけCREnumContactVertexとCRSupportAreaを作る
		UTRef<CREnumContactVertex> contacts[1];
		supportObject.resize(1);

		// 右足で地面と接触するSolid
		contacts[0] = new CREnumContactVertex;
		contacts[0]->AddSubject(solids[4]->GetFrame());	// 踵
		contacts[0]->AddSubject(solids[5]->GetFrame());	// 爪先

		for(unsigned i = 0; i < 1; i++){
			ExceptContactParts(contacts[i]);
			contacts[i]->Loaded(scene);
		}

		for(unsigned i = 0; i < 1; i++){
			// 片足ずつ
			supportObject[i].SetContacts(contacts[i]);
			// 体全体なので両足をセットする
			supportArea.SetContacts(contacts[i]);
		}
	}
}

void Model::ExceptContactParts(CREnumContactVertex* contact){
	for(unsigned i = 0; i < solids.size(); i++){
		if(!(solids[i] == NULL)){
			contact->AddExcept(solids[i]->GetFrame());
		}
	}
}

}		// end namespace