#include "Creature.h"
#pragma hdrstop
// CRActionPlanner.cpp: CRActionPlanner �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRActionPlanner.h"
#include <Graphics/GRRender.h>

#ifdef USE_VHCHECKDLG
#include "../../../Experiments/VH/VHInteraction/VHCheckDialog.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

//////////////////// �ڐG���N���X ////////////////////
void CRActionPlanner::ContactInfo::SetContactInfo(int vh, int user, int c, float t){
	soVHIndex = vh;
	soUserIndex = user;
	firstCount = lastCount = c;
	firstTime = t;
}

int CRActionPlanner::ContactInfo::CheckContactType(int vh, int user){
	// ���ʂ̐ڐG
	int type = 0;

	if(user == 6 || user == 9){
		// ���[�U�̍U�����q�b�g
		if(vh == 2 || vh == 3) type = 1;
		// ���[�U�̍U�����K�[�h
		if(vh == 5 || vh == 6 || vh == 8 || vh == 9) type = 3;
	}
	if(vh == 6 || vh == 9){
		// VH �̍U�����q�b�g
		if(user == 2 || user == 3) type = 2;
		// VH �̍U�����K�[�h
		if(user == 5 || user == 8) type = 4;
	}

	return type;
}

//////////////////// �\���s���N���X ////////////////////
CRActionPlanner::CRActionPlanner(){
}
	
void CRActionPlanner::Load(SGScene* rScene, SGScene* iScene, CRPuppet* rPuppet, CRPuppet* iPuppet, CRUser *iUser, CRInternalModel* internalModel){
	bDraw       = false;
	bPlanner    = false;
	bPrediction = false;
	bLoadReal   = false;
	bLoadTemp   = false;
	times     = 0;
	startTime = 0;
	this->rScene = rScene;
	this->iScene = iScene;
	this->rPuppet = rPuppet;
	this->iPuppet = iPuppet;
	this->iUser = iUser;
	this->internalModel = internalModel;
}
	
void CRActionPlanner::SaveState(SGScene* scene, SceneType type){
	switch(type){
	case STReal:
		stateReal.clear();
		scene->SaveState(stateReal);	// type == 1 �̎��͌��݂̏�ԂƂ��ĕۑ�
		bLoadReal = true;
		break;
	case STInternal:
		stateTemp.clear();
		scene->SaveState(stateTemp);	// type == 0 �̎��͓����̏�ԂƂ��ĕۑ�
		bLoadTemp = true;
		break;
	}
}

void CRActionPlanner::LoadState(SGScene* scene, SceneType type){
	switch(type){
	case STReal:
		if(bLoadReal){
			scene->LoadState(stateReal);	// type == 1 �̎��͌��݂̏�Ԃ�ǂݍ���
			stateReal.clear();
		}
		break;
	case STInternal:
		if(bLoadTemp){
			scene->LoadState(stateTemp);	// type == 0 �̎��͓����̏�Ԃ�ǂݍ���
			stateTemp.clear();
		}
		break;
	}
}

void CRActionPlanner::Step(bool bInternalModelStable){
	if(iPuppet->IsLoaded() && iUser->IsLoaded()){
		if(bPlanner){
			Spr::GRRender* render;
			iScene->GetRenderers().Find(render);
			if(bPrediction){
				PredictionAction(iPuppet, iUser, iScene);
			
				// �\���O����`��
				if(bDraw){
					for(int i = 0; i < 2; ++i){
						if(iUser->locus.bPrediction[i]){
							iUser->locus.DrawFutureLocus(iUser->locus.ak[i], 10, 10, render, Vec4f(0,1,1,1));
						}
					}
				}
			}
			else{
				iUser->locus.SetLocusCoefficient(iUser, iScene);
				if(iPuppet->IsAimed(iUser, iScene) && /*/ bInternalModelStable /*/ true /**/){
					bPrediction = true;
					times = 0;
					internalModel->DisableVisualUpdate();
				}
			}
			if(cPos.norm() > 0){
				// �ڐG�\�z�ʒu��\��
				if(bDraw){
					render->SetModelMatrix(Affinef());
					render->SetLineWidth(25);
					render->SetMaterial(GRMaterialData(Vec4f(1,0,0,1)));
					Vec3f pos = cPos;
					render->DrawDirect(GRRender::POINTS, &pos, &pos+1);
				}
			}
		}
	}
}

void CRActionPlanner::PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene){
	const int SETCOUNT = 10;
	
	if(times >= SETCOUNT){
		contactInfo.clear();
		bPrediction = false;
		internalModel->EnableVisualUpdate();
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
		cPos = Vec3f();
		return;
	}
	
	if(ChooseTargetAction(rPuppet, user)){ // rPuppet�̍s��������
		contactInfo.clear();
		bPrediction = false;
		internalModel->EnableVisualUpdate();
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
		return;
	}
	
	currentTime = scene->GetCount() * scene->GetTimeStep();	// ���݂̎���

	if(times == 0){
		contactInfo.clear();
		startTime = currentTime;
		
		// ���B�^���̃R�s�[(rPuppet����iPuppet��)
		//for(int i = 0; i < 3; ++i) puppet->reaching[1][i] = rPuppet->reaching[0][i];
	}
	
	MovementPrediction(puppet, user, scene, times);
	
	times++;
}

void CRActionPlanner::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	const int STEPCOUNT = 4;
	const float RATE = 1.0f;
	//const float RATE=1.0f;

	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		PredictionStep(puppet, user, scene, count * STEPCOUNT + i);
		ContactTest(puppet, user, scene, count * STEPCOUNT + i);
	}

	scene->SetTimeStep(dt);
}


void CRActionPlanner::PredictionStep(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	float dt = scene->GetTimeStep();

	scene->ClearForce();
	scene->GenerateForce();
	
	puppet->SetSpringForce(dt);
	if(user->locus.degree == 1) user->SetExpectedPos(3.0f*count, dt);
	else                        user->SetExpectedPos(count, dt);
	user->SetSpringForce(dt);

	for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Step(scene);
	
	scene->Integrate();
}

bool CRActionPlanner::IsFirstContact(int vh, int user, int count){
	for(int i = 0; i < contactInfo.size(); ++i){
		if(contactInfo[i].soVHIndex == vh && contactInfo[i].soUserIndex == user){
			// �O����ڐG���Ă���������
			if(contactInfo[i].lastCount == count - 1){
				contactInfo[i].lastCount = count;
				return false;
			}
		}
	}
	return true;
}

void CRActionPlanner::ContactTest(CRPuppet* puppet1, CRPuppet* puppet2, SGScene* scene, int step){
	ContactInfo cInfo;
	for(int i = 0; i < puppet1->solids.size(); ++i){
		if(puppet1->solids[i]){
			for(int j = 0; j < puppet2->solids.size(); ++j){
				if(puppet2->solids[j]){
					// �Q�̍��̂̐ڐG����
					if(puppet1->humanContactInfo.ContactCheckOfSolidPair(puppet1->solids[i], puppet2->solids[j], scene)){
						// �O����ڐG���Ă���������
						if(IsFirstContact(i, j, step)){
							int type = cInfo.CheckContactType(i,j);
							if(type != 0){
								float time = startTime + step * scene->GetTimeStep();
								cInfo.SetContactInfo(i, j, step, time);
								puppet1->humanContactInfo.SetContactPointOfSolidPair(puppet1->solids[i], puppet2->solids[j], scene, cInfo.contactPoint);
								cInfo.soVH   = puppet1->solids[i];
								cInfo.soUser = puppet2->solids[j];
								cInfo.contactType = type;
								// �ڐG����o�^
								contactInfo.push_back(cInfo);
							}
						}
					}
				}
			}
		}
	}
}

bool CRActionPlanner::ChooseTargetAction(CRPuppet* puppet, CRUser* user){
	int cType;
	ContactInfo* cInfo = NULL;

	for(int i = 0, count = contactInfo.size(); i < contactInfo.size(); ++i){
		cType = contactInfo[i].contactType;
		if(cType > 0 && cType < 2){
			cInfo = &contactInfo[i];
			count = contactInfo[i].firstCount;
			if(cType == 1) break;
		}
		if(count < contactInfo[i].firstCount) break;
	}

	if(cInfo){
		//DSTR << "Contact Solid: " << cInfo->soVHIndex << std::endl;
		cPos = cInfo->contactPoint[0];
		float tTime = cInfo->firstTime - currentTime;

		switch(cType){
		case 1:	// ���[�U�̍U������Ƀq�b�g���Ă��܂��ꍇ
			// �̂̍s��
			//if(cInfo->soVHIndex == 3){
			if(cInfo->soVHIndex == 3 || cInfo->soVHIndex == 2){
				//float rate = 0.2f;
				//if(tTime > rate) rate = tTime;

				float rate = 0.1f;
				if(tTime > rate) rate = tTime * 0.5;
				
				PHSolid* avoidSolid;
				rScene->FindObject(avoidSolid, cInfo->soVH->GetName());

				puppet->reaching[0][2].SetSpring(avoidSolid, Vec3f(0.02f, 0.02f, 0.02f));
				Vec3f p = GetPointToAvoid(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][2].GetPos(), 0.25f);
				puppet->reaching[0][2].SetTargetPos(p, Vec3f());
				puppet->reaching[0][2].SetTimer(rate, 0.1f);
				puppet->reaching[0][2].SetLimitForce(1e10f);
				puppet->reaching[0][2].SetType(3);
				puppet->reaching[0][2].SetSprRate(0.7f);
				puppet->reaching[0][2].SetDmpRate(0.7f);
			}

			// �E�r�̍s��
			if(puppet->bGuard){
			//else{
				if(puppet->reaching[0][0].state == 1){
					puppet->reaching[0][0].Init();
				}
				else{
					float rate = 0.05f;
//						if(tTime > 0.2f) rate = tTime * 0.5f;

					if((puppet->solids[5]->GetCenterPosition() - cPos).norm() > (puppet->solids[6]->GetCenterPosition() - cPos).norm()){
						puppet->reaching[0][0].SetSpring(puppet->solids[5], Vec3f(0.0f,-0.9f * puppet->GetSolidInfo(5).scale.Y(),0));
					}
					else{
						puppet->reaching[0][0].SetSpring(puppet->solids[5], Vec3f(0.0f,-0.5f * puppet->GetSolidInfo(5).scale.Y(),0));
					}
					Vec3f p = GetPointToGuard(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][0].GetPos());
					//p += Vec3f(0.0f,0.1f,0.0f);
					puppet->reaching[0][0].SetTargetPos(p, Vec3f());
					//puppet->reaching[0][0].SetTimer(rate, 0.4f - rate);
					puppet->reaching[0][0].SetTimer(rate, 0.0f);
					puppet->reaching[0][0].SetLimitForce(1000000000000.0f);//1000.0f);
					puppet->reaching[0][0].SetType(2);
					puppet->reaching[0][0].SetSprRate(0.8f*4.0f);
					puppet->reaching[0][0].SetDmpRate(0.8f*4.0f);
				}

				// ���r�̍s��
				if(puppet->reaching[0][1].state == 1){
					puppet->reaching[0][1].Init();
				}
				else{
					float rate = 0.05f;
//						if(tTime > 0.2f) rate = tTime * 0.5f;

					if((puppet->solids[8]->GetCenterPosition() - cPos).norm() > (puppet->solids[9]->GetCenterPosition() - cPos).norm()){
						puppet->reaching[0][1].SetSpring(puppet->solids[8], Vec3f(0.0f,-0.9f * puppet->GetSolidInfo(8).scale.Y(),0));
					}
					else{
						puppet->reaching[0][1].SetSpring(puppet->solids[8], Vec3f(0.0f,-0.5f * puppet->GetSolidInfo(8).scale.Y(),0));
					}
					Vec3f p = GetPointToGuard(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][0].GetPos());
					//p += Vec3f(0.0f,0.1f,0.0f);
					puppet->reaching[0][1].SetTargetPos(p, Vec3f());
					//puppet->reaching[0][1].SetTimer(rate, 0.4f - rate);
					puppet->reaching[0][1].SetTimer(rate, 0.0f);
					puppet->reaching[0][1].SetLimitForce(1000000000000.0f);//1000.0f);
					puppet->reaching[0][1].SetType(2);
					puppet->reaching[0][1].SetSprRate(0.8f*4.0f);
					puppet->reaching[0][1].SetDmpRate(0.8f*4.0f);
				}
			}

			contactInfo.clear();
			return true;
			break;

		//case 2:	// ���[�U�ւ̍U���q�b�g����ꍇ
		//	contactInfo.clear();
		//	DSTR << "Attack !" << std::endl;
		//	break;

		//case 3:	// ���[�U�̍U�����K�[�h����ꍇ
		//	break;

		default:	// ���̑��̏ꍇ
			//contactInfo.clear();
			//DSTR << "Normal !" << std::endl;
			break;
		}
		
		//DSTR << cInfo->soVH->GetName() << " collided with " << cInfo->soUser->GetName() << std::endl;
		//DSTR << "VH chooses " << gdm.type << " action." << std::endl;
		//contactInfo.clear();
		return false;
	}
	else{
		//contactInfo.clear();
		//times = 0;
		return false;
	}
}


Vec3f CRActionPlanner::GetNearestPoint(Vec3f a, Vec3f b, Vec3f c){
	// ����ab��œ_c�Ƃ̍ŋߖT�_��Ԃ�
	Vec3f p;
	float t;
	t = (b - a) * (c - a) / ((b - a).norm() * (b - a).norm());
	if(t < 0) t = 0;
	if(t > 1) t = 1;
	p = (b - a) * t + a;
	return p;
}

Vec3f CRActionPlanner::GetPointToAvoid(Vec3f a, Vec3f b, Vec3f c, float d){
	Vec3f p = GetNearestPoint(a, (b - a).unit() * 0.3f + b, c);
	Vec3f q = (c - p);
	q.y *= 0.0f;
	q.z *= 0.1f;
	//if(q.Z() > 0) q.z = 0.0f;
	//return d * q.unit() + p;
	return d * q.unit() + c;
}

Vec3f CRActionPlanner::GetPointToGuard(Vec3f a, Vec3f b, Vec3f c){
	//return GetNearestPoint(a, (b - a) * 0.4f + a, c);
	return GetNearestPoint(a, (a - b).unit() * 0.05f + b, c);
}

void CRActionPlanner::SetSolidInvisible(PHSolid* solid){
	for(int i=0; i<contactDisabledSolids.size(); i++){
		if (contactDisabledSolids[i]==solid){
			return;
		}
	}
	contactDisabledSolids.push_back(solid);
}

void CRActionPlanner::SetSolidVisible(PHSolid* solid){
	for(std::vector<PHSolid*>::iterator it=contactDisabledSolids.begin();
			it != contactDisabledSolids.end();
			it++)
		{
		if (*it==solid){
			contactDisabledSolids.erase(it);
			return;
		}
	}
}

void CRActionPlanner::DisableContact(CRPuppet* puppet, SGScene* scene){
	PHContactEngine* contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);
	int pos = contactEngine->GetListenerPos();

	CDCollisionEngine* collisionEngine = NULL;
	scene->GetBehaviors().Find(collisionEngine);

	for(int i=0; i<puppet->solids.size(); i++){
		for(int j=0; j<contactDisabledSolids.size(); j++){
			collisionEngine->AddInactive(
																	 puppet->solids[i]->GetFrame(),
																	 contactDisabledSolids[j]->GetFrame(),
																	 pos
																	 );
		}
	}
}
	
void CRActionPlanner::EnableContact(CRPuppet* puppet, SGScene* scene){
	PHContactEngine* contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);
	int pos = contactEngine->GetListenerPos();

	CDCollisionEngine* collisionEngine = NULL;
	scene->GetBehaviors().Find(collisionEngine);

	for(int i=0; i<puppet->solids.size(); i++){
		for(int j=0; j<contactDisabledSolids.size(); j++){
			collisionEngine->AddActive(
																	 puppet->solids[i]->GetFrame(),
																	 contactDisabledSolids[j]->GetFrame(),
																	 pos
																	 );
		}
	}
}


}

