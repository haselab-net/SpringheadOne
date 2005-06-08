#include "Creature.h"
#pragma hdrstop
// CRBalanceQuadPR.cpp: CRBalanceQuadPR クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRBalanceQuadPR.h"
#include "CRHuman.h"
#include "CRVirtualHuman.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;
using namespace std;

CRBalanceQuadPR::CRBalanceQuadPR()
{

}

CRBalanceQuadPR::~CRBalanceQuadPR()
{

}

bool CRBalanceQuadPR::CompNoJoint(int jointNum,CRHuman* crHuman){
	for(int i = 0; i < crHuman->noUseJoint.size(); i++){
		if(jointNum == crHuman->noUseJoint[i])
			return true;
	}
	return false;
}

void CRBalanceQuadPR::Init(SGScene* scene,CRHuman* crHuman){
	// 親クラスの初期化
	CRBalance::Init(scene,crHuman);

	// 単位ベクトルの初期化
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));


	//　バランスに使用する関節の数
	unsigned int all_jo_num = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL)
			all_jo_num++;
	}
	useJoNum = all_jo_num - crHuman->noUseJoint.size();
	// 伝播行列のサイズの設定&初期化
	transT.resize(ANKLE_DOF*ANKLE_NUM, useJoNum);
	transF.resize(ANKLE_DOF*ANKLE_NUM,ANKLE_DOF*ANKLE_NUM);
	transM.resize(ANKLE_DOF*ANKLE_NUM);
	transT.clear();
	transF.clear();
	transM.clear();

	tempA.resize(BALANCE_DOF, useJoNum);
	tempB.resize(BALANCE_DOF);
	tempA.clear();
	tempB.clear();

	// バランスに使用する関節の目標トルクのサイズの設定&初期化
	targetTorque.resize(useJoNum);
	targetTorqueP.resize(useJoNum);
	targetTorqueD.resize(useJoNum);
	targetTorque.clear();
	targetTorqueP.clear();
	targetTorqueD.clear();

	// coefficientの初期化
	coefficientJoint.resize(useJoNum);
	coefficientJoint.clear();
	coefficient = 0;
	coefficientP = 0;
	coefficientD = 0;

	// 最適トルクの初期化
	optTorque.resize(useJoNum);
	optTorque.clear();

	count = 0;
	qdprBalanceRate = 0;
	targetX.clear();
	bAdjust = false;

}

void CRBalanceQuadPR::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficientの設定
	coefficient = 1.0f;
	coefficientP = 0.3f;
	coefficientD = 0.3f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJoint[i] = 0.01f;
	}
	/*
	//膝をよく使う
	coefficientJoint[useJoNum-5] = -0.5f;
	coefficientJoint[useJoNum-13] = -0.5f;
	//足首をよく使う
	coefficientJoint[useJoNum-4] = -0.3f;
	coefficientJoint[useJoNum-3] = -0.3f;
	coefficientJoint[useJoNum-2] = -0.3f;
	coefficientJoint[useJoNum-12] = -0.3f;
	coefficientJoint[useJoNum-11] = -0.3f;
	coefficientJoint[useJoNum-10] = -0.3f;
	*/
	
	// 二次計画法の割合
	qdprBalanceRate = 1.0f;

	// 制約条件の設定
	PTM::VVector<float> vmin;
	PTM::VVector<float> vmax;
	vmin.resize(useJoNum);
	vmax.resize(useJoNum);
	for(int i=0; i<useJoNum; ++i){
		vmin[i] = -1000;
		vmax[i] = 1000;
	}
	quadPR.Init(vmin, vmax);

}

void CRBalanceQuadPR::ModifyBody(SGScene* scene,CRHuman* crHuman){

	///////　二次計画法でバランス　///////
	// VHの接触力計算
	crHuman->CalcContactForce();

	// 目的(到達運動)トルクのセット
	SetTargetJointTorque(scene,crHuman);

	// 伝播行列の計算
	static int i = 10;
	if(i/10){
		CalcTransMatrix(scene,crHuman);
		i = 0;
	}
	i++;
	// 二次計画法の式をセット
	SetQuadPR(crHuman);

	// 二次計画法を解く
	QuadPRSolve();

	// 最適トルクをセット(足首の保障も)
	SetBalanceTorque(crHuman);

	// 制約条件の更新
	//RenewConstraint(crHuman);

	// データ出力
	//DataOut(crHuman);

}

void CRBalanceQuadPR::SetTargetJointTorque(SGScene* scene,CRHuman* crHuman){
	// 各関節の目標トルク T0,T0のP,D成分の取得
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			if(CompNoJoint(i,crHuman))
				add++;
			else{
				targetTorque[i-add] = crHuman->joints[i]->GetJointTorque(0);
				targetTorqueP[i-add] = crHuman->jointPids[i]->GetProportionalTorque();
				targetTorqueD[i-add] = crHuman->jointPids[i]->GetDifferentialTorque();
			}
		}
		else
			add ++;
	}
}

void CRBalanceQuadPR::CalcTransMatrix(SGScene* scene,CRHuman* crHuman){
	timer.Start();

	// 踵の剛体の力、トルクを退避
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetForce();
	AnkleForce[1] = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetForce();
	AnkleTorque[0] = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetTorque();
	AnkleTorque[1] = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// フェザーストーンの一部を使い、左右足首最後のJointの加速度計算
		// 右足首
		crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->CalcAccel(scene->GetTimeStep());
		// 左足首
		crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->CalcAccel(scene->GetTimeStep());
		
		// 両足首の加速度
		static PTM::TVector<ANKLE_DOF*ANKLE_NUM,double> accelVec;
		// 右足
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAngularAccel();
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAccel();
		// 左足
		accelVec.sub_vector(6,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAngularAccel();
		accelVec.sub_vector(9,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAccel();


		//  係数行列に代入
		// 伝播行列M
		if(i==0){
			transM = accelVec;
		}
		// 伝播行列T
		else if(i > 0 && i <= useJoNum){
			transT.col(i-1) = accelVec - transM;
		}
		// 伝播行列F
		else if(i > useJoNum){
			transF.col(i - (useJoNum+1)) = accelVec - transM;
		}

		// 踵の力、トルクを戻す
		crHuman->solids[crHuman->supportSolidNum[0][0]]->SetForce(AnkleForce[0]);
		crHuman->solids[crHuman->supportSolidNum[1][0]]->SetForce(AnkleForce[1]);
		crHuman->solids[crHuman->supportSolidNum[0][0]]->SetTorque(AnkleTorque[0]);
		crHuman->solids[crHuman->supportSolidNum[1][0]]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
//	DSTR << "伝播行列の計算時間："<< transTime << "[ms]" << std::endl;;
}

Vec3d CRBalanceQuadPR::GetJointAccel(PHJoint1D* joint){
	// すべてWorld座標系
	Vec3d soPosAccel = joint->GetSolidAccel();					// 子のSolidの加速度
	Vec3d soRotAccel = joint->GetSolidAngularAccel();			// 子のSolidの角加速度
	Vec3d soPosVelocity = joint->GetSolidVelocity();			// 子のSolidの速度
	Vec3d soRotVelocity = joint->GetSolidAngularVelocity();		// 子のSolidの角速度
	Vec3d joPos = joint->solid->GetFrame()->GetPosture()
			* joint->GetPostureFromChild().Pos();				// Jointの位置
	//	関節位置：回転はWorld系で，Solid重心が原点
	Vec3d relPos = joPos - joint->solid->GetCenterPosition();

	// Jointの加速度
	Vec3d joPosAccel = soPosAccel + soRotVelocity % (soPosVelocity + soRotVelocity%relPos)
						+ soRotAccel % relPos;
	return joPosAccel;
}

void CRBalanceQuadPR::MakeTorquePattern(CRHuman* crHuman, int t){
/////	関節のトルク、足首にかかる力すべて0	  //////
	if(t == 0){
		// 各関節トルクを0にする
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				crHuman->joints[i]->SetJointTorque(0.0, 0);
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

/////// トルクに１がはいる　/////////////
	// 各関節トルクに1を入れる
	else if(t > 0 && t <= useJoNum){
		unsigned int add = 0;
		for(unsigned int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				if(CompNoJoint(i,crHuman)){
					add++;
				}
				if(i == t-1+add){
					if(CompNoJoint(i,crHuman)){
						crHuman->joints[i]->SetJointTorque(0.0, 0);
					}
					else{
						if(crHuman->joints[i] != NULL){
							crHuman->joints[i]->SetJointTorque(1.0, 0);
						}
					}
				}
				else{
					if(crHuman->joints[i] != NULL){
						crHuman->joints[i]->SetJointTorque(0.0, 0);
					}
				}
			}
			else{
				add++;
			}
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

//////	足首にかかる力が１はいる	///////
	else if(t > useJoNum){
		// 足首にかかる力に１を入れる
		int i = t - (useJoNum + 1);
		// 右足
		if(i < 3){
			crHuman->solids[crHuman->supportSolidNum[0][0]]->AddTorque(unitVec[i]);
		}
		else if(i >=3 && i < 6){
			crHuman->solids[crHuman->supportSolidNum[0][0]]->AddForce(unitVec[i-3]);
		}
		// 左足
		else if(i >=6 && i < 9){
			crHuman->solids[crHuman->supportSolidNum[1][0]]->AddTorque(unitVec[i-6]);	
		}
		else{
			crHuman->solids[crHuman->supportSolidNum[1][0]]->AddForce(unitVec[i-9]);
		}


		// 各関節トルクに0を入れる
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->SetJointTorque(0.0, 0);
			}
		}
	}
}

void CRBalanceQuadPR::SetQuadPR(CRHuman* crHuman){

	// 重心から足首への距離（伝播行列を求める前と後でも値変わらず）
	Vec3d cog =crHuman->GetCOG();
	Vec3d posCog_FootR = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetCenterPosition() - cog;
	Vec3d posCog_FootL = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetCenterPosition() - cog;


	// T_f'の設定
	static PTM::TMatrixRow<ANKLE_DOF*ANKLE_NUM,ANKLE_DOF*ANKLE_NUM,double> tempT_f;
	tempT_f = - transF.inv();

	static PTM::TMatrixRow<3,12,double> tempT_T_R;	//右足トルク
	tempT_T_R = tempT_f.sub_matrix(0,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_F_R;	//右足力
	tempT_F_R= tempT_f.sub_matrix(3,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_T_L;	//左足トルク
	tempT_T_L= tempT_f.sub_matrix(6,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_F_L;	//左足力
	tempT_F_L= tempT_f.sub_matrix(9,0, PTM::TMatDim<3,12>());

	
	// Tの設定
	static PTM::TMatrixRow<BALANCE_DOF,ANKLE_DOF*ANKLE_NUM,double> tempT;
	tempT.sub_matrix(0,0, PTM::TMatDim<3,12>()) = tempT_T_R + tempT_T_L;
	tempT.sub_matrix(3,0, PTM::TMatDim<3,12>()) = tempT_F_R + tempT_F_L;
	//tempT *= -1;
	/*
	// Tの設定
	static PTM::TMatrixRow<BALANCE_DOF,ANKLE_DOF*ANKLE_NUM,double> tempT;
	tempT.sub_matrix(0,0, PTM::TMatDim<3,12>()) = tempT_T_R + tempT_T_L + Matrix3d::Cross(posCog_FootR)*tempT_F_R + Matrix3d::Cross(posCog_FootL)*tempT_F_L;
	tempT.sub_matrix(3,0, PTM::TMatDim<3,12>()) = tempT_F_R + tempT_F_L;
	*/
	// Aの設定
	tempA = tempT*transT;

	// bの設定
	static PTM::TVector<BALANCE_DOF,double> f_mass;		// 重力
	f_mass.sub_vector(0,PTM::TVecDim<3>()) = Vec3d();
	f_mass.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;

	static PTM::TVector<BALANCE_DOF,double> f_contact;	// 接触力
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactTorque()-crHuman->GetCOG()%crHuman->supportArea.GetContactForce();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactForce();

	tempB = tempT*transM + f_mass + f_contact;


	// Qの設定
	PTM::VMatrixRow<double> quadMatQ;
	quadMatQ.resize(useJoNum,useJoNum);
	quadMatQ = tempA.trans()*tempA;
	for(int i = 0; i < useJoNum; i++){
		for(int j = 0; j < useJoNum; j++){
			if(i == j){
				quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientJoint[i] + coefficientP + coefficientD;
			}else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
		}
	}

	// Cの設定
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJoNum);
	targetX.sub_vector(0,PTM::TVecDim<3>()) = qdprBalanceRate*GetBalanceTorque();
	//targetX.sub_vector(3,PTM::TVecDim<3>()) = GetBalanceForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	targetX.sub_vector(3,PTM::TVecDim<3>()) = GetBalanceForce();
	quadMatC = (targetX - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	quadPR.vecC = quadMatC;

}

void CRBalanceQuadPR::QuadPRSolve(){
	timer.Start();
	// 二次計画法を解く
	count = quadPR.Solve();
	quadTime = timer.Stop()*1e-3;
	timer.Clear();
	//DSTR << "二次計画法の計算："<< quadTime << "[ms]" << std::endl;;
	//DSTR << "試行回数：" << count << std::endl;

	// 最適トルク
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}

void CRBalanceQuadPR::SetBalanceTorque(CRHuman* crHuman){
	// 最適トルクを体のバランスに使用した関節にセットする
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			if(CompNoJoint(i,crHuman)){
				add++;
			}
			else{
				crHuman->joints[i]->SetJointTorque(quadPR.vecX[i-add], 0);
			}
		}
		else{
			add++;
		}
	}
	// 足のZMPが足裏から出ないように足首トルクを調節
	AnkleBalance(crHuman);
	//AnkleBalance2(crHuman);
}

void CRBalanceQuadPR::AnkleBalance(CRHuman* crHuman){

	// 両足首の最適トルク(ローカル)→ワールド
	Vec3d balanceAnkleTorqueR = Vec3d(crHuman->joints[27]->GetJointTorque(0),-crHuman->joints[29]->GetJointTorque(0),crHuman->joints[28]->GetJointTorque(0));
	Vec3d balanceAnkleTorqueL = Vec3d(crHuman->joints[35]->GetJointTorque(0),crHuman->joints[37]->GetJointTorque(0),-crHuman->joints[36]->GetJointTorque(0));
	//Vec3d balanceAnkleTorqueL = Vec3d(crHuman->joints[34]->GetJointTorque(0),crHuman->joints[36]->GetJointTorque(0),-crHuman->joints[35]->GetJointTorque(0));
	balanceAnkleTorqueR = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()*balanceAnkleTorqueR;
	balanceAnkleTorqueL = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()*balanceAnkleTorqueL;

	// 両足首にかかる力(接触力)
	Vec3d ankleForceR = crHuman->supportObject[0].GetContactForce();
	Vec3d ankleForceL = crHuman->supportObject[1].GetContactForce();

	// 足が浮かないようにトルクを調節(ZMP計算→足裏領域に入れる→そのためのトルク)
	Vec3d addAnkleTorqueR = AdjustAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
	Vec3d addAnkleTorqueL = AdjustAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);

	//右足首の座標へ変換
	Vec3f AnkleTorqueR = (crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()).inv()*addAnkleTorqueR;
	crHuman->joints[crHuman->ankleJointNum[0][0]]->SetJointTorque(AnkleTorqueR.x, 0);
	crHuman->joints[crHuman->ankleJointNum[0][1]]->SetJointTorque(AnkleTorqueR.z, 0);
	crHuman->joints[crHuman->ankleJointNum[0][2]]->SetJointTorque(-AnkleTorqueR.y, 0);

	//左足首の座標へ変換
	Vec3f AnkleTorqueL = (crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()).inv()*addAnkleTorqueL;
	crHuman->joints[crHuman->ankleJointNum[1][0]]->SetJointTorque(AnkleTorqueL.x,0);
	crHuman->joints[crHuman->ankleJointNum[1][1]]->SetJointTorque(-AnkleTorqueL.z,0);
	crHuman->joints[crHuman->ankleJointNum[1][2]]->SetJointTorque(AnkleTorqueL.y,0);

}

Vec3d CRBalanceQuadPR::AdjustAnkleTorque(CRHuman* crHuman,Vec3d ankleForce,Vec3d ankleTorque,int ankle){

	// ZMPを計算
	ankleGoalZmp[ankle] = (ankleForce % -ankleTorque) / ankleForce.square();
	ankleGoalZmp[ankle] += crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetFrame()->GetPosture()
							*crHuman->joints[crHuman->ankleJointNum[ankle][2]]->GetPostureFromChild().Pos();
	ankleGoalZmp[ankle].Y() = 0.0f;

	// 片足のZMPが片足の接触多角形の中にあるかどうか、ない場合は最近傍点を求める
	if(!AnkleZmpCheck(ankleGoalZmp[ankle],&crHuman->supportObject[ankle])){
		// 最近傍点を求める
		ankleRealZmp[ankle]=crHuman->supportObject[ankle].GetSupportArea().ClosestPoint(ankleGoalZmp[ankle]);
		ankleTorque = (ankleRealZmp[ankle] - crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetFrame()->GetPosture()
						*crHuman->joints[crHuman->ankleJointNum[ankle][2]]->GetPostureFromChild().Pos())% ankleForce;
		
		return -ankleTorque;
	}else{
		ankleRealZmp[ankle] = ankleGoalZmp[ankle];
	}
	return ankleTorque;
}

bool CRBalanceQuadPR::AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea){

	bool zmp_in = false;
	std::vector<Vec3f> supportVetxs = supportArea->GetSupportArea();
	if(supportVetxs.size() > 3){
		for(int l = 0; l < supportVetxs.size() - 2  && !zmp_in; ++l){
			for(int m = l+1; m < supportVetxs.size() - 1 && !zmp_in; ++m){
				for(int n = m+1; n < supportVetxs.size() && !zmp_in; ++n){
					zmp_in = PosIncludeTriangle(supportVetxs[l],supportVetxs[m], supportVetxs[n], zmp);
				}
			}
		}
	}
	return zmp_in;
}

Vec3f CRBalanceQuadPR::CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp){

	Vec3f t = p0 - p1;
	float a = t.z/t.x;
	float b = p0.z - a*p0.x;
	float a_z = -1/a;
	float b_z = zmp.z-a_z*zmp.x;

	float x_cross = (b_z-b)/(a-a_z);

	if(p0.x	> p1.x){
		if(x_cross > p1.x && x_cross < p0.x){
			return Vec3f(x_cross, 0.0f , a*x_cross + b);
		}
		else{
			return Vec3f();
		}
	}
	else{
		if(x_cross > p0.x && x_cross < p1.x){
			return Vec3f(x_cross, 0.0f , a*x_cross + b);
		}
		else{
			return Vec3f();
		}
	}
}

void CRBalanceQuadPR::RenewConstraint(CRHuman* crHuman){

	// 右足首
	Vec3f maxRangeR,minRangeR;
	NewConstraint(crHuman,crHuman->solids[crHuman->supportSolidNum[0][0]],&maxRangeR,&minRangeR,0);
	// 左足首
	Vec3f maxRangeL,minRangeL;
	NewConstraint(crHuman,crHuman->solids[crHuman->supportSolidNum[1][0]],&maxRangeL,&minRangeL,1);

	/*
	quadPR.SetOneRange(27,20,-50);
	quadPR.SetOneRange(28,17,-17);
	quadPR.SetOneRange(34,20,-50);
	quadPR.SetOneRange(35,17,-17);
	*/
	/*		
	// 制約条件の設定
	// 右足首
	quadPR.SetOneRange(27,maxRangeR.x,minRangeR.x);
	quadPR.SetOneRange(28,maxRangeR.z,minRangeR.z);

	// 左足首
	quadPR.SetOneRange(34,maxRangeL.x,minRangeL.x);
	quadPR.SetOneRange(35,maxRangeL.z,minRangeL.z);
	*/
	
/*
	// 制約条件の設定
	PTM::VVector<float> vmin;
	PTM::VVector<float> vmax;
	vmin.resize(useJoNum);
	vmax.resize(useJoNum);
	for(int i=0; i<useJoNum; ++i){
		vmin[i] = -1000;
		vmax[i] = 1000;
	}
	vmax[27] = maxRangeR.x;
	vmin[27] = minRangeR.x;
	vmax[28] = maxRangeR.z;
	vmin[28] = minRangeR.z;

	vmax[34] = maxRangeL.x;
	vmin[34] = minRangeL.x;
	vmax[35] = maxRangeL.z;
	vmin[35] = minRangeL.z;

	quadPR.Init(vmin, vmax);
*/
	
}
void CRBalanceQuadPR::NewConstraint(CRHuman* crHuman,PHSolid* heel,Vec3f* maxRange,Vec3f* minRange,int ankleSide){

	Vec3f ankleJointPos = crHuman->joints[crHuman->ankleJointNum[ankleSide][2]]->GetPostureFromChild().Pos();
	//ankleJointPos.y = 0.0f;

	std::vector<Vec3f> supportVetxs = crHuman->supportObject[ankleSide].GetSupportArea();
	for(int i = 0; i < supportVetxs.size(); i++){
		supportVetxs[i] = heel->GetFrame()->GetPosture().inv()*supportVetxs[i];
		supportVetxs[i].y = 0.0f;
	}
	double max = 0;
	double min = 100000.0;
	int maxNum = 0;
	int minNum = 0;
	
	// 一番近い&遠い安定領域の頂点
	if(supportVetxs.size() > 2){
		for(int i = 0; i < supportVetxs.size(); i++){
			if(max < (supportVetxs[i] - ankleJointPos).norm()){
				max = (supportVetxs[i] - ankleJointPos).norm();
				maxNum = i;
			}
			if(min > (supportVetxs[i] - ankleJointPos).norm()){
				min = (supportVetxs[i] - ankleJointPos).norm();
				minNum = i;
			}
		}
	}

	/*
	Vec3f maxTorque = (supportVetxs[maxNum] - ankleJointPos)%crHuman->supportObject[ankleSide].GetContactForce();
	Vec3f minTorque = (supportVetxs[minNum] - ankleJointPos)%crHuman->supportObject[ankleSide].GetContactForce();
	*/
	Vec3f force = -crHuman->GetTotalMass()*gravityEngine->accel/2;
	Vec3f maxTorque = (supportVetxs[maxNum] - ankleJointPos)%force;
	Vec3f minTorque = (supportVetxs[minNum] - ankleJointPos)%force;

	maxRange->x = -std::min(maxTorque.x,minTorque.x);
	minRange->x = -std::max(maxTorque.x,minTorque.x);

	if(!ankleSide){	//右足
		maxRange->z = -std::min(maxTorque.z,minTorque.z);
		minRange->z = -std::max(maxTorque.z,minTorque.z);
	}
	else{			//左足
		maxRange->z = std::max(maxTorque.z,minTorque.z);
		minRange->z = std::min(maxTorque.z,minTorque.z);
	}
	DSTR << ankleSide <<"maxTorque" << *maxRange <<std::endl;
	DSTR << ankleSide <<"minTorque" << *minRange <<std::endl;


}

bool CRBalanceQuadPR::BalanceCheck(CRHuman* crHuman){

	if(abs(crHuman->solids[0]->GetOrientation().rotation().x) >0.8){
		return false;
	}
	if(abs(crHuman->solids[0]->GetOrientation().rotation().z) >0.8){
		return false;
	}
	return true;
}

void CRBalanceQuadPR::OffSpring(){
	// 重心の目標加速度(バランスが取れた状態の）
	targetAccel = Vec3f(0.0,0.0,0.0);
	//目標姿勢の設定
	targetPostureQ = Quaterniond(1.0,0.0,0.0,0.0);

	// バランストルク計算のバネ・ダンパ設定
	//dampa_no = 0.0003f*19;
	dampa_no = 0.00002f;
	for(int i = 0; i < 3; i++)
		spring_noV[i] = 0.00001f;
		//spring_noV[i] = 0.0001f*0.3f;

	// 重心を安定領域内にする力のバネ・ダンパの設定
	//springCog_no	= 0.000005f*0.3f;
	//springCog_no	= 0.000005f;
	springCog_no	= 0.0000007f;
	//dampaCog_no		= 0.000015f*19;
	//dampaCog_no		= 0.00015f*3;
	dampaCog_no		= 0.00015f*260;

}

void CRBalanceQuadPR::DataOut(CRHuman* crHuman){

	static bool bFirst = true;
	std::ofstream of;
	if (bFirst){
		of.open("quadPR.xls");
		bFirst = false;
		
		for(int i = 0 ; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				of << crHuman->joints[i]->GetName() << "\t" ;
		}of << std::endl;
	}else{
		of.open("quadPR.xls",ios::app);
	}
/*
	//バランスに必要な力
	of << "バランスに必要なトルクと力" << std::endl;
	for(int i = 0; i < targetX.size(); i++)
		of << targetX[i] << "\t";
	of << std::endl;

	//　最適解が重心にかける力とトルク
	of << "最適解が重心にかけるトルクと力" << std::endl;
	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	for(int i =0; i < BALANCE_DOF; i++)
		of << x_cog[i] << "\t";
	of << std::endl;


	//実際の重心のトルク
	of << "実際のトルクと力" << std::endl;
	Vec3f realTorque = crHuman->supportArea.GetContactTorque()-crHuman->GetCOG()%crHuman->supportArea.GetContactForce();;
	for(int i =0; i < 3; i++)
		of << realTorque[i] << "\t";
	
	//実際の重心の力
	Vec3f realForce = crHuman->supportArea.GetContactForce() + crHuman->GetTotalMass()*gravityEngine->accel;
	for(int i =0; i < 3; i++)
		of << realForce[i] << "\t";
	of<<std::endl;

	DSTR << "トルクの割合";
	for(int i = 0; i < 3; i ++){
		DSTR << abs(realTorque[i])/abs(targetX[i]) << "\t";
	}
	DSTR << std::endl;
	DSTR << "力の割合";
	for(int i = 0; i < 3; i ++){
		DSTR << abs(realForce[i])/abs(targetX[i+3]) << "\t";
	}
	DSTR << std::endl;




	// 最適解の出力
	of << "最適解" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}
	of << std::endl;
*/
	/*
	// 伝播行列の計算時間
	of << transTime << std::endl;
	// 二次計画法の計算時間
	//of << quadTime << std::endl;
	*/
	/*
	// 関節の名前
	static int kk = 0;
	if(!kk){
		for(int i = 0 ; i < crHuman->joints.size(); i++){
			if(!CompNoJoint(i)){
				if(crHuman->joints[i] != NULL){
					of << crHuman->joints[i]->GetName() << "\t" ;
				}
				of << "\t"<< "\t"<< "\t";
				of << "\t"<< "\t"<< "\t";
			}
		}
		of << std::endl;
		kk++;
	}

	for(int i = 0; i < tempA.width(); i++){
		for(int j = 0; j < tempA.height(); j++){
			of << tempA[j][i] << "\t";
		}
		of << "\t";
	}
	of << std::endl;
	*/

	/*
	// 各関節の目的トルク(t0)
	of << std::endl;
	of << "各関節の目的トルク" << std::endl;
	for(int i = 0 ; i < targetTorque.size(); i++){
		of << targetTorque[i] << "\t";
	}
	of << std::endl;

	// 最適解の出力
	of << std::endl;
	of << "最適解" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Qの出力
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		for(int j = 0 ; j < useJoNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// Cの出力
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
			of << quadPR.vecC[i] << "\t";
	}
*/
	of << std::endl;	
	//　伝播行列T
	of << std::endl;
	of << "伝播行列T" << std::endl;
	for(int i = 0 ; i < transT.height(); i++){
		for(int j = 0 ; j < transT.width(); j++){
			of << transT[i][j] << "\t";
		}
		of << std::endl;
	}

	//　伝播行列F
	of << std::endl;
	of << "伝播行列F" << std::endl;
	for(int i = 0 ; i < transF.height(); i++){
		for(int j = 0 ; j < transF.width(); j++){
			of << transF[i][j] << "\t";
		}
		of << std::endl;
	}
	
	//　伝播行列M
	of << std::endl;
	of << "伝播行列M" << std::endl;
	for(int i = 0 ; i < transM.size(); i++){
		of << transM[i] << "\t";
	}

	of << std::endl;
	of.close();
}

void CRBalanceQuadPR::AnkleBalance2(CRHuman* crHuman){

	// 最適解によって足(重心)にかかるトルクと力
	PTM::VVector<float> x_foot;
	x_foot.resize(ANKLE_DOF*ANKLE_NUM);
	x_foot = - transF.inv()*(transT*optTorque+transM);

	// 最適解によって右足にかかるトルクと力
	Vec3d footR_T_J = x_foot.sub_vector(0,PTM::TVecDim<3>());
	Vec3d footR_F_J = x_foot.sub_vector(3,PTM::TVecDim<3>());
	// 最適解によって左足にかかるトルクと力
	Vec3d footL_T_J = x_foot.sub_vector(6,PTM::TVecDim<3>());
	Vec3d footL_F_J = x_foot.sub_vector(9,PTM::TVecDim<3>());


	// 右足にかかる全てのトルクと力
	Vec3d footR_T = footR_T_J + crHuman->solids[crHuman->supportSolidNum[0][0]]->GetCenterPosition()
							%(crHuman->solids[crHuman->supportSolidNum[0][0]]->GetMass()*gravityEngine->accel);
	Vec3d footR_F = footR_F_J + crHuman->solids[crHuman->supportSolidNum[0][0]]->GetMass()*gravityEngine->accel;
	// 左足にかかる全てのトルクと力
	Vec3d footL_T = footL_T_J + crHuman->solids[crHuman->supportSolidNum[1][0]]->GetCenterPosition()
							%(crHuman->solids[crHuman->supportSolidNum[1][0]]->GetMass()*gravityEngine->accel);
	Vec3d footL_F = footR_F_J + crHuman->solids[crHuman->supportSolidNum[1][0]]->GetMass()*gravityEngine->accel;


	// 右足のZMP
	footR_zmp = (footR_F%footR_T)/footR_F.square();
	//footR_zmp +=crHuman->solids[crHuman->supportSolidNum[0][0]]->GetCenterPosition();
	// 左足のZMP
	footL_zmp = footL_F%footL_T/footL_F.square();
	//footL_zmp +=crHuman->solids[crHuman->supportSolidNum[1][0]]->GetCenterPosition();


	//右足の修正
	if(!AnkleZmpCheck(footR_zmp,&crHuman->supportObject[0])){
		footR_zmp = AdjustAnkleTorque2(crHuman,footR_F,footR_T,footR_zmp,0);
	}
	//左足の修正
	if(!AnkleZmpCheck(footL_zmp,&crHuman->supportObject[1])){
		footL_zmp = AdjustAnkleTorque2(crHuman,footL_F,footL_T,footL_zmp,1);
	}

}
Vec3d CRBalanceQuadPR::AdjustAnkleTorque2(CRHuman* crHuman,	Vec3d footForce,Vec3d footTorque,Vec3d zmp,int foot){

	// 最近傍点
	zmp = crHuman->supportObject[foot].GetSupportArea().ClosestPoint(zmp);

	// 修正足トルクの計算
	Vec3d newFootTorque = zmp%footForce;

	// 実際に足にかかっていたトルクと修正トルクの差
	Vec3d difFootTorque = footTorque - newFootTorque;

	DSTR << foot <<"footTorque" <<difFootTorque <<std::endl;
	DSTR << foot <<"difFootTorque" <<difFootTorque <<std::endl;

	// 伝播行列の抜き出し
	static PTM::VMatrixRow<float> tempFT;
	tempFT.resize(ANKLE_DOF*ANKLE_NUM, useJoNum);
	tempFT = (-transF.inv())*transT;
	PTM::TMatrixRow<3,3,double> tempT_a;
	if(!foot){ // 右足
		tempT_a = tempFT.vsub_matrix(0,useJoNum-(11+1),3,3);
	}
	else{		// 左足
		tempT_a = tempFT.vsub_matrix(6,useJoNum-(3+1),3,3);
	}

	// 足首に加えるトルク
	Vec3d addAnkleTorque = -(tempT_a.inv()*difFootTorque);
	DSTR << foot <<"addAnkleTorque" <<addAnkleTorque <<std::endl;

	DSTR << "balanceTorque" <<balanceTorque <<std::endl;

	if(bAdjust){
		// 足首にトルクを加える(足首関節の順番がx,z,yだが，addAnkleTorqueもその順番なのでそのまま)
		if(!foot){ // 右足
			crHuman->joints[crHuman->ankleJointNum[0][0]]->AddJointTorque(addAnkleTorque.x,0);
			crHuman->joints[crHuman->ankleJointNum[0][1]]->AddJointTorque(addAnkleTorque.y,0);
			crHuman->joints[crHuman->ankleJointNum[0][2]]->AddJointTorque(addAnkleTorque.z,0);
		}
		else{	// 左足
			crHuman->joints[crHuman->ankleJointNum[1][0]]->AddJointTorque(addAnkleTorque.x,0);
			crHuman->joints[crHuman->ankleJointNum[1][1]]->AddJointTorque(addAnkleTorque.y,0);
			crHuman->joints[crHuman->ankleJointNum[1][2]]->AddJointTorque(addAnkleTorque.z,0);
		}
	}
	return zmp;
}


}		//	end namespace Spr