// CRActionPlanner.cpp: CRActionPlanner クラスのインプリメンテーション
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
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

//////////////////// 接触情報クラス ////////////////////
void CRActionPlanner::ContactInfo::SetContactInfo(int vh, int user, int c, float t){
	soVHIndex = vh;
	soUserIndex = user;
	firstCount = lastCount = c;
	firstTime = t;
}

int CRActionPlanner::ContactInfo::CheckContactType(int vh, int user){
	// 普通の接触
	int type = 0;

	if(user == 6 || user == 9){
		// ユーザの攻撃がヒット
		if(vh == 2 || vh == 3) type = 1;
		// ユーザの攻撃をガード
		if(vh == 5 || vh == 6 || vh == 8 || vh == 9) type = 3;
	}
	if(vh == 6 || vh == 9){
		// VH の攻撃がヒット
		if(user == 2 || user == 3) type = 2;
		// VH の攻撃をガード
		if(user == 5 || user == 8) type = 4;
	}

	return type;
}

//////////////////// 予測行動クラス ////////////////////
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
		scene->SaveState(stateReal);	// type == 1 の時は現在の状態として保存
		bLoadReal = true;
	}
	else{
		stateTemp.clear();
		scene->SaveState(stateTemp);	// type == 0 の時は内部の状態として保存
		bLoadTemp = true;
	}
}

void CRActionPlanner::LoadState(SGScene* scene, bool type){
	if(type){
		if(bLoadReal){
			scene->LoadState(stateReal);	// type == 1 の時は現在の状態を読み込む
			stateReal.clear();
		}
	}
	else{
		if(bLoadTemp){
			scene->LoadState(stateTemp);	// type == 0 の時は内部の状態を読み込む
			stateTemp.clear();
		}
	}
}

void CRActionPlanner::PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene){
	const int SETCOUNT =  20;

	if(times >= SETCOUNT){
		contactInfo.clear();
		bPrediction = false;
		// 内部到達運動の初期化
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i].Init();
			user->rm2[i].Init();
		}
		return;
	}

	if(ChooseTargetAction(puppet, user)){
		contactInfo.clear();
		bPrediction = false;
		// 内部到達運動の初期化
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i].Init();
			user->rm2[i].Init();
		}
		return;
	}

	currentTime = scene->GetCount() * scene->GetTimeStep();	// 現在の時刻
	SaveState(scene, 1);		// 現在の状態を保存
	if(times == 0){
		SaveState(scene, 0);	// 初期内部状態を保存
		contactInfo.clear();
		startTime = currentTime;

		// 到達運動のコピー
		for(int i = 0; i < 3; ++i){
			puppet->rm2[i] = puppet->rm[i];
			user->rm2[i]   = user->rm[i];
		}
	}

	MovementPrediction(puppet, user, scene, times);

	LoadState(scene, 1);		// 現在の状態を読み込む

	times++;
}

void CRActionPlanner::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	const int STEPCOUNT = 3;
	const float RATE = 2.0f;
	
	LoadState(scene, 0);	// 途中内部状態から再開
	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		Step(puppet, user, scene);
		ContactTest(puppet, user, scene, count * STEPCOUNT + i);
	}

	// 更新結果を描画
	GRRender* render;
	scene->GetRenderers().Find(render);
	vhCheckDialog->DrawTest(render, scene);

	scene->SetTimeStep(dt);
	SaveState(scene, 0);	// 途中状態を保存
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
			// 前回も接触していたか判定
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
					// ２つの剛体の接触判定
					if(ContactCheckOfSolidPair(human1->solids[i], human2->solids[j], scene)){
						// 前回も接触していたか判定
						if(IsFirstContact(i, j, step)){
							int type = cInfo.CheckContactType(i,j);
							if(type != 0){
								float time = startTime + step * scene->GetTimeStep();
								cInfo.SetContactInfo(i, j, step, time);
								SetContactPointOfSolidPair(human1->solids[i], human2->solids[j], scene, cInfo.contactPoint);
								cInfo.soVH   = human1->solids[i];
								cInfo.soUser = human2->solids[j];
								cInfo.contactType = type;
								// 接触情報を登録
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
		pos[0] = fp->intersections.begin()->convexPair->commonPoint;		// グローバル座標系
		pos[1] = fp->intersections.begin()->convexPair->closestPoint[0];	// soVH 座標系
		pos[2] = fp->intersections.begin()->convexPair->closestPoint[1];	// soUser 座標系
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
		case 1:	// ユーザの攻撃が先にヒットしてしまう場合
			//DSTR << "OH! No! " << "接触まで残り " <<  cInfo->firstTime - currentTime << std::endl;

			if(cInfo->firstTime - currentTime < 0.02f){
				//DSTR << "今回はあきらめれ" << std::endl;
				//puppet->rm[0].Init();
				//puppet->rm[1].Init();
				//puppet->rm[2].Init();
				/*if(cInfo->soVHIndex == 3){
					puppet->rm[2].SetSpring(cInfo->soVH, Vec3f());
					Vec3f p = (cInfo->soVH->GetCenterPosition() - cInfo->contactPoint[0]).unit() * 0.05f + cInfo->soVH->GetCenterPosition();
					puppet->rm[2].SetTargetPos(p, Vec3f());
					puppet->rm[2].SetTimer(0.01f, 0.0f);
					puppet->rm[2].SetType(3);
					//DSTR << "顔は避けろ！" << std::endl;
				}*/
				contactInfo.clear();
				return true;
			}
			// 体の行動
			if(cInfo->soVHIndex == 3){
				if(puppet->rm[2].state == 0){
					puppet->rm[2].SetSpring(cInfo->soVH, Vec3f());
					Vec3f p = GetPointToAvoid(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[2].GetPos(), 0.25f);
					puppet->rm[2].SetTargetPos(p, Vec3f());
					puppet->rm[2].SetTimer(cInfo->firstTime - currentTime, 0.05f);
					puppet->rm[2].SetType(3);
					//DSTR << "顔は避けろ！" << std::endl;
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
				// 右腕の行動
				if(puppet->rm[0].state == 1){
					puppet->rm[0].Init();
					//DSTR << "右腕は休め！" << std::endl;
				}
				else if(puppet->rm[0].state == 0){
					puppet->rm[0].SetSpring(puppet->solids[5], Vec3f(0,-0.8 * puppet->GetSolidInfo(5).scale.Y(),0));
					Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[0].GetPos());
					puppet->rm[0].SetTargetPos(p, Vec3f());
					puppet->rm[0].SetTimer((cInfo->firstTime - currentTime) * 0.7, (cInfo->firstTime - currentTime) * 0.4);
					puppet->rm[0].SetType(2);

					//DSTR << "右腕は守れ！" << std::endl;
				}
				else if(puppet->rm[0].state == 2){
					//puppet->rm[0].SetTarget(cInfo->contactPoint[0], Vec3f());
					//DSTR << "右腕はそのまま！" << std::endl;
				}

				// 左腕の行動
				if(puppet->rm[1].state == 1){
					puppet->rm[1].Init();
					//DSTR << "左腕は休め！" << std::endl;
				}
				else if(puppet->rm[1].state == 0){
					puppet->rm[1].SetSpring(puppet->solids[8], Vec3f(0,-0.8 * puppet->GetSolidInfo(8).scale.Y(),0));
					Vec3f p = GetPointToGuard(cInfo->soVH->GetFrame()->GetPosture() * cInfo->contactPoint[2], cInfo->contactPoint[0], puppet->rm[1].GetPos());
					puppet->rm[1].SetTargetPos(p, Vec3f());
					puppet->rm[1].SetTimer((cInfo->firstTime - currentTime) * 0.7, (cInfo->firstTime - currentTime) * 0.4);
					puppet->rm[1].SetType(2);

					//DSTR << "左腕は守れ！" << std::endl;
				}
				else if(puppet->rm[1].state == 2){
					//puppet->rm[1].SetTarget(cInfo->contactPoint[0], Vec3f());
					//DSTR << "左腕はそのまま！" << std::endl;
				}
			}

			contactInfo.clear();
			return true;
			break;

		//case 2:	// ユーザへの攻撃ヒットする場合
		//	contactInfo.clear();
		//	DSTR << "Attack !" << std::endl;
		//	break;

		//case 3:	// ユーザの攻撃をガードする場合
		//	break;

		default:	// その他の場合
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
	// 線分ab上で点cとの最近傍点を返す
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

