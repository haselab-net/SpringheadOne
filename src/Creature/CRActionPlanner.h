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

namespace Spr{;

class CRActionPlanner{
	// 接触情報
	class ContactInfo{
	public:
		int firstCount, lastCount;	// 初接触時刻(Count)
		float firstTime;			// 初接触時刻
		int soVHIndex, soUserIndex;	// 接触したSolid番号
		PHSolid* soVH;
		PHSolid* soUser;
		Vec3f contactPoint[3];		// 接触点(グローバル, soVHローカル, soUserローカル)
		int contactType;			// 接触タイプ
	
		ContactInfo(){};
		void SetContactInfo(int vh, int user, int c, float t);
		int  CheckContactType(int vh, int user);
	};
	class ContactInfoSequence: public std::vector<ContactInfo>{
	};

public:
	CRActionPlanner();

	void SaveState(SGScene* scene, bool type);
	void LoadState(SGScene* scene, bool type);

	void PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene);
	void MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count);
	void Step(CRPuppet* puppet, CRUser* user, SGScene* scene);

	void ContactTest(CRHuman* human1, CRHuman* human2, SGScene* scene, int step);
	bool IsFirstContact(int vh, int user, int count);

	Vec3f ContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
	void SetContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene, Vec3f* pos);
	bool ContactCheckOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
	bool ContactCheckOfSolid(PHSolid* so, CRHuman* human, SGScene* scene);

	Vec3f GetContactForceOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
	Vec3f GetContactForceOfSolid(PHSolid* so, CRHuman* human, SGScene* scene);

	bool ChooseTargetAction(CRPuppet* puppet, CRUser* user);

	Vec3f GetNearestPoint(Vec3f a, Vec3f b, Vec3f c);
	Vec3f GetPointToAvoid(Vec3f a, Vec3f b, Vec3f c, float d);
	Vec3f GetPointToGuard(Vec3f a, Vec3f b, Vec3f c);
		
	bool bPlanner;
	bool bPrediction;
	bool bLoadReal, bLoadTemp;
	int times;
	float startTime, currentTime;

	SGBehaviorStates stateReal, stateTemp;
	ContactInfoSequence contactInfo;
};

}

#endif // !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
