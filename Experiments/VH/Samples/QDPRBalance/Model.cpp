// Model.cpp: Model �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Model.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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

	//	�ڐG�G���W���Ƃ̐ڑ�
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
			if(i == solids.size()-2)		// ��
				SetOneInertia(solids[i], sinfo[12].scale);
			else if(i == solids.size()-1)	// �y���܂�
				SetOneInertia(solids[i], sinfo[13].scale);
			else							// ��������frame,solid���g���Ă���B
				SetOneInertia(solids[i], sinfo[10].scale);
		}
	}
}
void Model::SetJointRange(){
	for(unsigned i = 0; i < joints.size(); i++){
		if(joints[i] != NULL){
			if(i == joints.size()-4)		// ����X
				SetOneJointRange(joints[i], jinfo[27].rangeMin, jinfo[27].rangeMax);
			else if(i == joints.size()-3)	// ����Z
				SetOneJointRange(joints[i], jinfo[28].rangeMin, jinfo[28].rangeMax);
			else if(i == joints.size()-2)	// ����-Y
				SetOneJointRange(joints[i], jinfo[29].rangeMin, jinfo[29].rangeMax);
			else if(i == joints.size()-1)	// �y���܂�
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
	// ����̈�v�Z
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

		// �̂��x������I�u�W�F�N�g(�E��,����)�̐�����CREnumContactVertex��CRSupportArea�����
		UTRef<CREnumContactVertex> contacts[1];
		supportObject.resize(1);

		// �E���Œn�ʂƐڐG����Solid
		contacts[0] = new CREnumContactVertex;
		contacts[0]->AddSubject(solids[4]->GetFrame());	// ��
		contacts[0]->AddSubject(solids[5]->GetFrame());	// �ܐ�

		for(unsigned i = 0; i < 1; i++){
			ExceptContactParts(contacts[i]);
			contacts[i]->Loaded(scene);
		}

		for(unsigned i = 0; i < 1; i++){
			// �Б�����
			supportObject[i].SetContacts(contacts[i]);
			// �̑S�̂Ȃ̂ŗ������Z�b�g����
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