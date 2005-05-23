#include "Creature.h"
#pragma hdrstop
// CRActionPlanner.cpp: CRActionPlanner クラスのインプリメンテーション
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

void CRActionPlanner::Step(CRPuppet* puppet, CRUser* user, SGScene* scene){
	if(puppet->IsLoaded() && user->IsLoaded()){
		if(bPlanner){
			Spr::GRRender* render;
			scene->GetRenderers().Find(render);
			//user->locus.PrintLogPos(user);
			if(bPrediction){
				PredictionAction(puppet, user, scene);

				// 予測軌道を描画
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
//					DSTR << "予測開始: " << scene->GetCount() * scene->GetTimeStep() << std::endl;
					//user->locus.PrintLocusCoefficient(user);
					bPrediction = true;
					times = 0;
				}
			}
			if(cPos.norm() > 0){
				// 接触予想位置を表示
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
		// 内部到達運動の初期化
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
//		DSTR << "時間切れ: " << scene-> GetTimeStep() * scene->GetCount() << "\n" << std::endl;
		cPos = Vec3f();
		return;
	}

	if(ChooseTargetAction(puppet, user)){
		contactInfo.clear();
		bPrediction = false;
		// 内部到達運動の初期化
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
//		DSTR << "予想接触位置: " << cPos << std::endl;
//		DSTR << "予測完了: " << scene-> GetTimeStep() * scene->GetCount() << "\n" << std::endl;
		return;
	}

	currentTime = scene->GetCount() * scene->GetTimeStep();	// 現在の時刻
	SaveState(scene, 1);		// 現在の状態を保存
	if(times == 0){
		SaveState(scene, 0);	// 初期内部状態を保存
		contactInfo.clear();
		startTime = currentTime;

		// 到達運動のコピー
		for(int i = 0; i < 3; ++i) puppet->reaching[1][i] = puppet->reaching[0][i];
	}

	MovementPrediction(puppet, user, scene, times);

	LoadState(scene, 1);		// 現在の状態を読み込む

	times++;
}

void CRActionPlanner::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
	const int STEPCOUNT = 4;
	const float RATE = 1.25f;
	
	LoadState(scene, 0);	// 途中内部状態から再開
	float dt = scene->GetTimeStep();
	scene->SetTimeStep(dt * RATE);

	for(int i = 0; i < STEPCOUNT; i++){
		PredictionStep(puppet, user, scene, count * STEPCOUNT + i);
		ContactTest(puppet, user, scene, count * STEPCOUNT + i);
	}

	// 更新結果を描画
	GRRender* render;
	scene->GetRenderers().Find(render);
#ifdef USE_VHCHECKDLG
	vhCheckDialog->DrawTest(render, scene);
#endif

	scene->SetTimeStep(dt);
	SaveState(scene, 0);	// 途中状態を保存
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
			// 前回も接触していたか判定
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
					// ２つの剛体の接触判定
					if(puppet1->humanContactInfo.ContactCheckOfSolidPair(puppet1->solids[i], puppet2->solids[j], scene)){
						// 前回も接触していたか判定
						if(IsFirstContact(i, j, step)){
							int type = cInfo.CheckContactType(i,j);
							if(type != 0){
								float time = startTime + step * scene->GetTimeStep();
								cInfo.SetContactInfo(i, j, step, time);
								puppet1->humanContactInfo.SetContactPointOfSolidPair(puppet1->solids[i], puppet2->solids[j], scene, cInfo.contactPoint);
								cInfo.soVH   = puppet1->solids[i];
								cInfo.soUser = puppet2->solids[j];
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
		case 1:	// ユーザの攻撃が先にヒットしてしまう場合
			// 体の行動
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

			// 右腕の行動
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

				// 左腕の行動
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

