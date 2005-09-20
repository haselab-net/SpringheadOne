// Balance.cpp: Balance クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Balance.h"
#include "Model.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

Balance::Balance()
{

}

Balance::~Balance()
{

}
void  Balance::RegistNoJoint(CRHuman* crHuman){
	noJoint.clear();
	noJoint.push_back(crHuman->joints.size()-1);	// 土踏まず
	noJoint.push_back(crHuman->joints.size()-2);	// 足首関節(-Y)
	noJoint.push_back(crHuman->joints.size()-3);	// 足首関節(Z)
	noJoint.push_back(crHuman->joints.size()-4);	// 足首関節(X)
}
bool Balance::CompNoJoint(int jointNum){
	for(int i = 0; i < noJoint.size(); i++){
		if(jointNum == noJoint[i])
			return true;
	}
	return false;
}
void Balance::Init(SGScene* scene,CRHuman* crHuman){
	CRBalance::Init(scene,crHuman);

	// 単位ベクトルの初期化
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// バランスに使わない関節を登録
	RegistNoJoint(crHuman);

	//　バランスに使用する関節の数
	useJoNum = crHuman->joints.size() - noJoint.size();

	// 伝播行列のサイズの設定&初期化
	transT.resize(ANKLE_DOF_*ANKLE_NUMBER, useJoNum);
	transF.resize(ANKLE_DOF_*ANKLE_NUMBER,ANKLE_DOF_*ANKLE_NUMBER);
	transM.resize(ANKLE_DOF_*ANKLE_NUMBER);
	transT.clear();
	transF.clear();
	transM.clear();

	// バランスに使用する関節の目標トルクのサイズの設定&初期化
	targetTorque.resize(useJoNum);
	targetTorqueP.resize(useJoNum);
	targetTorqueD.resize(useJoNum);
	targetTorque.clear();
	targetTorqueP.clear();
	targetTorqueD.clear();

	// coefficientの初期化
	coefficientJo.resize(useJoNum);
	coefficientJo.clear();
	coefficient = 0;
	coefficientP = 0;
	coefficientD = 0;

	count = 0;
	qdprBalanceRate = 0;
	cogForce.clear();

}
void Balance::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficientの設定
	coefficient = 1.0f;
	coefficientP = 0.7f;
	coefficientD = 1.0f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJo[i] = 0.8f;
	}
	//coefficientJo[2] = 0.0;

	// 二次計画法の割合
	qdprBalanceRate = 0.8f;

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
void Balance::Measure(SGScene* scene,CRHuman* crHuman){
	CalcBalanceTorque(scene,crHuman);			// バランスに必要なトルクを計算
	CalcTargetZMP(scene,crHuman);				// 目標ZMPの計算
}
void Balance::ModifyBody(SGScene* scene,CRHuman* crHuman){
	
	///////　足首でバランス　///////
	/*
	Vec3d targetT_R = (crHuman->solids[crHuman->solids.size()-2]->GetRotation()).inv()*GetTargetTorque();
	crHuman->joints[crHuman->joints.size()-4]->SetTorque(-targetT_R.x);
	crHuman->joints[crHuman->joints.size()-3]->SetTorque(-targetT_R.z);
	crHuman->joints[crHuman->joints.size()-2]->SetTorque(-targetT_R.y);
	*/
	///////　二次計画法でバランス　///////
	// 目的(到達運動)トルクのセット
	SetPreQuadPR(scene,crHuman);
	// 伝播行列の計算
	static int i = 0;
	if(i/10){
		CalcTransMatrix(scene,crHuman);
		i = 0;
	}
	i++;
	// 二次計画法の式をセット
	SetQuadPR(crHuman);
	// 二次計画法を解く
	QuadPRSolve();
	// 最適トルクをセット
	SetJointTorque(crHuman);
	// 足首でバランスの補償(最適トルクとバランストルクの差)
	SetAnkle(crHuman);
	// データ出力
	//DataOut(crHuman);
}

void Balance::SetPreQuadPR(SGScene* scene,CRHuman* crHuman){
	// 各関節の目標トルク T0,T0のP,D成分の取得
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(CompNoJoint(i))
			add++;
		else{
			targetTorque[i-add] = crHuman->joints[i]->GetTorque();
			targetTorqueP[i-add] = crHuman->jointPids[i]->GetProportionalTorque();
			targetTorqueD[i-add] = crHuman->jointPids[i]->GetDifferentialTorque();
		}
	}
}

void Balance::CalcTransMatrix(SGScene* scene,CRHuman* crHuman){
	timer.Start();

	// 踵の剛体の力、トルクを退避
	Vec3d AnkleForce,AnkleTorque;
	AnkleForce = crHuman->solids[crHuman->solids.size()-2]->GetForce();
	AnkleTorque = crHuman->solids[crHuman->solids.size()-2]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF_*ANKLE_NUMBER; i++){
		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// フェザーストーンの一部を使い、左右足首最後のJointの加速度計算
		crHuman->joints[crHuman->joints.size()-2]->CalcAccel(scene->GetTimeStep());	// 右足首
		
		// 両足首の加速度
		PTM::TVector<ANKLE_DOF_,double> accelVec;
		accelVec = GetJointAccel(crHuman->joints[crHuman->joints.size()-2]);

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
		crHuman->solids[crHuman->solids.size()-2]->SetForce(AnkleForce);
		crHuman->solids[crHuman->solids.size()-2]->SetTorque(AnkleTorque);
	}

	DSTR << "伝播行列の計算時間："<< timer.Stop()*1e-3 << "[ms]" << std::endl;;
	timer.Clear();
}
Vec3d Balance::GetJointAccel(PHJoint1D* joint){
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
void Balance::MakeTorquePattern(CRHuman* crHuman, int t){
/////	関節のトルク、足首にかかる力すべて0	  //////
	if(t == 0){
		// 各関節トルクを0にする
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				crHuman->joints[i]->SetTorque(0.0);
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

/////// トルクに１がはいる　/////////////
	// 各関節トルクに1を入れる
	else if(t > 0 && t <= useJoNum){
		int add = 0;
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(CompNoJoint(i)){
				add++;
			}
			if(i == t-1+add){
				if(CompNoJoint(i)){
					crHuman->joints[i]->SetTorque(0.0);
				}
				else{
					if(crHuman->joints[i] != NULL){
						crHuman->joints[i]->SetTorque(1.0);
					}
				}
			}
			else{
				if(crHuman->joints[i] != NULL){
					crHuman->joints[i]->SetTorque(0.0);
				}
			}
		}
		// 足首にかかる力を0に・・・入れないことが0
	}

//////	足首にかかる力が１はいる	///////
	else if(t > useJoNum){
		// 足首にかかる力に１を入れる
		int i = t - (useJoNum + 1);
		crHuman->solids[crHuman->solids.size()-2]->AddForce(unitVec[i]
			,crHuman->solids[crHuman->solids.size()-2]->GetFrame()->GetPosture()*crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos());

		// 各関節トルクに0を入れる
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}
void Balance::DataOut(CRHuman* crHuman){

	std::ofstream of("quadPR.xls");

	// 関節の名前
	for(int i = 0 ; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			of << crHuman->joints[i]->GetName() << "\t";
		}
	}
	of << std::endl;

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

	// バランスに必要なトルクの出力
	of << std::endl;
	of << "バランスに必要なトルク" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << BalanceTorque[i] << "\t";
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
	
	of.close();
}

void Balance::SetQuadPR(CRHuman* crHuman){
	// 重心から足首への距離（伝播行列を求める前と後でも値変わらず）
	Vec3d cog =crHuman->GetCOG();
	Vec3d posCog_Ankle = crHuman->joints[crHuman->joints.size()-2]->solid->GetFrame()->GetPosture()
									* crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos() - cog;

	// T_f'の設定
	PTM::TMatrixRow<ANKLE_DOF_*ANKLE_NUMBER,ANKLE_DOF_*ANKLE_NUMBER,double> tempT_f = - transF.inv();

	// Tの設定
	PTM::TMatrixRow<DOF_NUM,ANKLE_DOF_,double> tempT;
	Matrix3d e;
	tempT.sub_matrix(0,0, PTM::TMatDim<ANKLE_DOF_,ANKLE_DOF_>()) = e.Unit();
	tempT.sub_matrix(3,0, PTM::TMatDim<ANKLE_DOF_,ANKLE_DOF_>()) = Matrix3d::Cross(posCog_Ankle);
	//tempT *= -1;

	// Aの設定
	PTM::VMatrixRow<double> tempA;
	tempA.resize(DOF_NUM,useJoNum);
	tempA = tempT*tempT_f*transT;

	// bの設定
	PTM::VVector<double> tempB;
	tempB.resize(DOF_NUM);
	PTM::TVector<DOF_NUM,double> f_m;		// 重力
	f_m.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;
	f_m.sub_vector(3,PTM::TVecDim<3>()) = Vec3d();
	PTM::TVector<DOF_NUM,double> f_contact;		// 接触力
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetContactForce();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetContactTorque();
	tempB = tempT*tempT_f*transM + f_m + f_contact;


	// Qの設定
	PTM::VMatrixRow<double> quadMatQ;
	quadMatQ.resize(useJoNum,useJoNum);
	quadMatQ = tempA.trans()*tempA;
	for(int i = 0; i < useJoNum; i++){
		for(int j = 0; j < useJoNum; j++){
			if(i == j){
				//quadPR.matQ[i][j] = quadMatQ[i][j] + coefficient;
				quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientJo[i];
				//quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientP + coefficientD;
			}
			else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
		}
	}

	// Cの設定
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJoNum);
	PTM::TVector<DOF_NUM,double> targetF;
	//targetF.sub_vector(0,PTM::TVecDim<3>()) = cogForce;
	targetF.sub_vector(0,PTM::TVecDim<3>()) = GetTargetForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	targetF.sub_vector(3,PTM::TVecDim<3>()) = qdprBalanceRate*GetTargetTorque();
	//quadMatC = (targetF - tempB)*tempA + coefficient*targetTorque;
	quadMatC = (targetF - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	for(int i = 0; i < useJoNum; i++){
		quadPR.vecC[i] = quadMatC[i];
	}

	// TEST
	BalanceTorque.resize(useJoNum);
	BalanceTorque = GetTargetTorque();
	optA.resize(DOF_NUM,useJoNum);
	optA = tempA;
	optB.resize(DOF_NUM);
	optB = tempB;
}

void Balance::QuadPRSolve(){
	timer.Start();
	count = quadPR.Solve();
	DSTR << "二次計画法の計算時間："<< timer.Stop()*1e-3 << "[ms]" << std::endl;;
	timer.Clear();
	DSTR << "試行回数：" << count << std::endl;
}

void Balance::SetJointTorque(CRHuman* crHuman){
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(CompNoJoint(i))
			add++;
		else
			crHuman->joints[i]->SetTorque(quadPR.vecX[i-add]);
	}
}

void Balance::SetAnkle(CRHuman* crHuman){

	//　バランスに必要なトルクと最適解の差を足首が補償
	PTM::VVector<float> optTorque;
	optTorque.resize(useJoNum);
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
	PTM::VVector<float> x_cog;
	x_cog.resize(DOF_NUM);
	x_cog = optA*optTorque+optB;

	/*
	DSTR << "各関節が重心に与えるトルク"<<x_cog.sub_vector(3,PTM::TVecDim<3>()) <<std::endl;
	DSTR << "バランスに必要なトルクトルク"<<GetTargetTorque() <<std::endl;
	*/

	Vec3d targetT = GetTargetTorque();
	Vec3d balanceTorque =  qdprBalanceRate*targetT - x_cog.sub_vector(3,PTM::TVecDim<3>());
	balanceTorque += (1-qdprBalanceRate)*targetT;

	Vec3d ankleForce = -transF.trans()*(transT*optTorque + transM);

	AdjustAnkleTorque(crHuman,ankleForce,balanceTorque,0);

	/*
	//足首の座標へ変換
	Vec3d targetT_R = crHuman->solids[crHuman->solids.size()-2]->GetRotation()*(torque + (1-qdprBalanceRate)*(targetT));
	crHuman->joints[crHuman->joints.size()-4]->SetTorque(-targetT_R.x);
	crHuman->joints[crHuman->joints.size()-3]->SetTorque(-targetT_R.z);
	*/
	Vec3d targetT_R = (crHuman->solids[crHuman->solids.size()-2]->GetRotation()).inv()*balanceTorque;
	crHuman->joints[crHuman->joints.size()-4]->SetTorque(-targetT_R.x);
	crHuman->joints[crHuman->joints.size()-3]->SetTorque(-targetT_R.z);
	crHuman->joints[crHuman->joints.size()-2]->SetTorque(-targetT_R.y);

}

void Balance::AdjustAnkleTorque(CRHuman* crHuman,Vec3d& ankleForce,Vec3d& ankleTorque,int ankle){

	// ZMPを計算
	ankleZmp = (ankleForce % ankleTorque) / ankleForce.square();
	ankleZmp += crHuman->solids[crHuman->solids.size()-2]->GetFrame()->GetPosture()*crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos();
	ankleZmp.Y() = 0.0f;

	// 片足のZMPが片足の接触多角形の中にあるかどうか、ない場合は最近傍点を求める
	if(!AnkleZmpCheck(ankleZmp,&crHuman->supportObject[ankle])){
		// 最近傍点を求める
		ankleZmp=ClosestPoint(ankleZmp,&crHuman->supportObject[ankle]);

		// 最近傍点での足にかかるトルクを求める
		ankleTorque = (ankleZmp - crHuman->solids[crHuman->solids.size()-2]->GetFrame()->GetPosture()
								*crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos())% ankleForce;
	}
}

bool Balance::AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea){

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

Vec3f Balance::ClosestPoint(Vec3f zmp,CRSupportArea* supportArea){
	Vec3f closest;
	std::vector<Vec3f> supportVetxs = supportArea->GetSupportArea();
	double max = 0;					//絶対値の最大値
	int maxNum = 0;

	// 一番近い安定領域の頂点
	if(supportVetxs.size() > 2){
		for(int i =0; i < supportVetxs.size(); i++){
			if(max > (supportVetxs[i] - zmp).norm()){
				max = (supportVetxs[i] - zmp).norm();
				maxNum = i;
			}
		}
		closest = CalcPoint(supportVetxs[maxNum],supportVetxs[maxNum-1],zmp);
		if(!(closest == Vec3f())){
			return closest;
		}
		else {
			closest = CalcPoint(supportVetxs[maxNum],supportVetxs[maxNum+1],zmp);
			if(!(closest == Vec3f())){
				return closest;
			}
			else{
				return supportVetxs[maxNum];
			}
		}
	}
	else return zmp;
}

Vec3f Balance::CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp){

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


}		// end namespace