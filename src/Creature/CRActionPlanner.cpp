// CRActionPlanner.cpp: CRActionPlanner �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRActionPlanner.h"
#include <Graphics/GRRender.h>

#include "../../../Experiments/VH/VHInteraction/VHCheckDialog.h"

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
	bPlanner    = false;
	bPrediction = false;
	bLoadReal   = false;
	bLoadTemp   = false;
	times     = 0;
	startTime = 0;
}
	
void CRActionPlanner::SaveState(SGScene* scene, bool type){
	if(type){
		stateReal.clear();
		scene->SaveState(stateReal);	// type == 1 �̎��͌��݂̏�ԂƂ��ĕۑ�
		bLoadReal = true;
	}
	else{
		stateTemp.clear();
		scene->SaveState(stateTemp);	// type == 0 �̎��͓����̏�ԂƂ��ĕۑ�
		bLoadTemp = true;
	}
}

void CRActionPlanner::LoadState(SGScene* scene, bool type){
	if(type){
		if(bLoadReal){
			scene->LoadState(stateReal);	// type == 1 �̎��͌��݂̏�Ԃ�ǂݍ���
			stateReal.clear();
		}
	}
	else{
		if(bLoadTemp){
			scene->LoadState(stateTemp);	// type == 0 �̎��͓����̏�Ԃ�ǂݍ���
			stateTemp.clear();
		}
	}
}

void CRActionPlanner::PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene){
	const int SETCOUNT =  20;

	if(times >= SETCOUNT){
		contactInfo.clear();
		bPrediction = false;
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i].Init();
			user->rm2[i].Init();
		}
		return;
	}

	if(ChooseTargetAction(puppet, user)){
		contactInfo.clear();
		bPrediction = false;
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i].Init();
			user->rm2[i].Init();
		}
		return;
	}

	currentTime = scene->GetCount() * scene->GetTimeStep();	// ���݂̎���
	SaveState(scene, 1);		// ���݂̏�Ԃ�ۑ�
	if(times == 0){
		SaveState(scene, 0);	// ����������Ԃ�ۑ�
		contactInfo.clear();
		startTime = currentTime;

		// ���B�^���̃R�s�[
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i] = puppet->rm[i];
			user->rm2[i]   = user->rm[i];
		}
	}

	MovementPrediction(puppet, user, scene, times);

	LoadState(scene, 1);		// ���݂̏�Ԃ�ǂݍ���

	times++;
}

void CRActionPlanner::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	const int STEPCOUNT = 3;
	const float RATE = 2.0f;
	
	LoadState(scene, 0);	// �r��������Ԃ���ĊJ
	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		Step(puppet, user, scene);
		ContactTest(puppet, user, scene, count * STEPCOUNT + i);
	}

	// �X�V���ʂ�`��
	GRRender* render;
	scene->GetRenderers().Find(render);
	vhCheckDialog->DrawTest(render, scene);

	scene->SetTimeStep(dt);
	SaveState(scene, 0);	// �r����Ԃ�ۑ�
}

void CRActionPlanner::Step(CRPuppet* puppet, CRUser* user, SGScene* scene){
	float dt = scene->GetTimeStep();

	scene->ClearForce();
	scene->GenerateForce();
	
	puppet->SetExpectedPos(dt);
	puppet->SetSpringForce(dt);
	user->SetExpectedPos(dt);
	user->SetSpringForce(dt);

	for(int i = 0; i < 3; ++i){
		puppet->rm2[i].Step(dt);
		user->rm2[i].Step(dt);
	}
	
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

void CRActionPlanner::ContactTest(CRHuman* human1, CRHuman* human2, SGScene* scene, int step){
	ContactInfo cInfo;
	for(int i = 0; i < human1->solids.size(); ++i){
		if(human1->solids[i]){
			for(int j = 0; j < human2->solids.size(); ++j){
				if(human2->solids[j]){
					// �Q�̍��̂̐ڐG����
					if(ContactCheckOfSolidPair(human1->solids[i], human2->solids[j], scene)){
						// �O����ڐG���Ă���������
						if(IsFirstContact(i, j, step)){
							int type = cInfo.CheckContactType(i,j);
							if(type != 0){
								float time = startTime + step * scene->GetTimeStep();
								cInfo.SetContactInfo(i, j, step, time);
								SetContactPointOfSolidPair(human1->solids[i], human2->solids[j], scene, cInfo.contactPoint);
								cInfo.soVH   = human1->solids[i];
								cInfo.soUser = human2->solids[j];
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

void CRActionPlanner::SetContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene, Vec3f* pos){
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		pos[0] = fp->intersections.begin()->convexPair->commonPoint;		// �O���[�o�����W�n
		pos[1] = fp->intersections.begin()->convexPair->closestPoint[0];	// soVH ���W�n
		pos[2] = fp->intersections.begin()->convexPair->closestPoint[1];	// soUser ���W�n
	}
}

Vec3f CRActionPlanner::ContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;
	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp->intersections.size() > 0){
		return fp->intersections.begin()->convexPair->commonPoint;
	}
	return Vec3f();
}

bool CRActionPlanner::ContactCheckOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
	if(so2 == NULL) return false;
	CDCollisionEngine* ce = NULL;
	CDFramePair* fp = NULL;

	scene->GetBehaviors().Find(ce);
	fp = ce->GetFramePair(so1->GetFrame(), so2->GetFrame());
	if(fp == NULL) fp = ce->GetFramePair(so2->GetFrame(), so1->GetFrame());

	//return (fp->intersections.size() > 0);
	return (fp->lastContactCount == scene->GetCount() - 1);
}

bool CRActionPlanner::ContactCheckOfSolid(PHSolid* so, CRHuman* human, SGScene* scene){
	if(so == NULL) return false;
	Vec3f rforce = Vec3f(0,0,0);
	for(int i = 0; i < human->solids.size(); ++i){
		if(ContactCheckOfSolidPair(so, human->solids[i], scene)) return true;
	}
	return false;
}

Vec3f CRActionPlanner::GetContactForceOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene){
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

Vec3f CRActionPlanner::GetContactForceOfSolid(PHSolid* so, CRHuman* human, SGScene* scene){
	Vec3f rforce = Vec3f(0,0,0);
	if(so == NULL) return rforce;
	for(int i = 0; i < human->solids.size(); ++i){
		rforce += GetContactForceOfSolidPair(human->solids[i], so, scene);
	}
	return rforce;
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
		switch(cType){
		case 1:	// ���[�U�̍U������Ƀq�b�g���Ă��܂��ꍇ
			//DSTR << "OH! No! " << "�ڐG�܂Ŏc�� " <<  cInfo->firstTime - currentTime << std::endl;

			if(cInfo->firstTime - currentTime < 0.02f){
				//DSTR << "����͂�����߂�" << std::endl;
				//puppet->rm[0].Init();
				//puppet->rm[1].Init();
				//puppet->rm[2].Init();
				/*if(cInfo->soVHIndex == 3){
					puppet->rm[2].SetSpring(cInfo->soVH, Vec3f());
					Vec3f p = (cInfo->soVH->GetCenterPosition() - cInfo->contactPoint[0]).unit() * 0.05f + cInfo->soVH->GetCenterPosition();
					puppet->rm[2].SetTargetPos(p, Vec3f());
					puppet->rm[2].SetTimer(0.01f, 0.0f);
					puppet->rm[2].SetType(3);
					//DSTR << "��͔�����I" << std::endl;
				}*/
				contactInfo.clear();
				return true;
			}
			// �̂̍s��
			if(cInfo->soVHIndex == 3){
				if(puppet->rm[2].state == 0){
					puppet->rm[2].SetSpring(cInfo->soVH, Vec3f());
					Vec3f p = GetPointToAvoid(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[2].GetPos(), 0.25f);
					puppet->rm[2].SetTargetPos(p, Vec3f());
					puppet->rm[2].SetTimer(cInfo->firstTime - currentTime, 0.05f);
					puppet->rm[2].SetType(3);
					//DSTR << "��͔�����I" << std::endl;
				}
				else if(puppet->rm[2].state == 3){
					//puppet->rm[2].Init();
					if(puppet->rm[0].state == 0){
						puppet->rm[0].SetSpring(puppet->solids[6], Vec3f());
						Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[0].GetPos());
						puppet->rm[0].SetTargetPos(p, Vec3f());
						puppet->rm[0].SetTimer((cInfo->firstTime - currentTime) * 0.5, (cInfo->firstTime - currentTime) * 0.5);
						puppet->rm[0].SetType(2);
					}
					else if(puppet->rm[0].state == 1){
						puppet->rm[0].Init();
					}
					if(puppet->rm[1].state == 0){
						puppet->rm[1].SetSpring(puppet->solids[9], Vec3f());
						Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[1].GetPos());
						puppet->rm[1].SetTargetPos(p, Vec3f());
						puppet->rm[1].SetTimer((cInfo->firstTime - currentTime) * 0.5, (cInfo->firstTime - currentTime) * 0.5);
						puppet->rm[1].SetType(2);
					}
				}
			}

			else if(cInfo->soVHIndex == 2){
				// �E�r�̍s��
				if(puppet->rm[0].state == 1){
					puppet->rm[0].Init();
					//DSTR << "�E�r�͋x�߁I" << std::endl;
				}
				else if(puppet->rm[0].state == 0){
					puppet->rm[0].SetSpring(puppet->solids[5], Vec3f(0,-0.8 * puppet->GetSolidInfo(5).scale.Y(),0));
					Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[0].GetPos());
					puppet->rm[0].SetTargetPos(p, Vec3f());
					puppet->rm[0].SetTimer((cInfo->firstTime - currentTime) * 0.7, (cInfo->firstTime - currentTime) * 0.4);
					puppet->rm[0].SetType(2);

					//DSTR << "�E�r�͎��I" << std::endl;
				}
				else if(puppet->rm[0].state == 2){
					//puppet->rm[0].SetTarget(cInfo->contactPoint[0], Vec3f());
					//DSTR << "�E�r�͂��̂܂܁I" << std::endl;
				}

				// ���r�̍s��
				if(puppet->rm[1].state == 1){
					puppet->rm[1].Init();
					//DSTR << "���r�͋x�߁I" << std::endl;
				}
				else if(puppet->rm[1].state == 0){
					puppet->rm[1].SetSpring(puppet->solids[8], Vec3f(0,-0.8 * puppet->GetSolidInfo(8).scale.Y(),0));
					Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[1].GetPos());
					puppet->rm[1].SetTargetPos(p, Vec3f());
					puppet->rm[1].SetTimer((cInfo->firstTime - currentTime) * 0.7, (cInfo->firstTime - currentTime) * 0.4);
					puppet->rm[1].SetType(2);

					//DSTR << "���r�͎��I" << std::endl;
				}
				else if(puppet->rm[1].state == 2){
					//puppet->rm[1].SetTarget(cInfo->contactPoint[0], Vec3f());
					//DSTR << "���r�͂��̂܂܁I" << std::endl;
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
	Vec3f p = GetNearestPoint(a, (b - a).unit() * 0.25f + b, c);
	Vec3f q = (c - p);
	q.y = 0.0f;
	if(q.Z() > 0) q.z = 0.0f;
	return d * q.unit() + p;
}

Vec3f CRActionPlanner::GetPointToGuard(Vec3f a, Vec3f b, Vec3f c){
	return GetNearestPoint(a, (b - a) * 0.7f + a, c);
}

}

