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
	
void CRActionPlanner::Load(){
	bDraw       = false;
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

void CRActionPlanner::Step(CRPuppet* puppet, CRUser* user, SGScene* scene){
	if(puppet->IsLoaded() && user->IsLoaded()){
		if(bPlanner){
			Spr::GRRender* render;
			scene->GetRenderers().Find(render);
			//user->locus.PrintLogPos(user);
			if(bPrediction){
				PredictionAction(puppet, user, scene);

				// �\���O����`��
				if(bDraw){
					for(int i = 0; i < 2; ++i){
						if(user->locus.bPrediction[i]){
							user->locus.DrawFutureLocus(user->locus.ak[i], 10, 10, render, Vec4f(0,1,1,1));
						}
					}
				}
			}
			else{
				user->locus.SetLocusCoefficient(user, scene);
				if(puppet->IsAimed(user, scene)){
//					DSTR << "�\���J�n: " << scene->GetCount() * scene->GetTimeStep() << std::endl;
					//user->locus.PrintLocusCoefficient(user);
					bPrediction = true;
					times = 0;
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
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
//		DSTR << "���Ԑ؂�: " << scene-> GetTimeStep() * scene->GetCount() << "\n" << std::endl;
		cPos = Vec3f();
		return;
	}

	if(ChooseTargetAction(puppet, user)){
		contactInfo.clear();
		bPrediction = false;
		// �������B�^���̏�����
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
//		DSTR << "�\�z�ڐG�ʒu: " << cPos << std::endl;
//		DSTR << "�\������: " << scene-> GetTimeStep() * scene->GetCount() << "\n" << std::endl;
		return;
	}

	currentTime = scene->GetCount() * scene->GetTimeStep();	// ���݂̎���
	SaveState(scene, 1);		// ���݂̏�Ԃ�ۑ�
	if(times == 0){
		SaveState(scene, 0);	// ����������Ԃ�ۑ�
		contactInfo.clear();
		startTime = currentTime;

		// ���B�^���̃R�s�[
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i] = puppet->reaching[0][i];
	}

	MovementPrediction(puppet, user, scene, times);

	LoadState(scene, 1);		// ���݂̏�Ԃ�ǂݍ���

	times++;
}

void CRActionPlanner::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	const int STEPCOUNT = 4;
	const float RATE = 1.25f;
	
	LoadState(scene, 0);	// �r��������Ԃ���ĊJ
	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		PredictionStep(puppet, user, scene, count * STEPCOUNT + i);
		ContactTest(puppet, user, scene, count * STEPCOUNT + i);
	}

	// �X�V���ʂ�`��
	GRRender* render;
	scene->GetRenderers().Find(render);
#ifdef USE_VHCHECKDLG
	vhCheckDialog->DrawTest(render, scene);
#endif

	scene->SetTimeStep(dt);
	SaveState(scene, 0);	// �r����Ԃ�ۑ�
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
				float rate = 0.2f;
				if(tTime > rate) rate = tTime;

				puppet->reaching[0][2].SetSpring(cInfo->soVH, Vec3f(0.02f, 0.02f, 0.02f));
				Vec3f p = GetPointToAvoid(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][2].GetPos(), 0.25f);
				puppet->reaching[0][2].SetTargetPos(p, Vec3f());
				puppet->reaching[0][2].SetTimer(rate, 0.45f);
				puppet->reaching[0][2].SetType(3);
			}

			// �E�r�̍s��
			if(puppet->bGuard){
			//else{
				if(puppet->reaching[0][0].state == 1){
					puppet->reaching[0][0].Init();
				}
				else{
					float rate = 0.1f;
					if(tTime > 0.2f) rate = tTime * 0.5f;

					if((puppet->solids[5]->GetCenterPosition() - cPos).norm() > (puppet->solids[6]->GetCenterPosition() - cPos).norm()){
						puppet->reaching[0][0].SetSpring(puppet->solids[5], Vec3f(0.0f,-0.9f * puppet->GetSolidInfo(5).scale.Y(),0));
					}
					else{
						puppet->reaching[0][0].SetSpring(puppet->solids[5], Vec3f());
					}
					Vec3f p = GetPointToGuard(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][0].GetPos());
					puppet->reaching[0][0].SetTargetPos(p, Vec3f());
					puppet->reaching[0][0].SetTimer(rate, 0.4f - rate);
					puppet->reaching[0][0].SetType(2);
				}

				// ���r�̍s��
				if(puppet->reaching[0][1].state == 1){
					puppet->reaching[0][1].Init();
				}
				else{
					float rate = 0.1f;
					if(tTime > 0.2f) rate = tTime * 0.5f;

					if((puppet->solids[8]->GetCenterPosition() - cPos).norm() > (puppet->solids[9]->GetCenterPosition() - cPos).norm()){
						puppet->reaching[0][1].SetSpring(puppet->solids[8], Vec3f(0.0f,-0.9f * puppet->GetSolidInfo(8).scale.Y(),0));
					}
					else{
						puppet->reaching[0][1].SetSpring(puppet->solids[8], Vec3f());
					}
					Vec3f p = GetPointToGuard(cInfo->soUser->GetFrame()->GetPosture() * cInfo->contactPoint[2], cPos, puppet->reaching[0][0].GetPos());
					puppet->reaching[0][1].SetTargetPos(p, Vec3f());
					puppet->reaching[0][1].SetTimer(rate, 0.4f - rate);
					puppet->reaching[0][1].SetType(2);
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

}

