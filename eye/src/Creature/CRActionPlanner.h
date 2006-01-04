// CRActionPlanner.h: CRActionPlanner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#include <Collision/CDCollisionEngine.h>
#include <Creature/CRUser.h>
#include <Graphics/GRRender.h>
#include <SceneGraph/SGScene.h>
#include <vector>

#if !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
#define AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Creature/CRInternalModel.h>

namespace Spr{;

class CRActionPlanner{
	enum SceneType{STReal, STInternal};
	
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
	void Load(SGScene* rScene, SGScene* iScene, CRPuppet* rPuppet, CRPuppet* iPuppet, CRUser* iUser, CRInternalModel* internalModel);

	void SaveState(SGScene* scene, SceneType type);
	void LoadState(SGScene* scene, SceneType type);

	void Step(bool bInternalModelStable);

	void PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene);
	void MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count);
	void PredictionStep(CRPuppet* puppet, CRUser* user, SGScene* scene, int count);

	void ContactTest(CRPuppet* puppet1, CRPuppet* puppet2, SGScene* scene, int step);
	bool IsFirstContact(int vh, int user, int count);

	bool ChooseTargetAction(CRPuppet* puppet, CRUser* user);

	Vec3f GetNearestPoint(Vec3f a, Vec3f b, Vec3f c);
	Vec3f GetPointToAvoid(Vec3f a, Vec3f b, Vec3f c, float d);
	Vec3f GetPointToGuard(Vec3f a, Vec3f b, Vec3f c);

	// 特定Solidの衝突判定を無効に設定
	void SetSolidInvisible(PHSolid* solid);
	// 無効にしたSolidの衝突判定を再開
	void SetSolidVisible(PHSolid* solid);
	// 衝突判定Off
	void DisableContact(CRPuppet* puppet, SGScene* scene);
	// 衝突判定On
	void EnableContact(CRPuppet* puppet, SGScene* scene);

	SGScene* rScene;
	SGScene* iScene;
	CRPuppet* rPuppet;
	CRPuppet* iPuppet;
	CRUser* iUser;
	CRInternalModel* internalModel;

	bool bDraw;
	bool bPlanner;
	bool bPrediction;
	bool bLoadReal, bLoadTemp;
	int times;
	float startTime, currentTime;
	Vec3f cPos;
	std::vector<PHSolid*> contactDisabledSolids;

	SGBehaviorStates stateReal, stateTemp;
	ContactInfoSequence contactInfo;
};

}

#endif // !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
