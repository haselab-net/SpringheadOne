// CRBrain.cpp: CRBrain クラスのインプリメンテーション
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
		//// 動きの解析
		crvMotionAnalysis.Step();
		//// 注意機構
		crAttention.Step();
		//// 首の制御
		crNeckController.Step();
		//// 目の制御
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
		//// 動きの解析
		crvMotionAnalysis.Step();
		//// 内部モデルのシミュレーション
		//crInternalModel.Step();
		//crInternalModel.AttentionFromCertainty(&crAttention);

		//// 予測行動
		//crPlanner.Step(crInternalModel.IsStable());
		//// 攻撃動作
		puppet->Attack(user);
		puppet->TopDownAttention(&crAttention);
		//// 防御動作
		if(!crPlanner.bPlanner){
			puppet->GuardTest(user, scene);
		}
			
		// 到達運動の制限
		puppet->ReachingMovemantsSafetyCheck(user, scene);
		//// 当たり判定
		puppet->HittedCheck(user, scene, crPlanner.bPlanner);

		//// 注意機構
		crAttention.Step();
		//// 首の制御
		if (crEye.bEyeMode != 0) crNeckController.Step();
		//// 目の制御
		crEye.Step();
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



