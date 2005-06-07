// CRBallActionPlanner.h: CRBallActionPlanner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#include <Creature/CRBallUser.h>
#include <Graphics/GRRender.h>
#include <SceneGraph/SGScene.h>
#include <vector>

#if !defined(CRACTIONPLANNER_H)
#define CRACTIONPLANNER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Spr{;

class CRBallActionPlanner{
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
	CRBallActionPlanner();
	void Load();

	void SaveState(SGScene* scene, bool type);
	void LoadState(SGScene* scene, bool type);

	void Step(CRBallPuppet* puppet, CRBallUser* user, SGScene* scene);

	void PredictionAction(CRBallPuppet* puppet, CRBallUser* user, SGScene* scene);
	void MovementPrediction(CRBallPuppet* puppet, CRBallUser* user, SGScene* scene, int count);
	void PredictionStep(CRBallPuppet* puppet, CRBallUser* user, SGScene* scene, int count);

	void ContactTest(CRBallPuppet* puppet1, CRBallPuppet* puppet2, SGScene* scene, int step);
	bool IsFirstContact(int vh, int user, int count);

	bool ChooseTargetAction(CRBallPuppet* puppet, CRBallUser* user);

	Vec3f GetNearestPoint(Vec3f a, Vec3f b, Vec3f c);
	Vec3f GetPointToAvoid(Vec3f a, Vec3f b, Vec3f c, float d);
	Vec3f GetPointToGuard(Vec3f a, Vec3f b, Vec3f c);

	bool bDraw;
	bool bPlanner;
	bool bPrediction;
	bool bLoadReal, bLoadTemp;
	int times;
	float startTime, currentTime;
	Vec3f cPos;

	SGBehaviorStates stateReal, stateTemp;
	ContactInfoSequence contactInfo;
};

}

#endif // !defined(CRACTIONPLANNER_H)
