// CRActionPlanner.h: CRActionPlanner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#include <Creature/CRHuman.h>
#include <Creature/CRUser.h>
#include <Graphics/GRRender.h>
#include <vector>
#include <Physics/PHContactEngine.h>
#include <SceneGraph/SGScene.h>

#if !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
#define AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRActionPlanner{
	// 到達運動(仮)
	class GoalDirectedMovement{
	public:
		Spr::PHSolid* solid;		// 対象Solid
		Spr::Vec3f r;				// Solidローカル座標
		Spr::Vec3f target, temp;	// 目標位置(仮目標)
		float end, period;			// 目標時刻(目標時間)
		enum Type{
			STANDBY,
			ATTACK,
			GUARD,
			AVOID,
		} type;

		GoalDirectedMovement();
		void SetGoalDirectedMovement(Spr::PHSolid* so, Spr::Vec3f pos, float time){
			solid = so;
			target = pos;
			end = time;
		}
		void AddForce(float time);
	};
	
	// 接触情報
	class ContactInfo{
	public:
		int firstCount, lastCount;	// 初接触時刻(Count)
		float firstTime;			// 初接触時刻
		int soVHIndex, soUserIndex;	// 接触したSolid番号
		Spr::PHSolid* soVH;
		Spr::PHSolid* soUser;
		Spr::Vec3f contactPoint[3];	// 接触点(グローバル, soVHローカル, soUserローカル)
		int contactType;			// 接触タイプ
	
		ContactInfo(){};
		void SetContactInfo(int vh, int user, int c, float t);
		void SetContactType(int vh, int user);
	};
	class ContactInfoSequence: public std::vector<ContactInfo>{
	};

public:
	CRActionPlanner();

	void SaveState(Spr::SGScene* scene, bool type);
	void LoadState(Spr::SGScene* scene, bool type);

	void PredictionAction(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene);
	void MovementPrediction(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene, int count);
	void Step(Spr::CRHuman* human, CRUser* user, Spr::SGScene* scene);

	void ContactTest(Spr::CRHuman* human1, Spr::CRHuman* human2, Spr::SGScene* scene, int step);
	bool IsFirstContact(int vh, int user, int count);

	Spr::Vec3f ContactPointOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene);
	void SetContactPointOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene, Spr::Vec3f* pos);
	bool ContactCheckOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene);
	bool ContactCheckOfSolid(Spr::PHSolid* so, Spr::CRHuman* human, Spr::SGScene* scene);

	void ChooseTargetAction();
	//void SetGoalDirectedMovement(Spr::PHSolid* so, Spr::Vec3f pos, float time);
		
	bool bPrediction;
	bool bLoadReal, bLoadTemp;
	int times;
	float startTime;

	Spr::SGBehaviorStates stateReal, stateTemp;
	ContactInfoSequence contactInfo;

	GoalDirectedMovement gdm;
};

#endif // !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
