// TPredictor.cpp: TPredictor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "virtualhuman.h"
#include "TPredictor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TPredictor::TPredictor()
{
	bStates = false;
}

TPredictor::~TPredictor()
{

}

void TPredictor::OnDraw(Spr::UTRef<Spr::GRRender> render, Spr::SGScene* scene, CWnd* m_hWnd){
	if (!render) return;
	render->Render(scene);			//	シーンの描画

	render->EndScene();				//	描画終了
	render->Present();				//	表示

	render->SetViewport(&m_hWnd);	//	Viewportの設定
	render->ClearBuffer();			//	バッファクリア
	render->BeginScene();			//	描画開始
}
	
void TPredictor::OnKeyDown(UINT nChar, Spr::SGScene* scene){
	if(nChar == 'P') SaveState(scene);
	if(nChar == 'O') LoadState(scene);
}

void TPredictor::SaveState(Spr::SGScene* scene){
	bStates = true;
	scene->SaveState(states);	
}

void TPredictor::LoadState(Spr::SGScene* scene){
	if(bStates) scene->LoadState(states);	
}

void TPredictor::MovementPrediction(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene, float dt){
	SaveState(scene);
	scene->SetTimeStep(dt*0.5);
	for(int i = 0; i < STEPCOUNT; i++){
		Step(human1, human2, scene);
	}
	scene->SetTimeStep(dt);
	LoadState(scene);
}

void TPredictor::Step(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene){
	scene->ClearForce();
	scene->GenerateForce();
	scene->Integrate();
}

void TPredictor::ContactTest(Spr::PHHuman* human1, Spr::PHHuman* human2, Spr::SGScene* scene){
	DSTR << ContactCheckOfSolid(human2->solids[6], human1, scene) << std::endl;
}

Spr::Vec3f TPredictor::ContactCheckOfSolidPair(Spr::PHSolid* so1, Spr::PHSolid* so2, Spr::SGScene* scene){
	Spr::PHContactEngine* pce = NULL;
	Spr::Vec3f rforce;

	scene->GetBehaviors().Find(pce);
	rforce  = pce->GetFramePairRecord(so1->GetFrame(), so2->GetFrame())->GetReflexForce();
	rforce += pce->GetFramePairRecord(so1->GetFrame(), so2->GetFrame())->GetFrictionForce();

	pce->GetFramePairRecord(so1->GetFrame(), so2->GetFrame())->Clear();
	return rforce;
}

Spr::Vec3f TPredictor::ContactCheckOfSolid(Spr::PHSolid* so, Spr::PHHuman* human, Spr::SGScene* scene){
	Spr::Vec3f rforce = Spr::Vec3f(0,0,0);
	for(int i = 0; i < human->solids.size(); ++i){
		rforce += ContactCheckOfSolidPair(human->solids[i], so, scene);
	}
	return rforce;
}
