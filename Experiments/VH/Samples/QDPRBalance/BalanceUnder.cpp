// BalanceUnder.cpp: BalanceUnder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BalanceUnder.h"
#include "ModelUnder.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

BalanceUnder::BalanceUnder()
{

}

BalanceUnder::~BalanceUnder()
{

}

void  BalanceUnder::RegistNoJoint(){
	noJoint.clear();
/*
#if 1	//	上半身を制御するなら1，下半身だけなら0
	// 上半身
	// (腰:X,Z,Y)
	//noJoint.push_back(0);
	//noJoint.push_back(1);
	//noJoint.push_back(2);

	// (胸:X,Z,Y）
	//noJoint.push_back(3);
	//noJoint.push_back(4);
	//noJoint.push_back(5);

	// (首:X,Z,Y）
	//noJoint.push_back(6);
	noJoint.push_back(7);
	noJoint.push_back(8);

	// (右肩:X,Z,Y）
	//noJoint.push_back(9);
	//noJoint.push_back(10);
	noJoint.push_back(11);

	// (右肘:X,Y）
	noJoint.push_back(12);
	noJoint.push_back(13);

	//	(右手首:Z,X)
	noJoint.push_back(14);
	noJoint.push_back(15);

	// (左肩:X,Z,Y）
	//noJoint.push_back(16);
	//noJoint.push_back(17);
	noJoint.push_back(18);

	// (左肘:X,Y）
	noJoint.push_back(19);
	noJoint.push_back(20);

	//	(左手首:Z,X)
	noJoint.push_back(21);
	noJoint.push_back(22);
#endif
	// 下半身
	// (右股関節:X,Z,Y）
	//noJoint.push_back(23);
	//noJoint.push_back(24);
	//noJoint.push_back(25);

	// (右膝:X）
	//noJoint.push_back(26);

	// (左股関節:X,Z,Y）
	//noJoint.push_back(31);
	//noJoint.push_back(32);
	//noJoint.push_back(33);

	// (左膝:X）
	//noJoint.push_back(34);
	
	// 以下は必須
	//	(右足首)
	/*
	noJoint.push_back(27);
	noJoint.push_back(28);
	noJoint.push_back(29);
	*/
	//	(右土踏まず)
	noJoint.push_back(30);

	//	(左足首)
	/*
	noJoint.push_back(35);
	noJoint.push_back(36);
	noJoint.push_back(37);
	*/

	//	(左土踏まず)
	noJoint.push_back(38);
}

bool BalanceUnder::CompNoJoint(int jointNum){
	for(int i = 0; i < noJoint.size(); i++){
		if(jointNum == noJoint[i])
			return true;
	}
	return false;
}

void BalanceUnder::Init(SGScene* scene,CRHuman* crHuman){
	// 親クラスの初期化
	CRBalance::Init(scene,crHuman);

	// 単位ベクトルの初期化
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// バランスに使わない関節を登録
	RegistNoJoint();

	//　バランスに使用する関節の数
	unsigned int all_jo_num = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL)
			all_jo_num++;
	}
	useJoNum = all_jo_num - noJoint.size();

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
	coefficientJo.resize(useJoNum);
	coefficientJo.clear();
	coefficient = 0;
	coefficientP = 0;
	coefficientD = 0;

	count = 0;
	qdprBalanceRate = 0;

	targetX.clear();

}

void BalanceUnder::ModifyBody(SGScene* scene,CRHuman* crHuman){

	///////　足首でバランス　///////
	/*
	Vec3d targetT_R = (crHuman->solids[R_HEEL_S]->GetRotation()).inv()*(GetTargetTorque()/2);
	Vec3d targetT_L = (crHuman->solids[L_HEEL_S]->GetRotation()).inv()*(GetTargetTorque()/2);
	crHuman->joints[R_ANKLE_J-2]->SetTorque(-targetT_R.x);
	crHuman->joints[R_ANKLE_J-1]->SetTorque(-targetT_R.z);
	crHuman->joints[R_ANKLE_J]->SetTorque(-targetT_R.y);
	crHuman->joints[L_ANKLE_J-2]->SetTorque(-targetT_L.x);
	crHuman->joints[L_ANKLE_J-1]->SetTorque(targetT_L.z);
	crHuman->joints[L_ANKLE_J]->SetTorque(targetT_L.y);	
	*/
	///////　二次計画法でバランス　///////
	crHuman->CalcContactForce();
	// 目的(到達運動)トルクのセット
	SetPreQuadPR(scene,crHuman);
	// 伝播行列の計算
	static int i = 10;
	if(i/10){
		//CalcTransMatrix(scene,crHuman);
		CalcTransMatrix2(scene,crHuman);
		i = 0;
	}
	i++;
	// 二次計画法の式をセット
	//SetQuadPR(crHuman);
	SetQuadPR2(crHuman);
	// 二次計画法を解く
	QuadPRSolve();
	// 最適トルクをセット
	SetJointTorque(crHuman);
	// 足首でバランスの補償(最適トルクとバランストルクの差)
	//SetAnkle(crHuman);
	// データ出力
	DataOut(crHuman);

}

void BalanceUnder::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficientの設定
	coefficient = 1.0f;
	coefficientP = 0.2f;
	coefficientD = 0.5f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJo[i] = 0.01f;
	}

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

void BalanceUnder::SetPreQuadPR(SGScene* scene,CRHuman* crHuman){
	// 各関節の目標トルク T0,T0のP,D成分の取得
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			if(CompNoJoint(i))
				add++;
			else{
				targetTorque[i-add] = crHuman->joints[i]->GetTorque();
				targetTorqueP[i-add] = crHuman->jointPids[i]->GetProportionalTorque();
				targetTorqueD[i-add] = crHuman->jointPids[i]->GetDifferentialTorque();
			}
		}
		else
			add ++;
	}
}

void BalanceUnder::CalcTransMatrix(SGScene* scene,CRHuman* crHuman){
	timer.Start();

	// 踵の剛体の力、トルクを退避
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// フェザーストーンの一部を使い、左右足首最後のJointの加速度計算
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 右足首
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 左足首
		
		// 両足首の加速度
		PTM::TVector<BALANCE_DOF,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[R_ANKLE_J]);
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[L_ANKLE_J]);

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
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
	//DSTR << "伝播行列の計算時間："<< transTime << "[ms]" << std::endl;;
}

void BalanceUnder::MakeTorquePattern(CRHuman* crHuman, int t){
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
		unsigned int add = 0;
		for(unsigned int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
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
		if(i < 3){
			crHuman->solids[R_HEEL_S]->AddForce(unitVec[i]
				,crHuman->solids[R_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());
		}
		else{
			crHuman->solids[L_HEEL_S]->AddForce(unitVec[i-3]
				,crHuman->solids[L_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());
		}

		// 各関節トルクに0を入れる
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}

Vec3d BalanceUnder::GetJointAccel(PHJoint1D* joint){
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

void BalanceUnder::DataOut(CRHuman* crHuman){
	static bool bFirst = true;
	std::ofstream of;
	if (bFirst){
		of.open("quadPR.xls");
		bFirst = false;
	}else{
		of.open("quadPR.xls",ios::app);
	}
	/*
	static int kk = 0;
	if(!kk){
		of << "実際の重心の力 " << "\t" << "\t"<< "\t"<<
			"実際の重心のトルク " << "\t" << "\t"<< "\t"<<
			"バランスに必要な力 " << "\t" << "\t"<< "\t"<<
			"バランスに必要なトルク " << "\t" << "\t"<< "\t"<<
			"最適解による力 " << "\t" << "\t"<< "\t"<<
			"最適解によるトルク " << "\t" << "\t"<< "\t"<<std::endl;
		kk++;
	}

	//実際の重心の力
	Vec3f realForce = crHuman->GetContactForce() + crHuman->GetTotalMass()*gravityEngine->accel;
	for(int i =0; i < 3; i++)
		of << realForce[i] << "\t";

	//実際の重心のトルク
	Vec3f realTorque = crHuman->GetContactTorque();
	for(int i =0; i < 3; i++)
		of << realTorque[i] << "\t";
	*/
	//バランスに必要な力
	of << "バランスに必要な力とトルク" << std::endl;
	for(int i = 0; i < targetX.size(); i++)
		of << targetX[i] << "\t";
	of << std::endl;
	/*Vec3f balanceForce = GetTargetForce();
	for(int i =0; i < 3; i++)
		of << balanceForce[i] << "\t";

	//バランスに必要なトルク
	Vec3f balanceTorquee = GetTargetTorque();
	for(int i =0; i < 3; i++)
		of << balanceTorquee[i] << "\t";
	of << std::endl;
	*/
	//　最適解が重心にかける力とトルク
	of << "最適解が重心にかける力とトルク" << std::endl;
	PTM::VVector<float> optTorque;
	optTorque.resize(useJoNum);
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	for(int i =0; i < 3; i++)
		of << x_cog.sub_vector(0,PTM::TVecDim<3>())[i] << "\t";

	for(int i =0; i < 3; i++)
		of << x_cog.sub_vector(3,PTM::TVecDim<3>())[i] << "\t";
	of << std::endl;

	
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
	*/
	// 最適解の出力
	of << "最適解" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Qの出力
	of << "Q" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		for(int j = 0 ; j < useJoNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// Cの出力
	of << "C" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
			of << quadPR.vecC[i] << "\t";
	}
	of << std::endl;
	/*
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
	*/
	of << std::endl;
	of.close();
}

void BalanceUnder::QuadPRSolve(){
	timer.Start();

	count = quadPR.Solve();

	quadTime = timer.Stop()*1e-3;
	timer.Clear();
	DSTR << "二次計画法の計算："<< quadTime << "[ms]" << std::endl;
	DSTR << "試行回数：" << count << std::endl;
}

void BalanceUnder::SetJointTorque(CRHuman* crHuman){
	int add = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			if(CompNoJoint(i)){
				add++;
			}
			else{
				crHuman->joints[i]->SetTorque(quadPR.vecX[i-add]);
			}
		}
		else{
			add++;
		}
	}
}
void BalanceUnder::SetAnkle(CRHuman* crHuman){

/////　バランスに必要なトルクと最適解の差を足首が補償	/////

	//　最適解が重心にかける力とトルク
	PTM::VVector<float> optTorque;
	optTorque.resize(useJoNum);
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	Vec3d targetT = GetTargetTorque();
	// 最適解による重心への力＆トルクとバランスの力＆トルクの差（バランスの残り）
	Vec3d balanceAnkleTorque =  qdprBalanceRate*targetT - x_cog.sub_vector(3,PTM::TVecDim<3>());

	// 二次計画法バランスと足首制御バランスを分けたときの足首制御バランスのトルク
	Vec3d targetT_ankle = (1-qdprBalanceRate)*targetT;

	// balanceAnkleTorqueとtargetT_ankleを両足に分配する(f_arとf_alの絶対値に比例)
	PTM::TVector<ANKLE_DOF*ANKLE_NUM,float> ankleForce;
	ankleForce = -transF.trans()*(transT*optTorque + transM);
	Vec3d ankleForceR = ankleForce.sub_vector(0,PTM::TVecDim<3>());
	Vec3d ankleForceL = ankleForce.sub_vector(3,PTM::TVecDim<3>());
	double rateR = ankleForceR.norm()/(ankleForceR.norm() + ankleForceL.norm());
	Vec3d balanceAnkleTorqueR = rateR * (balanceAnkleTorque + targetT_ankle);
	Vec3d balanceAnkleTorqueL = (1 - rateR) * (balanceAnkleTorque + targetT_ankle);

	// 足が浮かないようにトルクを調節
	SaveAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
	SaveAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);

	// 補償トルクをかける
	//右足首の座標へ変換
	Vec3f AnkleTorqueR = (crHuman->solids[R_HEEL_S]->GetRotation()).inv()*balanceAnkleTorqueR;
	crHuman->joints[R_ANKLE_J-2]->SetTorque(-AnkleTorqueR.x);
	crHuman->joints[R_ANKLE_J-1]->SetTorque(-AnkleTorqueR.z);
	crHuman->joints[R_ANKLE_J]->SetTorque(-AnkleTorqueR.y);

	//左足首の座標へ変換
	Vec3f AnkleTorqueL = (crHuman->solids[L_HEEL_S]->GetRotation()).inv()*balanceAnkleTorqueL;
	crHuman->joints[L_ANKLE_J-2]->SetTorque(-AnkleTorqueL.x);
	crHuman->joints[L_ANKLE_J-1]->SetTorque(AnkleTorqueL.z);
	crHuman->joints[L_ANKLE_J]->SetTorque(AnkleTorqueL.y);

}

void BalanceUnder::SaveAnkleTorque(CRHuman* crHuman,Vec3d& ankleForce,Vec3d& ankleTorque,int ankle){

	// ZMPを計算
	zmp[ankle] = (ankleForce % ankleTorque) / ankleForce.square();
	if(!ankle)
		zmp[ankle] += crHuman->solids[R_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos();
	else
		zmp[ankle] += crHuman->solids[L_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos();

	zmp[ankle].Y() = 0.0f;

	// 片足のZMPが片足の接触多角形の中にあるかどうか、ない場合は最近傍点を求める
	if(!AnkleZmpCheck(zmp[ankle],&crHuman->supportObject[ankle])){
		// 最近傍点を求める
		zmp[ankle]=ClosestPoint(zmp[ankle],&crHuman->supportObject[ankle]);

		// 最近傍点での足にかかるトルクを求める
		if(!ankle)
			ankleTorque = (zmp[ankle] - crHuman->solids[R_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos())
							% ankleForce;
		else
			ankleTorque = (zmp[ankle] - crHuman->solids[L_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos())
							% ankleForce;

	}
}

bool BalanceUnder::AnkleZmpCheck(Vec3f zmp,CRSupportArea* supportArea){

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

Vec3f BalanceUnder::ClosestPoint(Vec3f zmp,CRSupportArea* supportArea){
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

Vec3f BalanceUnder::CalcPoint(Vec3f p0, Vec3f p1,Vec3f zmp){

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


void BalanceUnder::CalcTransMatrix2(SGScene* scene,CRHuman* crHuman){
	timer.Start();

	// 踵の剛体の力、トルクを退避
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern2(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// フェザーストーンの一部を使い、左右足首最後のJointの加速度計算
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 右足首
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// 左足首
		
		// 両足首の加速度
		PTM::TVector<ANKLE_DOF*ANKLE_NUM,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = crHuman->joints[R_ANKLE_J]->GetSolidAngularAccel();
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = crHuman->joints[R_ANKLE_J]->GetSolidAccel();
		accelVec.sub_vector(6,PTM::TVecDim<3>()) = crHuman->joints[L_ANKLE_J]->GetSolidAngularAccel();
		accelVec.sub_vector(9,PTM::TVecDim<3>()) = crHuman->joints[L_ANKLE_J]->GetSolidAccel();


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
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
	DSTR << "伝播行列の計算時間："<< transTime << "[ms]" << std::endl;;
}

void BalanceUnder::MakeTorquePattern2(CRHuman* crHuman, int t){
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
		unsigned int add = 0;
		for(unsigned int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
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
		if(i < 3){
			crHuman->solids[R_HEEL_S]->AddTorque(unitVec[i]);
		}
		else if(i >=3 && i < 6){
			crHuman->solids[R_HEEL_S]->AddForce(unitVec[i-3]);
		}
		else if(i >=6 && i < 9){
			crHuman->solids[L_HEEL_S]->AddTorque(unitVec[i-6]);	
		}
		else{
			crHuman->solids[L_HEEL_S]->AddForce(unitVec[i-9]);
		}

		// 各関節トルクに0を入れる
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->SetTorque(0.0);
			}
		}
	}
}

void BalanceUnder::SetQuadPR2(CRHuman* crHuman){

	// T_f'の設定
	static PTM::TMatrixRow<ANKLE_DOF*ANKLE_NUM,ANKLE_DOF*ANKLE_NUM,double> tempT_f;
	tempT_f = - transF.inv();

	PTM::TMatrixRow<3,12,double> tempT_T_R = tempT_f.sub_matrix(0,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_F_R = tempT_f.sub_matrix(3,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_T_L = tempT_f.sub_matrix(6,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_F_L = tempT_f.sub_matrix(9,0, PTM::TMatDim<3,12>());
	

	// Tの設定
	static PTM::TMatrixRow<BALANCE_DOF,ANKLE_DOF*ANKLE_NUM,double> tempT;
	tempT.sub_matrix(0,0, PTM::TMatDim<3,12>()) = tempT_T_R + tempT_T_L;
	tempT.sub_matrix(3,0, PTM::TMatDim<3,12>()) = tempT_F_R + tempT_F_L;
	//tempT *= -1;

	// Aの設定
	tempA = tempT*transT;

	// bの設定
	PTM::TVector<BALANCE_DOF,double> f_mass;		// 重力
	f_mass.sub_vector(0,PTM::TVecDim<3>()) = Vec3d();
	f_mass.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;

	PTM::TVector<BALANCE_DOF,double> f_contact;		// 接触力
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetContactTorque();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetContactForce();

	tempB = tempT*transM + f_mass + f_contact;


	// Qの設定
	PTM::VMatrixRow<double> quadMatQ;
	quadMatQ.resize(useJoNum,useJoNum);
	quadMatQ = tempA.trans()*tempA;
	for(int i = 0; i < useJoNum; i++){
		for(int j = 0; j < useJoNum; j++){
			if(i == j){
				quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientJo[i] + coefficientP + coefficientD;
			}else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
		}
	}

	// Cの設定
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJoNum);
	targetX.sub_vector(0,PTM::TVecDim<3>()) = qdprBalanceRate*GetTargetTorque();
	targetX.sub_vector(3,PTM::TVecDim<3>()) = GetTargetForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	quadMatC = (targetX - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	quadPR.vecC = quadMatC;
}

}		// end namespace