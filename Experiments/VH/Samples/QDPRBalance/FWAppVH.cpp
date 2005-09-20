// FWAppVH.cpp: FWAppVH クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWAppVH.h"
#include <Framework/Framework.h>

#include "QDPRBalance.h"
#include "VHCheckDialog.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

namespace Spr{;

FWAppVH::FWAppVH()
{
	contactEngine = NULL;
	bCalc = false;

}

FWAppVH::~FWAppVH()
{

}

void FWAppVH::Step(){
	if (bBusy) return;
	cs.Enter();
	//scene->Step();
	scene->ClearForce();
	scene->GenerateForce();

	//////////////		StartFWAppVH		//////////////////
	if(model && model->IsLoaded()){
		if(scene->GetCount() < 10){
			for(int i = 0; i < 2; i ++){
				model->stayPosi[i].AddSpringForce(scene->GetTimeStep());
				model->stayPost[i].AddSpringTorque(scene->GetTimeStep());
			}
		}
	// アプリの制御（リロードなど）
		AppControl(model);
		if(bCalc && scene->GetCount() > 10){
		//if(bCalc){
			// バランス制御(二次計画法)
			model->MaintainBalance(scene,render);

			// 描画
			model->balance.Draw(render,&model->supportArea);

			render->SetModelMatrix(Affinef());
			render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,0,1)));
			render->SetLineWidth(10);
			Vec3f v = model->GetCOG();
			//Vec3f v = model->balance.GetAnkleZMP();
			render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);

			//qdpr.Step(&model->balance);
		}
	}
	//////////////		EndFWAppVH		//////////////////

	scene->Integrate();
	pointers.Step(scene->GetTimeStep());
	cs.Leave();
}

void FWAppVH::Load(std::string fn){
	bBusy = true;
	cs.Enter();
	if (!render){
		initialLoadFile = fn;
		return;
	}
	lastLoadFile = fn;

	model = NULL;
	scene->Clear();
	scene->GetRenderers().Add(render);

	fileIO.Load(lastLoadFile.c_str(), scene);
//	scene->GetWorld()->Print(DSTR);
	ConnectHis();
	cs.Leave();
	bBusy = false;

	//////////////		StartFWAppVH	//////////////////
	bCalc = false;
	contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);

	model = new ModelUnder;
	model->Load(scene);
	//////////////		EndFWAppVH		//////////////////
}

void FWAppVH::Draw(){
	if (!render) return;
	render->Render(scene);		//	シーンの描画
	render->EndScene();			//	描画終了
	render->Present();			//	表示

	DrawDlg();					//別ダイアログで描画

	render->SetViewport(hWnd);	//	Viewportの設定
	render->ClearBuffer();		//	バッファクリア
	render->BeginScene();		//	描画開始

	//////////////		StartFWAppVH	////////////////
	//crVH.balanceQDPR.Draw(render);

	/*
	if(model->IsLoaded()){
		render->SetModelMatrix(Affinef());
		render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
		render->SetLineWidth(10);
		Vec3f v = model->balance.GetAnkleZmpR();
		render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);

		render->SetModelMatrix(Affinef());
		render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,1,1)));
		render->SetLineWidth(10);
		Vec3f vv = model->balance.GetAnkleZmpL();
		render->DrawDirect(Spr::GRRender::POINTS, &vv , &vv+1);

	}
	*/

	//////////////		EndFWAppVH		///////////////
}

bool FWAppVH::OnKeyDown(unsigned int nChar){
	//////////////		StartFWAppVH	////////////////
	if (nChar == 'E'){
		//crVH.crBalance.OnKeyDown(nChar,&crVH);
		//crVH.joints[2]->SetTorque(10);
		//crVH.jointPids[37]->goal += 5*M_PI/180;
		model->jointPids[9]->goal += 5*M_PI/180;
		//model->solids[0]->AddForce(Vec3f(10.0f,0.0f,0.0f));
	}
	//////////////		EndFWAppVH		///////////////
	else{
		FWApp::OnKeyDown(nChar);
	}
	return true;
}

void FWAppVH::DrawDlg(){
	if(vhDlg.IsDraw()){
		//vhDlg.Draw(render,&model);
	}
}

void FWAppVH::AppControl(ModelUnder* model){

	SGFrame* floor;
	scene->FindObject(floor,"frFloor");

	// 足が地面についたらバランス制御開始
	if(!bCalc){
		//足首ある用
		PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(model->solids[model->solids.size()-1]->GetFrame(),floor);
		//足首ない用
		//PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(model->solids[model->solids.size()-3]->GetFrame(),floor);
		
		if(fpr->GetReflexForce().norm() > 0.1){
			bCalc = true;
		}
	}

	// //転倒したらリロードする
	//PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crVH->solids[0]->GetFrame(),floor);
	//if(fpr->GetReflexForce().norm() > 0.1){
	//	Reset();
	//}
}
void FWAppVH::AppControl(Model* model){

	SGFrame* floor;
	scene->FindObject(floor,"frFloor");

	// 足が地面についたらバランス制御開始
	if(!bCalc){
		//足首ある用
		PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(model->solids[model->solids.size()-1]->GetFrame(),floor);
		//足首ない用
		//PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(model->solids[model->solids.size()-3]->GetFrame(),floor);
		
		if(fpr->GetReflexForce().norm() > 0.1){
			bCalc = true;
		}
	}

	// //転倒したらリロードする
	//PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crVH->solids[0]->GetFrame(),floor);
	//if(fpr->GetReflexForce().norm() > 0.1){
	//	Reset();
	//}
}


}	//namespace Spr
