// CRActionPlanning.h: CRActionPlanning クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//#include <Physics/PHHuman.h>
#include <Creature/CRHuman.h>
#include <Graphics/GRRender.h>
#include <vector>
#include <Physics/PHContactEngine.h>
#include <SceneGraph/SGScene.h>

#if !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
#define AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRActionPlanning{
	class CRContactInfo{
	public:
		int firstCount, lastCount;
		int soVHIndex, soUserIndex;
		int contactType;
	
		CRContactInfo(int vh, int user, int c){
			soVHIndex = vh;
			soUserIndex = user;
			firstCount = lastCount = c;
			SetContactType(vh, user);
		}
		void SetContactType(int vh, int user);
	};
	class CRContactInfoSequence: public std::vector<CRContactInfo>{
	};

public:
	#define STEPCOUNT 500
	#define Dt2	0.001

	CRActionPlanning();
	virtual ~CRActionPlanning();

	void OnDraw(Spr::UTRef<Spr::GRRender> render, Spr::SGScene* scene, CWnd* m_hWnd);
	void OnKeyDown(UINT nChar, Spr::SGScene* scene);
	void SaveState(Spr::SGScene* scene);
	void LoadState(Spr::SGScene* scene);
	void MovementPrediction(Spr::CRHuman* human1, Spr::CRHuman* human2, Spr::SGScene* scene, float dt);
	void Step(Spr::CRHuman* human1, Spr::CRHuman* human2, Spr::SGScene* scene);
	void ContactTest(Spr::CRHuman* human1, Spr::CRHuman* human2, Spr::SGScene* scene, int step);
	Spr::Vec3f ContactCheckOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene);
	Spr::Vec3f ContactCheckOfSolid(Spr::PHSolid* so, Spr::CRHuman* human, Spr::SGScene* scene);
	Spr::Vec3f GetReflexForce();
	bool IsFirstContact(int vh, int user, int count);
	void GoalDirectedMovement(Spr::PHSolid* so, Spr::Vec3f pos, float time);
		
	bool bStates;
	Spr::SGBehaviorStates states;
	CRContactInfoSequence contactInfo;
};

#endif // !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
