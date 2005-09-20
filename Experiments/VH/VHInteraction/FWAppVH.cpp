// FWAppVH.cpp: FWAppVH �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWAppVH.h"
#include <Framework/Framework.h>
#include <ImpD3D/D3Render.h>

#include "VHInteraction.h"
#include "VHCheckDialog.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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

		// SPIDAR �̃`�F�b�N
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
			// ���x�����O�J����
			//if(slc.Step(scene)) slc.SetUserPos(&crUser);
			// Spidar
			crUser.SetSpidarPos(spidars);
			// �}�E�X�œ���
			//crUser.positionSprings[1].SetTarget(((UTRef<FWPointer6D>) pointers[0])->GetPos(),Vec3f(),true);

			crUser.Step(scene);
		}

		if(crPuppet.IsLoaded() && crUser.IsLoaded()){
			// �U������
			crPuppet.Attack(&crUser);
			crUser.Attack(&crPuppet);
			// �h�䓮��(�e�X�g)
			if(!crPlanner.bPlanner) crPuppet.GuardTest(&crUser, scene);
			crUser.GuardTest(&crPuppet, scene);
			// ���B�^���̐���
			//crPuppet.ReachingMovemantsSafetyCheck(&crUser, scene);
			//crUser.ReachingMovemantsSafetyCheck(&crPuppet, scene);
			// �����蔻��
			crPuppet.HittedCheck(&crUser, scene);
			crUser.HittedCheck(&crPuppet, scene);
			// �͊o�񎦂̃e�X�g
			crUser.SetSpidarForce(&crPuppet, scene, spidars);
		}
		//////////////		EndFWAppVH		//////////////////

		scene->GenerateForce();
		scene->Integrate();

		//pointers.Step(scene->GetTimeStep());

		cs.Leave();

	}
	//////////////		�\�����f��		//////////////////
	// �\���s���̃e�X�g
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
		// ����
		soPos[0] = crUser.solids[3]->GetRotation() 
			* Vec3f(0,  crUser.GetSolidInfo(3).scale.Y(), 0)
			+ crUser.solids[3]->GetCenterPosition();
		// �E�I
		soPos[1] = crUser.solids[4]->GetRotation() 
			* Vec3f(0, -crUser.GetSolidInfo(4).scale.Y(), 0)
			+ crUser.solids[4]->GetCenterPosition();
		// ���I
		soPos[2] = crUser.solids[7]->GetRotation() 
			* Vec3f(0, -crUser.GetSolidInfo(7).scale.Y(), 0)
			+ crUser.solids[7]->GetCenterPosition();

		slc.SetInitMarkerPos(soPos);
	}*/

	// �f�o�C�X(Spidar4)�̐ڑ�
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
	render->Render(scene);		//	�V�[���̕`��
	render->EndScene();			//	�`��I��
/*
	WXINTF(Direct3DSurface9) surf;
	((D3Render*)&*render)->device->GetRenderTarget(0, &surf.Intf());
	HDC dc=NULL;
	surf->GetDC(&dc);
	TextOut(dc, 0,0, "abc", 3);
	surf->ReleaseDC(dc);
*/
	render->Present();			//	�\��

//	DrawDlg();					//�ʃ_�C�A���O�ŕ`��

	render->SetViewport(hWnd);	//	Viewport�̐ݒ�
	render->ClearBuffer();		//	�o�b�t�@�N���A
	render->BeginScene();		//	�`��J�n

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
