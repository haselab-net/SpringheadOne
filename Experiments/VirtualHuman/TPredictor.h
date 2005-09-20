// TPredictor.h: TPredictor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#include <Physics/PHHuman.h>
#include <Graphics/GRRender.h>
#include <vector>
#include <Physics/PHContactEngine.h>
#include <SceneGraph/SGScene.h>

#if !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
#define AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TPredictor
{
public:
	#define STEPCOUNT 500
	#define Dt2	0.001

	TPredictor();
	virtual ~TPredictor();

	void OnDraw(Spr::UTRef<Spr::GRRender> render, Spr::SGScene* scene, CWnd* m_hWnd);
	void OnKeyDown(UINT nChar, Spr::SGScene* scene);
	void SaveState(Spr::SGScene* scene);
	void LoadState(Spr::SGScene* scene);
	void MovementPrediction(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene, float dt);
	void Step(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene);
	void ContactTest(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene);
	Spr::Vec3f ContactCheckOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene);
	Spr::Vec3f ContactCheckOfSolid(Spr::PHSolid* so, Spr::PHHuman* human, Spr::SGScene* scene);
	Spr::Vec3f GetReflexForce();
		
	bool bStates;
	Spr::SGBehaviorStates states;

};

#endif // !defined(AFX_MOVEMENTPREDICTOR_H__71A071B6_77BD_4835_9979_17A1DB9DF1F8__INCLUDED_)
