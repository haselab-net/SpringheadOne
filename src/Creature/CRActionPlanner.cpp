// CRActionPlanner.cpp: CRActionPlanner �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRActionPlanner.h"
#include <Graphics/GRRender.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CRActionPlanner::GoalDirectedMovement::GoalDirectedMovement(){
	type = STANDBY;
}

//////////////////// ���B�^���N���X(��) ////////////////////
void CRActionPlanner::GoalDirectedMovement::AddForce(float time){
	if(time >= end) return;
	const float MINIMUN = 0.005f;
	const float SAFETYRATE = 0.10f;
	const float SPRING = 0.1f * SAFETYRATE;		//	�o�l
	const float DAMPER = 0.9f * SAFETYRATE;		//	�_���p

	Spr::Vec3f pos2 = solid->GetCenterPosition(); // ���݂�Pos,Vel
	Spr::Vec3f vel2 = solid->GetVelocity();

	if(end - time > MINIMUN){
		temp = (MINIMUN * target + (end - time - MINIMUN) * pos2) / (end - time);
		period = MINIMUN;
	}
	else{
		temp = target;
		period = end - time;
	}
	float mass = solid->GetMass();
	float spring = SPRING * mass * 2 / (period * period);
	float damper = DAMPER * mass / (period);

	if(target.square() < Spr::Square(900)){
		Spr::Vec3f dPos = target - pos2;
		Spr::Vec3f dVel = -vel2;
		if(dVel.norm() > 1e5f) dVel = dVel.unit()*1e5f;
		if(dPos.norm() > 1e2f) dPos = dPos.unit()*1e2f;
		Spr::Vec3f force = spring*dPos + damper*dVel;
		if (force.norm() / mass > 1e7f) force = force.unit() * mass*1e7f;
		solid->AddForce(force);
	}
}

//////////////////// �ڐG���N���X ////////////////////
void CRActionPlanner::ContactInfo::SetContactInfo(int vh, int user, int c, float t){
	soVHIndex = vh;
	soUserIndex = user;
	firstCount = lastCount = c;
	firstTime = t;
	SetContactType(vh, user);
}

void CRActionPlanner::ContactInfo::SetContactType(int vh, int user){
	// ���ʂ̐ڐG
	this->contactType = 0;

	if(0 <= vh && vh <= 3){
		if(user == 6 || user == 9) this->contactType = 1;
	}

	if(vh == 6 || vh == 9){
		// VH �̍U�����q�b�g
		if(0 <= user && user <= 3) this->contactType = 2;
		// VH �̍U�����K�[�h
		if((4 <= user && user <= 6) || (7 <= user && user <= 9)) this->contactType = 4;
	}

	// ���[�U�̍U�����K�[�h
	if((4 <= vh && vh <= 6) || (7 <= vh && vh <= 9)){
		if(user == 6 || user == 9) this->contactType = 3;
	}
}

//////////////////// �\���s���N���X ////////////////////
CRActionPlanner::CRActionPlanner()
{
	bPrediction = false;
	bLoadReal   = false;
	bLoadTemp   = false;
	times     = 0;
	startTime = 0;
}
	
void CRActionPlanner::SaveState(Spr::SGScene* scene, bool type){
	if(type){
		if(stateReal.size() > 0) stateReal.clear();
		scene->SaveState(stateReal);	// type == 1 �̎��͌��݂̏�ԂƂ��ĕۑ�
		bLoadReal = true;
	}
	else{
		if(stateTemp.size() > 0) stateTemp.clear();
		scene->SaveState(stateTemp);	// type == 0 �̎��͓����̏�ԂƂ��ĕۑ�
		bLoadTemp = true;
	}
}

void CRActionPlanner::LoadState(Spr::SGScene* scene, bool type){
	if(type){
		if(bLoadReal) scene->LoadState(stateReal);	// type == 1 �̎��͌��݂̏�Ԃ�ǂݍ���
	}
	else{
		if(bLoadTemp) scene->LoadState(stateTemp);	// type == 0 �̎��͓����̏�Ԃ�ǂݍ���
	}
}

#include "../../../Experiments/VH/VHInteraction/VHCheckDialog.h"

void CRActionPlanner::PredictionAction(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene){
	const int SETCOUNT = 20;

	if(times >= SETCOUNT){
		ChooseTargetAction();

		bPrediction = false;
		times = 0;
		return;
	}

	SaveState(scene, 1);		// ���݂̏�Ԃ�ۑ�
	if(times == 0){
		SaveState(scene, 0);	// ������Ԃ�ۑ�
		contactInfo.clear();
		startTime = scene->GetCount() * scene->GetTimeStep();
	}

	MovementPrediction(human, user, scene, times);
	LoadState(scene, 1);		// ���݂̏�Ԃ�ǂݍ���

	scene->SaveState(stateReal);
	scene->LoadState(stateReal);

	times++;
}

void CRActionPlanner::MovementPrediction(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene, int count){
	const int STEPCOUNT = 5;
	const float RATE = 2.5f;
	
	LoadState(scene, 0);	// �r����Ԃ���ĊJ
	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		Step(human, user, scene);
		ContactTest(human, &user->crHuman, scene, count * STEPCOUNT + i);
	}

	// �X�V���ʂ�`��
	Spr::GRRender* render;
	scene->GetRenderers().Find(render);
	vhCheckDialog->DrawTest(render, scene);

	scene->SetTimeStep(dt);
	SaveState(scene, 0);	// �r����Ԃ�ۑ�
}

void CRActionPlanner::Step(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene){
	float dt = scene->GetTimeStep();

	scene->ClearForce();
	
	user->SetTargetPosition2(dt);
	user->InputUserPosition(dt);
	
	scene->GenerateForce();
	scene->Integrate();
}

bool CRActionPlanner::IsFirstContact(int vh, int user, int count){
	for(int i = 0; i < contactInfo.size(); ++i){
		if(contactInfo[i].soVHIndex == vh && contactInfo[i].soUserIndex == user){
			// ���O���ڐG���Ă���������
			if(contactInfo[i].lastCount == count - 1){
				contactInfo[i].lastCount = count;
				return false;
			}
		}
	}
	return true;
}

void CRActionPlanner::ContactTest(Spr::CRHuman* human1, Spr::CRHuman* human2, Spr::SGScene* scene, int step){
	for(int i = 0; i < human1->solids.size(); ++i){
		for(int j = 0; j < human2->solids.size(); ++j){
			// �Q�̍��̂̐ڐG����
			if(ContactCheckOfSolidPair(human1->solids[i], human2->solids[j], scene)){
				// �O����ڐG���Ă���������
				if(IsFirstContact(i, j, step)){
					ContactInfo cInfo;
					cInfo.SetContactInfo(i, j, step, startTime + step * scene->GetTimeStep());
					SetContactPointOfSolidPair(human1->solids[i], human2->solids[j], scene, cInfo.contactPoint);
					cInfo.soVH   = human1->solids[i];
					cInfo.soUser = human2->solids[j];
					// �ڐG����o�^
					contactInfo.push_back(cInfo);

					/*DSTR << human1->solids[i]->GetName() << " collided with " 
						 << human2->solids[j]->GetName() << " at " << time << std::endl;
					DSTR << "TimeStep: " << scene->GetTimeStep() << std::endl;*/
				}
			}
		}
	}
}

void CRActionPlanner::SetContactPointOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene, Spr::Vec3f* pos){
	Spr::CDCollisionEngine* ce = NULL;
	Spr::CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		pos[0] = fp->intersections.begin()->convexPair->commonPoint;		// �O���[�o�����W�n
		pos[1] = fp->intersections.begin()->convexPair->closestPoint[0];	// soVH ���W�n
		pos[2] = fp->intersections.begin()->convexPair->closestPoint[1];	// soUser ���W�n
	}
}

Spr::Vec3f CRActionPlanner::ContactPointOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene){
	Spr::CDCollisionEngine* ce = NULL;
	Spr::CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		return fp->intersections.begin()->convexPair->commonPoint;
	}
	return Spr::Vec3f();
}

bool CRActionPlanner::ContactCheckOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene){
	Spr::CDCollisionEngine* ce = NULL;
	Spr::CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	//return (fp->intersections.size() > 0);
	return (fp->lastContactCount == scene->GetCount() - 1);
}

bool CRActionPlanner::ContactCheckOfSolid(Spr::PHSolid* so, Spr::CRHuman* human, Spr::SGScene* scene){
	Spr::Vec3f rforce = Spr::Vec3f(0,0,0);
	for(int i = 0; i < human->solids.size(); ++i){
		if(ContactCheckOfSolidPair(so, human->solids[i], scene)) return true;
	}
	return false;
}

void CRActionPlanner::ChooseTargetAction(){
	int cType;
	ContactInfo* cInfo = NULL;

	for(int i = 0, count = contactInfo.size(); i < contactInfo.size(); ++i){
		cType = contactInfo[i].contactType;
		if(cType > 0 && cType < 4){
			cInfo = &contactInfo[i];
			count = contactInfo[i].firstCount;
		}
		if(count < contactInfo[i].firstCount) break;
	}

	if(cInfo){
		switch(cType){
		case 1:	// ���[�U�̍U������Ƀq�b�g���Ă��܂��ꍇ
			gdm.type = GoalDirectedMovement::GUARD;
			//gdm.type = GoalDirectedMovement::AVOID;
			break;

		case 2:	// ���[�U�ւ̍U���q�b�g����ꍇ
			gdm.type = GoalDirectedMovement::ATTACK;
			gdm.SetGoalDirectedMovement(cInfo->soVH, cInfo->soUser->GetCenterPosition(), cInfo->firstTime - startTime);
			break;

		case 3:	// ���[�U�̍U�����K�[�h����ꍇ
			gdm.type = GoalDirectedMovement::GUARD;
			gdm.SetGoalDirectedMovement(cInfo->soVH, cInfo->contactPoint[0], cInfo->firstTime - startTime);
			break;

		default:	// ���̑��̏ꍇ
			if(gdm.type != GoalDirectedMovement::AVOID) gdm.type = GoalDirectedMovement::ATTACK;
			break;
		}
		
		//DSTR << cInfo->soVH->GetName() << " collided with " << cInfo->soUser->GetName() << std::endl;
		//DSTR << "VH chooses " << gdm.type << " action." << std::endl;
	}
}

/*void CRActionPlanner::SetAttackGDM(ContactInfo* cInfo, int type){

}*/
