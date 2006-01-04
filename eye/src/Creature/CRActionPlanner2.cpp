// CRActionPlanner2.cpp: CRActionPlanner2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRActionPlanner2.h"

namespace Spr{

	void CRActionPlanner2::Load(SGScene* rScene, SGScene* iScene, CRPuppet* rPuppet, CRPuppet* iPuppet, CRUser *iUser, CRInternalModel* internalModel){
		/*
		CRActionPlanner::Load();
		this->rScene = rScene;
		this->iScene = iScene;
		this->rPuppet = rPuppet;
		this->iPuppet = iPuppet;
		this->iUser = iUser;
		this->internalModel = internalModel;
		*/
	}
	
	void CRActionPlanner2::Step(bool bInternalModelStable){
		if(iPuppet->IsLoaded() && iUser->IsLoaded()){
			if(bPlanner){
				Spr::GRRender* render;
				iScene->GetRenderers().Find(render);
				if(bPrediction){
					PredictionAction(iPuppet, iUser, iScene);
				
					// 予測軌道を描画
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

	void CRActionPlanner2::PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene){
		const int SETCOUNT = 10;
		
		if(times >= SETCOUNT){
			contactInfo.clear();
			bPrediction = false;
			internalModel->EnableVisualUpdate();
			// 内部到達運動の初期化
			for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
			cPos = Vec3f();
			return;
		}
		
		if(ChooseTargetAction(rPuppet, user)){ // rPuppetの行動を決定
			contactInfo.clear();
			bPrediction = false;
			internalModel->EnableVisualUpdate();
			// 内部到達運動の初期化
			for(int i = 0; i < 3; ++i) puppet->reaching[1][i].Init();
			return;
		}
		
		currentTime = scene->GetCount() * scene->GetTimeStep();	// 現在の時刻

		if(times == 0){
			contactInfo.clear();
			startTime = currentTime;
			
			// 到達運動のコピー(rPuppetからiPuppetへ)
			//for(int i = 0; i < 3; ++i) puppet->reaching[1][i] = rPuppet->reaching[0][i];
		}
		
		MovementPrediction(puppet, user, scene, times);
		
		times++;
	}

	void CRActionPlanner2::MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count){
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

	bool CRActionPlanner2::ChooseTargetAction(CRPuppet* puppet, CRUser* user){
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

				// 右腕の行動
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

					// 左腕の行動
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


}

