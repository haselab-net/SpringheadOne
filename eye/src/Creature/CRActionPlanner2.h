// CRActionPlanner2.h: CRActionPlanner2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRACTIONPLANNER2_H
#define CRACTIONPLANNER2_H

#include <Creature/CRActionPlanner.h>
#include <Creature/CRInternalModel.h>

namespace Spr{;

	class CRInternalModel;
	class CRPuppet;

	class CRActionPlanner2: public CRActionPlanner{
	public:
		void Load(SGScene* rScene, SGScene* iScene, CRPuppet* rPuppet, CRPuppet* iPuppet, CRUser* iUser, CRInternalModel* internalModel);
		void Step(bool bInternalModelStable);

		void PredictionAction(CRPuppet* puppet, CRUser* user, SGScene* scene);
		void MovementPrediction(CRPuppet* puppet, CRUser* user, SGScene* scene, int count);
		bool ChooseTargetAction(CRPuppet* puppet, CRUser* user);

		SGScene* rScene;
		SGScene* iScene;
		CRPuppet* rPuppet;
		CRPuppet* iPuppet;
		CRUser* iUser;
		CRInternalModel* internalModel;
	};

}

#endif
