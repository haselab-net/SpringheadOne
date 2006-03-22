// CRBrain.cpp: CRBrain �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#pragma hdrstop

#include "CRBrain.h"

namespace Spr{

	//
	// CRBrain
	//

	void CRBrain::Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser){}
	void CRBrain::Step(){}
	void CRBrain::Draw(GRRender* render){}


	// 
	// CRBaseBrain
	//

	void CRBaseBrain::Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser){
		this->scene  = scene;
		this->puppet = crPuppet;
		this->user   = crUser;
		
		crvMotionAnalysis.Load(scene, crPuppet, &crEye, &crAttention); 
		crAttention.Load(scene, crPuppet, &crEye, &crNeckController);
		crNeckController.Load(scene, crPuppet);
		crEye.Load(scene, crPuppet, &crNeckController); 
	}

	void CRBaseBrain::Step(){
		//// �����̉��
		crvMotionAnalysis.Step();
		//// ���Ӌ@�\
		crAttention.Step();
		//// ��̐���
		crNeckController.Step();
		//// �ڂ̐���
		crEye.Step();
	}

	void CRBaseBrain::Draw(GRRender* render){
		crvMotionAnalysis.Draw(render);
		crAttention.Draw(render);
		crEye.Draw(render);
		crNeckController.Draw(render);
	}


	// 
	// CRSimulatingBrain
	//

	void CRSimulatingBrain::Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser){
		CRBaseBrain::Load(scene, crPuppet, crUser);

		crInternalModel.Load(filename, render, crPuppet);
		crvMotionAnalysis.SetInternalModel(&crInternalModel);
		crPlanner.Load(scene, crInternalModel.GetScene(),crPuppet,crInternalModel.GetPuppet(),crInternalModel.GetUser(),&crInternalModel);
	}

	void CRSimulatingBrain::Step(){
		//// �����̉��
		crvMotionAnalysis.Step();
		//// �������f���̃V�~�����[�V����
		//crInternalModel.Step();
		//crInternalModel.AttentionFromCertainty(&crAttention);

		//// �\���s��
		//crPlanner.Step(crInternalModel.IsStable());
		//// �U������
		puppet->Attack(user);
		puppet->TopDownAttention(&crAttention);
		//// �h�䓮��
		if(!crPlanner.bPlanner){
			puppet->GuardTest(user, scene);
		}
			
		// ���B�^���̐���
		puppet->ReachingMovemantsSafetyCheck(user, scene);
		//// �����蔻��
		puppet->HittedCheck(user, scene, crPlanner.bPlanner);

		//// ���Ӌ@�\
		crAttention.Step();
		//// ��̐���
		if (crEye.bEyeMode != 0) crNeckController.Step();
		//// �ڂ̐���
		crEye.Step();

		Vec3f eyeposL  = crEye.soLEye->GetFrame()->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
		Vec3f eyeposR  = crEye.soREye->GetFrame()->GetPosture().Rot() * Vec3f(0.0f, 0.0f, 1.0f);
		Vec3f headpos  = crNeckController.GetHeadOrientation() * Vec3f(0.0f, 0.0f, 1.0f);

		ofs << "L " << GetTickCount() << " " << eyeposL << std::endl;
		ofs << "R " << GetTickCount() << " " << eyeposR << std::endl;
		ofs << "H " << GetTickCount() << " " << headpos << std::endl;
		ofs << "U " << GetTickCount() << " " << crvMotionAnalysis.soHeadU->GetCenterPosition() << std::endl;
		ofs << "l " << GetTickCount() << " " << crvMotionAnalysis.soLHandU->GetCenterPosition() << std::endl;
		ofs << "r " << GetTickCount() << " " << crvMotionAnalysis.soRHandU->GetCenterPosition() << std::endl;
	}

	void CRSimulatingBrain::Draw(GRRender* render){
		crvMotionAnalysis.Draw(render);
		crAttention.Draw(render);
		crInternalModel.Draw(render);
		crEye.Draw(render);
		crNeckController.Draw(render);
	}

	void CRSimulatingBrain::ToggleEyeMode(){
		crEye.ToggleEyeMode();
	}

	void CRSimulatingBrain::StartAttentionExperiment(){
		crAttention.StartExperiment();
	}

} // namespace Spr



