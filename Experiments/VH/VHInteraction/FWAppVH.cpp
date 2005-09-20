// FWAppVH.cpp: FWAppVH クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWAppVH.h"
#include <Framework/Framework.h>
#include <ImpD3D/D3Render.h>

#include "VHInteraction.h"
#include "VHCheckDialog.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

namespace Spr{;

FWAppVH::FWAppVH(){
}

FWAppVH::~FWAppVH(){
}

void FWAppVH::Step(){
//	timer.Clear();
//	timer.Start();
	for(int i = 0; i < spidars.size(); ++i){
		spidars[i]->Update(0.033f);

		// SPIDAR のチェック
		//DSTR << "Spidar[" << i << "]:" << spidars[i]->GetPos() << std::endl;
		//for(int j = 0; j < 4; ++j) DSTR << " m[" << 4*i+j << "] = " << spidars[i]->Motor()[j].GetLength();
		//DSTR << std::endl;
	}
	
	for(int i = 0; i < 1; ++i){
		if (bBusy) return;
		cs.Enter();
		scene->ClearForce();

		//////////////		StartFWAppVH		//////////////////
		if(crPuppet.IsLoaded()){
			crPuppet.Step(scene);
		}

		if(crUser.IsLoaded()){
			// ラベリングカメラ
			//if(slc.Step(scene)) slc.SetUserPos(&crUser);
			// Spidar
			crUser.SetSpidarPos(spidars);
			// マウスで入力
			//crUser.positionSprings[1].SetTarget(((UTRef<FWPointer6D>) pointers[0])->GetPos(),Vec3f(),true);

			crUser.Step(scene);
		}

		if(crPuppet.IsLoaded() && crUser.IsLoaded()){
			// 攻撃動作
			crPuppet.Attack(&crUser);
			crUser.Attack(&crPuppet);
			// 防御動作(テスト)
			if(!crPlanner.bPlanner) crPuppet.GuardTest(&crUser, scene);
			crUser.GuardTest(&crPuppet, scene);
			// 到達運動の制限
			//crPuppet.ReachingMovemantsSafetyCheck(&crUser, scene);
			//crUser.ReachingMovemantsSafetyCheck(&crPuppet, scene);
			// 当たり判定
			crPuppet.HittedCheck(&crUser, scene);
			crUser.HittedCheck(&crPuppet, scene);
			// 力覚提示のテスト
			crUser.SetSpidarForce(&crPuppet, scene, spidars);
		}
		//////////////		EndFWAppVH		//////////////////

		scene->GenerateForce();
		scene->Integrate();

		//pointers.Step(scene->GetTimeStep());

		cs.Leave();

	}
	//////////////		予測モデル		//////////////////
	// 予測行動のテスト
	crPlanner.Step(&crPuppet, &crUser, scene);
//	if(crPlanner.bPrediction) DSTR << timer.Stop()*1e-3 << std::endl;
}

void FWAppVH::Load(std::string fn){
	bBusy = true;
	cs.Enter();
	if (!render){
		initialLoadFile = fn;
		return;
	}
	lastLoadFile = fn;

	scene2= new SGScene;
	scene->Clear();
	scene2->Clear();
	scene->GetRenderers().Add(render);
	scene2->GetRenderers().Add(render);

	fileIO.Load(lastLoadFile.c_str(), scene);
	fileIO.Load(lastLoadFile.c_str(), scene2);

	ConnectHis();
	cs.Leave();
	bBusy = false;

	//////////////		StartFWAppVH	//////////////////
	crUser.Load(scene);
	crPuppet.Load(scene);
	crPlanner.Load();
	bDrawTest  = false;
	bDrawCount = false;
	bDrawState = true;

/*	slc.InitSLC();
	if(crUser.IsLoaded()){
		Vec3f soPos[3];
		// 頭頂
		soPos[0] = crUser.solids[3]->GetRotation() 
			* Vec3f(0,  crUser.GetSolidInfo(3).scale.Y(), 0)
			+ crUser.solids[3]->GetCenterPosition();
		// 右肘
		soPos[1] = crUser.solids[4]->GetRotation() 
			* Vec3f(0, -crUser.GetSolidInfo(4).scale.Y(), 0)
			+ crUser.solids[4]->GetCenterPosition();
		// 左肘
		soPos[2] = crUser.solids[7]->GetRotation() 
			* Vec3f(0, -crUser.GetSolidInfo(7).scale.Y(), 0)
			+ crUser.solids[7]->GetCenterPosition();

		slc.SetInitMarkerPos(soPos);
	}*/

	// デバイス(Spidar4)の接続
	spidars.clear();
	for(int i = 0; i < pointers.size(); ++i){
		if(strcmp(pointers[i]->device->GetType(), "Spidar4") == 0){
			HISpidar4* spidar;
			spidar = UTRef<HISpidar4> (pointers[i]->device);
			spidar->Calib();
			spidars.push_back(spidar);
		}
	}
	//////////////		EndFWAppVH		//////////////////

}

void FWAppVH::Draw(){
	if (!render) return;
	render->Render(scene);		//	シーンの描画
	render->EndScene();			//	描画終了
/*
	WXINTF(Direct3DSurface9) surf;
	((D3Render*)&*render)->device->GetRenderTarget(0, &surf.Intf());
	HDC dc=NULL;
	surf->GetDC(&dc);
	TextOut(dc, 0,0, "abc", 3);
	surf->ReleaseDC(dc);
*/
	render->Present();			//	表示

//	DrawDlg();					//別ダイアログで描画

	render->SetViewport(hWnd);	//	Viewportの設定
	render->ClearBuffer();		//	バッファクリア
	render->BeginScene();		//	描画開始

	//////////////		StartFWAppVH	////////////////

	//////////////		EndFWAppVH		///////////////
}

bool FWAppVH::OnKeyDown(unsigned int nChar){
	//////////////		StartFWAppVH	////////////////
	if (nChar == 'L'){
		for(int i = 0; i < spidars.size(); ++i) spidars[i]->Calib();
		slc.Init();
	}
	if (nChar == 'K') crUser.bSpidarForce = !crUser.bSpidarForce;
	if (nChar == '0'){
		crPlanner.bDraw = !crPlanner.bDraw;
		crPuppet.bDraw  = !crPuppet.bDraw;
		crUser.bDraw    = !crUser.bDraw;
	}
	if (nChar == '9') bDrawState = !bDrawState;
	if (nChar == '8'){
		bDrawCount = !bDrawCount;
		crPuppet.hittingCount = crUser.hittingCount = 0;
	}

	if (nChar == '1') crPlanner.bPlanner = crPuppet.bGuard = !crPlanner.bPlanner;
	if (nChar == '2') crPuppet.bGuard    = !crPuppet.bGuard;
	if (nChar == '3') crPuppet.bAttack   = !crPuppet.bAttack;
	if (nChar == '4') crUser.bGuard      = !crUser.bGuard;
	if (nChar == '5') crUser.bAttack     = !crUser.bAttack;
	if (nChar == '6') crUser.AttackTest(&crPuppet);
	if (nChar == '7') bDrawTest = !bDrawTest;
	//////////////		EndFWAppVH		///////////////
	else{
		FWApp::OnKeyDown(nChar);
	}
	return true;
}

void FWAppVH::DrawDlg(){
	if(vhDlg.IsDraw()){
		//vhDlg.Draw(render,&crHuman);
		//vhDlg.DrawTest(render,scene);
	}
}

}	//namespace Spr
