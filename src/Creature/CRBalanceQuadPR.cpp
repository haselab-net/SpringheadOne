// CRBalanceQuadPR.cpp: CRBalanceQuadPR クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Creature.h"
#include "CRBalanceQuadPR.h"
#include "CRHuman.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRBalanceQuadPR::CRBalanceQuadPR()
{
	// 単位行列の初期化
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// 行列の一部を取り出す行列の初期化
	for(int i = 0; i < upMat.height(); i++){
		for(int j = 0; j < upMat.width(); j++){
			if(i == j){
				upMat[i][j] = 1.0;
			}
			else{
				upMat[i][j] = 0.0;
			}

			if(i == j -  lowMat.width()/2){
				lowMat[i][j] = 1.0;
			}
			else{
				lowMat[i][j] = 0.0;
			}
		}
	}
	// 伝播行列の初期化
	for(int i = 0; i < transmissionM.height(); i++){
		for(int j = 0; j < transmissionM.width(); j++){
			transmissionM[i][j] = 0.0;
		}
	}
	for(int i = 0; i < transmissionD.height(); i++){
		for(int j = 0; j < transmissionD.width(); j++){
			transmissionD[i][j] = 0.0;
		}
	}

	count = 0;
	coefficient = 0.01;
	gravity = NULL;
	jointEngine = NULL;

}

CRBalanceQuadPR::~CRBalanceQuadPR()
{

}
void CRBalanceQuadPR::Step(CRHuman* crHuman){
	timer.Start();
	SetQuadPR(crHuman);
	DSTR << "係数の計算時間：" <<timer.Stop() << "[μs]" <<std::endl;
	timer.Clear();
	QuadPRSolve();
	SetJointTorque(crHuman);
	//TEST
	//SetAnkleForce(crHuman);
	DataOut(crHuman);

}
void CRBalanceQuadPR::TransTorque(SGScene* scene,CRHuman* crHuman){

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	// 踵の剛体の力、トルクを退避
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();
	
	for(int i = 0; i <= JOINT_NUM+DOF_NUM; i++){

		// トルク&力パターンの生成
		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// フェザーストーンの一部を使い、左右足首最後のJointの加速度計算
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 右足首
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 左足首

		// 両足首の加速度
		PTM::TVector<DOF_NUM,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[R_ANKLE_J]);
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[L_ANKLE_J]);

		//  係数行列に代入
		// 伝播行列C
		if(i==0){
			transmissionC = accelVec;
		}
		// 伝播行列M
		else if(i > 0 && i <= JOINT_NUM){
			transmissionM.col(i-1) = accelVec - transmissionC;
		}
		// 伝播行列D
		else if(i > JOINT_NUM){
			transmissionD.col(i - (JOINT_NUM+1)) = accelVec - transmissionC;
		}

		
		// 踵の力、トルクを戻す
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}
}

void CRBalanceQuadPR::MakeTorquePattern(CRHuman* crHuman, int t){
/*	i = 0 だとすべて0→C
	0 < i ＜= crHuman->joints.size()だと,i-1番目の関節トルクが１ → M
	crHuman->joints.size()+1 ≦ i ＜ crHuman->joints.size()+7だと,
											足首にかかる力がｘから順番に１ →　D
	C→M→Dの順番で求めるトルクパターンを作る
*/

/////	関節のトルク、足首にかかる力すべて0	  //////
	if(t == 0){
		// 各関節トルクを0にする
		for(int i = 0; i < JOINT_NUM; i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

/////// トルクに１がはいる　/////////////
	// 各関節トルクに1を入れる
	else if(t > 0 && t <= JOINT_NUM){
		for(int i = 0; i < JOINT_NUM; i++){
			if(i == t-1){
				if(crHuman->joints[i] != NULL){
					crHuman->joints[i]->torque = 1.0;
				}
			}
			else{
				if(crHuman->joints[i] != NULL){
					crHuman->joints[i]->torque = 0.0;
				}
			}
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

//////	足首にかかる力が１はいる	///////
	else if(t > JOINT_NUM){
		// 足首にかかる力に１を入れる
		int i = t - (JOINT_NUM + 1);
		if(i < 3){
			crHuman->solids[R_HEEL_S]->AddForce(unitVec[i]
				,crHuman->solids[R_HEEL_S]->GetRotation()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());
		}
		else{
			crHuman->solids[L_HEEL_S]->AddForce(unitVec[i-3]
				,crHuman->solids[L_HEEL_S]->GetRotation()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());
		}

		// 各関節トルクに0を入れる
		for(int i = 0; i < JOINT_NUM; i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}

Vec3d CRBalanceQuadPR::GetJointAccel(PHJoint1D* joint){
	// すべてWorld座標系
	Vec3d soPosAccel = joint->GetSolidAccel();					// 子のSolidの加速度
	Vec3d soRotAccel = joint->GetSolidAngularAccel();			// 子のSolidの角加速度
	Vec3d soPosVelocity = joint->GetSolidVelocity();			// 子のSolidの速度
	Vec3d soRotVelocity = joint->GetSolidAngularVelocity();		// 子のSolidの角速度
	Vec3d joPos = joint->solid->GetRotation() 
			* joint->GetPostureFromChild().Pos();				// Jointの位置

	// Jointの加速度
	Vec3d relPos = joPos - joint->solid->GetCenterPosition();
	Vec3d joPosAccel = soPosAccel + soRotVelocity % (soPosVelocity + soRotVelocity%relPos)
						+ soRotAccel % relPos;
	return joPosAccel;
}

void CRBalanceQuadPR::SetQuadPR(CRHuman* crHuman){

	// 重心から足首への距離
	Vec3d cog =crHuman->GetCOG();
	Vec3d posCog_AnkleR = ((crHuman->joints[R_ANKLE_J]->solid->GetRotation()
				* crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos()) + crHuman->joints[R_ANKLE_J]->solid->GetCenterPosition()) - cog;
	Vec3d posCog_AnkleL = ((crHuman->joints[L_ANKLE_J]->solid->GetRotation()
				* crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos()) + crHuman->joints[L_ANKLE_J]->solid->GetCenterPosition()) - cog;

	// D'の設定
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempD = - transmissionD.inv();

	// Kの設定
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempK;
	tempK.sub_matrix(0,0, PTM::TMatDim<3,6>()) = tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())
												+ tempD.sub_matrix(3,0, PTM::TMatDim<3,6>());
	tempK.sub_matrix(3,0, PTM::TMatDim<3,6>()) = Matrix3d::Cross(posCog_AnkleR)*tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())
												+ Matrix3d::Cross(posCog_AnkleL)*tempD.sub_matrix(3,0, PTM::TMatDim<3,6>());

	// Aの設定
	PTM::TMatrixRow<DOF_NUM,JOINT_NUM,double> tempA;
	tempA = tempK*transmissionM;

	// 各関節の目標トルク T0
	//PTM::TVector<JOINT_NUM,double> targetTorqe;
	for(int i = 0; i < JOINT_NUM; i++){
		targetTorqe[i] = crHuman->joints[i]->GetTorque();
	}

	// Qの設定
	PTM::TMatrixRow<JOINT_NUM,JOINT_NUM,double> quadMatQ;
	quadMatQ = tempA.trans()*tempA;

	for(int i = 0; i < JOINT_NUM; i++){
		for(int j = 0; j < JOINT_NUM; j++){
			//quadPR.matQ[i][j] = quadMatQ[i][j];
			
			if(i == j){
				quadPR.matQ[i][j] = quadMatQ[i][j] + coefficient;
			}
			else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
			
		}
	}

	// Cの設定
	PTM::TVector<JOINT_NUM,double> quadMatC;
	PTM::TVector<DOF_NUM,double> targetF;
	targetF.sub_vector(0,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetForce();
	targetF.sub_vector(3,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetTorque();
	//quadMatC = targetF*tempA - (tempK*transmissionC)*tempA;
	quadMatC = targetF*tempA - (tempK*transmissionC)*tempA + coefficient*targetTorqe;

/*	//結果は上と同じだった
	PTM::TVector<DOF_NUM,double> targetFF;
	targetFF.sub_vector(0,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetForce();
	targetFF.sub_vector(3,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetTorque();
	PTM::TMatrixRow<DOF_NUM,1,double> targetF;
	targetF.col(0) = targetFF;
	PTM::TMatrixRow<DOF_NUM,1,double> coc;
	coc.col(0) = transmissionC;
	quadMatC = targetF.trans()*tempA - (tempK*coc).trans()*tempA;
*/
	for(int i = 0; i < JOINT_NUM; i++){
		quadPR.vecC[i] = quadMatC[i];
	}

}

void CRBalanceQuadPR::QuadPRSolve(){
	timer.Start();
	count = quadPR.Solve();
	DSTR << "二次計画法の計算："<< timer.Stop() << "[μs]" << std::endl;;
	timer.Clear();
	DSTR << "試行回数：" << count << std::endl;
}

void CRBalanceQuadPR::Init(SGScene* scene,CRHuman* crHuman){
	
	CRBalanceQuadPR();

	// 二次計画法の次元設定
	quadPR.SetDim(JOINT_NUM);

	// 制約条件の設定
	TVector<JOINT_NUM, float> vmin;
	TVector<JOINT_NUM, float> vmax;
	for(int i=0; i<JOINT_NUM; ++i){
		vmin[i] = -1000000;
		//vmin[i] = 0.001;
		vmax[i] = 1000000;
	}
	quadPR.Init(vmin, vmax);
	
	scene->GetBehaviors().Find(jointEngine);
	scene->GetBehaviors().Find(gravity);
	

}

void CRBalanceQuadPR::SetJointTorque(CRHuman* crHuman){
	for(int i = 0 ; i < JOINT_NUM; i++){
		crHuman->joints[i]->SetTorque(quadPR.vecX[i]);
	}
}

void CRBalanceQuadPR::SetAnkleForce(CRHuman* crHuman){

	Vec3d rForce,lForce;

	PTM::TVector<JOINT_NUM,double> optToruqe;
	for(int i = 0; i < JOINT_NUM; i ++){
		optToruqe[i] = quadPR.vecX[i];
	}

	// D'の設定
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempD = - transmissionD.inv();

	rForce = tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())*transmissionM*optToruqe
							+ tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())*transmissionC;

	lForce = tempD.sub_matrix(3,0, PTM::TMatDim<3,6>())*transmissionM*optToruqe
							+ tempD.sub_matrix(3,0, PTM::TMatDim<3,6>())*transmissionC;

	// 足首に力を加える
	Vec3d grav = crHuman->GetTotalMass()*gravity->accel;
	crHuman->solids[R_HEEL_S]->AddForce(rForce
			,crHuman->solids[R_HEEL_S]->GetRotation()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());

	crHuman->solids[L_HEEL_S]->AddForce(lForce
			,crHuman->solids[L_HEEL_S]->GetRotation()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());

}

void CRBalanceQuadPR::DataOut(CRHuman* crHuman){

	std::ofstream of("quadPR.xls");

	// 関節の名前
	for(int i = 0 ; i < JOINT_NUM; i++){
		if(crHuman->joints[i] != NULL){
			of << crHuman->joints[i]->GetName() << "\t";
		}
	}
	of << std::endl;

	// 各関節の目的トルク(t0)
	of << std::endl;
	of << "各関節の目的トルク" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		of << targetTorqe[i] << "\t";
	}
	of << std::endl;
	// 目的関数の値
	//of << " 目的関数の値" << std::endl;
	//of << quadPR.vecX*quadPR.matQ*quadPR.vecX-2*quadPR.vecC << std::endl;
	//DSTR << quadPR.vecX*quadPR.matQ*quadPR.vecX-2*quadPR.vecC*quadPR.vecX << std::endl;

	// 最適解の出力
	of << std::endl;
	of << "最適解" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Qの出力
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		for(int j = 0 ; j < JOINT_NUM; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// Cの出力
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
			of << quadPR.vecC[i] << "\t";
	}
	of << std::endl;
	
	//　伝播行列M
	of << std::endl;
	of << "伝播行列M" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		for(int j = 0 ; j < JOINT_NUM; j++){
			of << transmissionM[i][j] << "\t";
		}
		of << std::endl;
	}
	
	//　伝播行列D
	of << std::endl;
	of << "伝播行列D" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		for(int j = 0 ; j < DOF_NUM; j++){
			of << transmissionD[i][j] << "\t";
		}
		of << std::endl;
	}
	
	
	//　伝播行列C
	of << std::endl;
	of << "伝播行列C" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		of << transmissionC[i] << "\t";
	}
	
	of.close();
}

}		//	end namespace Spr