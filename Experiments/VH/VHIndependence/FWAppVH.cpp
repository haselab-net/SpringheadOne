// FWAppVH.cpp: FWAppVH �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWAppVH.h"
#include <Framework/Framework.h>

#include "vhindependence.h"
#include "VHCheckDialog.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

namespace Spr{;

FWAppVH::FWAppVH()
{
	contactEngine = NULL;
	bCalc = false;
	balance = true;

}

FWAppVH::~FWAppVH()
{

}

void FWAppVH::Step(){
	//timer.Start();
	if (bBusy) return;
	cs.Enter();
	//scene->Step();
	scene->ClearForce();
	/*
	//////////////		StartFWAppVH		//////////////////
	if(crVH){
	// �A�v���̐���i�����[�h�Ȃǁj
		AppControl(crVH);
		if(bCalc){
			if(scene->GetCount() >= 100){
				// ���B�^��
				//crVH->ReachingMovment(scene);
			}
		}
	}
	//////////////		EndFWAppVH		//////////////////
	*/
	scene->GenerateForce();

	//////////////		StartFWAppVH		//////////////////
	if(crVH){
	// �A�v���̐���i�����[�h�Ȃǁj
		AppControl(crVH);
		if(bCalc){
			if(scene->GetCount() < 50){
				for(int i = 0; i < 2; i ++)
					crVH->stayPost[i].AddSpringTorque(scene->GetTimeStep());
			}
			else{
				// ���B�^��
				crVH->ReachingMovment(scene);
				if(balance)
					// �o�����X����(�񎟌v��@)
					crVH->MaintainBalance(scene);
				// �`��
				//crVH->Draw(render);
				//DataOut();
			}
		}
	}
	//////////////		EndFWAppVH		//////////////////

	scene->Integrate();
	pointers.Step(scene->GetTimeStep());
	cs.Leave();
	
	//DSTR << "�S�v�Z����" << timer.Stop()*1e-3 << "[ms]" << std::endl;
	//timer.Clear();
}

void FWAppVH::Load(std::string fn){
	bBusy = true;
	cs.Enter();
	if (!render){
		initialLoadFile = fn;
		return;
	}
	lastLoadFile = fn;
	
	crVH=NULL;
	scene->Clear();
	scene->GetRenderers().Add(render);

	fileIO.Load(lastLoadFile.c_str(), scene);
	crVH = new CRVirtualHuman;
	//crVH = new CRVH;
//	scene->GetWorld()->Print(DSTR);
	ConnectHis();
	cs.Leave();
	bBusy = false;

	//////////////		StartFWAppVH	//////////////////
	bCalc = false;
	contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);

	crVH->Load(scene);
	//////////////		EndFWAppVH		//////////////////
}

void FWAppVH::Draw(){
	if (!render) return;
	render->Render(scene);		//	�V�[���̕`��
	render->EndScene();			//	�`��I��
	render->Present();			//	�\��

	DrawDlg();					//�ʃ_�C�A���O�ŕ`��

	render->SetViewport(hWnd);	//	Viewport�̐ݒ�
	render->ClearBuffer();		//	�o�b�t�@�N���A
	render->BeginScene();		//	�`��J�n

	//////////////		StartFWAppVH	////////////////
	//crVH->balanceQDPR.Draw(render);
	//////////////		EndFWAppVH		///////////////
}

bool FWAppVH::OnKeyDown(unsigned int nChar){
	//////////////		StartFWAppVH	////////////////
	crVH->OnKeyDown(nChar,scene);	// 3,4,5,6,7,K
	/*
	if(nChar == 'M'){
		crVH->balanceQDPR.SetBalanceState(crVH->balanceQDPR.TBlance::RIGHT_LEG);
	}
	else if(nChar == 'N'){
		crVH->balanceQDPR.SetBalanceState(crVH->balanceQDPR.TBlance::BOTH_LEG);
	}
	else if(nChar == 'B'){
		crVH->balanceQDPR.SetBalanceState(crVH->balanceQDPR.TBlance::LEFT_LEG);
	}
	else if(nChar == 'V'){
		crVH->balanceQDPR.changeQDPR = true;
	}
	else if (nChar == 'E'){
		crVH->rMovment.SetTarget(scene,crVH,((UTRef<FWPointer6D>) pointers[0])->GetPos());
	}
	else if (nChar == 'J'){
		static int i =0;
		i++;
		//Matrix3f mat = Matrix3f::Rot((float)Rad(-i), 'z');
		//crVH->balanceQDPR.targetPostureQ.from_matrix(mat);
		crVH->jointPids[crVH->ankleJointNum[0][1]]->goal +=Rad(1);
	}
	*/
	// ���񒲐ߐ���ON
	if (nChar == 'H'){
		balance = !balance;
		//crVH->balanceQDPR.bAdjust = true;
	}
	else if(nChar == 'J'){
		crVH->OffSpring();
	}
	else if(nChar == 'K'){
		//crVH->SetJointSpring(scene->GetTimeStep());
		//crVH->balanceQDPR.OffSpring();
		crVH->rMovmentPD.offSpring = !crVH->rMovmentPD.offSpring;
		//crVH->rMovmentPD.offSpring = true;
	}
	// �E�蓞�B�^��
	else if(nChar == '1'){
		if(crVH->rMovmentPD.movmentSolid.size()>0){
			crVH->rMovmentPD.StartReachingMovment(0);
			crVH->rMovmentPD.SetTarget(scene,((UTRef<FWPointer6D>) pointers[0])->GetPos());
		}
	}
	// ���蓞�B�^��
	else if(nChar == '2'){
		if(crVH->rMovmentPD.movmentSolid.size()>1){
			crVH->rMovmentPD.StartReachingMovment(1);
			crVH->rMovmentPD.SetTarget(scene,((UTRef<FWPointer6D>) pointers[0])->GetPos());
		}
	}
	// PD+���x�̍ŏ����B�^��
	else if(nChar == 'N'){
		crVH->rMovmentPD.pdType = true;
	}
	// PD�̍ŏ����B�^��
	else if(nChar == 'M'){
		crVH->rMovmentPD.pdType = false;
	}
	// ���x+���R�r�A���̍ŏ����B�^��
	else if (nChar == 'E'){
		crVH->rMovment.SetTarget(scene,crVH,((UTRef<FWPointer6D>) pointers[0])->GetPos());
	}
	//////////////		EndFWAppVH		///////////////
	else{
		FWApp::OnKeyDown(nChar);
	}
	return true;
}

void FWAppVH::DrawDlg(){
	if(vhDlg.IsDraw()){
		vhDlg.Draw(render,crVH);
	}
}

void FWAppVH::AppControl(CRVirtualHuman* crVH){

	SGFrame* floor;
	scene->FindObject(floor,"frFloor");

	// �����n�ʂɂ�����o�����X����J�n
	if(!bCalc){
		PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crVH->solids[12]->GetFrame(),floor);
		//PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crVH->solids[11]->GetFrame(),floor);
		if(fpr->GetReflexForce().norm() > 0.1){
			bCalc = true;
		}
	}

	// //�]�|�����烊���[�h����
	PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crVH->solids[0]->GetFrame(),floor);
	if(fpr->GetReflexForce().norm() > 0.1){
		//Reset();
	}
}

void FWAppVH::DataOut(){

	//if(crVH->rMovmentPD.bMove){
	if(crVH){
		static double time = 0.0;
		static double step =0.0;

		//time = timer.Stop()*1e-3;
		//DSTR << "�S�v�Z����" << time << "[ms]" << std::endl;
		//timer.Clear();

		double t = scene->GetTimeStep()*(scene->GetCount() - step);

		if(t <= 30){
			static bool bFirst = true;
			std::ofstream of;
			if (bFirst){
				of.open("time.xls");
				bFirst = false;
				step = scene->GetCount();
				timer.Clear();
			}else{
				time = timer.Stop()*1e-3;
				timer.Clear();
				of.open("time.xls",std::ios::app);
				if(time < 5)
					of << t << '\t' << time <<std::endl;
			}

			of.close();
			timer.Start();
		}
	}

}

}	//namespace Spr
