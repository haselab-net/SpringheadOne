// QDPRSolver.cpp: QDPRSolver クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "QDPRSolver.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;
QDPRSolver::QDPRSolver()
{
	/*
	//bSpringhead = false;
	for(int i = 0; i < SIZE_P; i++){
		for(int j = 0; j < SIZE_J; j++){
			force_torque[i][j] = 0.0f;
		}
		force_torque_g[i] = 0.0f;
	}
	*/
}


QDPRSolver::~QDPRSolver()
{

}

void QDPRSolver::Init(){

	useJointNum = 6;
	ankleDOF	= 3;
	ankleNum	= 2;
	balanceDOF	= 6;

	coefficient = 1;

	tempA.resize(ankleDOF*ankleNum, useJointNum);
	tempB.resize(balanceDOF);
	tempA.clear();
	tempB.clear();


	// 制約条件の設定
	PTM::VVector<float> vmin;
	PTM::VVector<float> vmax;
	vmin.resize(useJointNum);
	vmax.resize(useJointNum);
	for(int i=0; i<useJointNum; ++i){
		vmin[i] = -100;
		vmax[i] = 100;
	}
	quadPR.Init(vmin, vmax);	

	srand(useJointNum);
	//srand((unsigned int)time(NULL));

	// tempAとtempBの設定
	for(int i = 0; i < tempA.height(); i++){
		for(int j = 0; j < tempA.width(); j++){
			tempA[i][j] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
			//tempA[i][j] = rand();
		}
	}
	for(int i = 0; i < tempB.size(); i++)
		tempB[i] =  ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
		//tempB[i] = rand();

	// x_goalの設定
	x_goal.resize(balanceDOF);
	x_goal.clear();
	x_goal.sub_vector(0,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);
	x_goal.sub_vector(3,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);

	x_cog.resize(balanceDOF);
	x_cog.clear();
	optTorque.resize(useJointNum);
	optTorque.clear();


	// Qの設定
	PTM::VMatrixRow<double> quadMatQ;
	quadMatQ.resize(useJointNum,useJointNum);
	quadMatQ = tempA.trans()*tempA;
	for(int i = 0; i < useJointNum; i++){
		for(int j = 0; j < useJointNum; j++){
			if(i == j){
				//quadPR.matQ[i][j] = quadMatQ[i][j] + coefficient;
				quadPR.matQ[i][j] = quadMatQ[i][j];
				//quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientP + coefficientD;
			}
			else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
		}
	}

	// Cの設定
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJointNum);
	quadMatC = (x_goal - tempB)*tempA;
	for(int i = 0; i < useJointNum; i++){
		quadPR.vecC[i] = quadMatC[i];
	}


/*
	//QとCを設定するために必要なパラメータ
	for(int i = 0; i < SIZE_J; i++){
		float norm = 0.0f;
		for(int j = 0; j < SIZE_P; j++){
			//force_torque[j][i] = rand()/(RAND_MAX +1.0);	// 各関節にかかる力とトルク(6次元)の設定
			force_torque[j][i] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));	// 各関節にかかる力とトルク(6次元)の設定
			norm += pow(force_torque[j][i],2);
		}
		for(int j = 0; j < SIZE_P; j++){
			force_torque[j][i] /= sqrt(norm);			// 正規化
		}
	}

	for(int j = 0; j < SIZE_P; j++){
		//force_torque_g[j] = rand()/(RAND_MAX +1.0);	// 全身にかかる目標の力とトルクの設定
		force_torque_g[j] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
	}

	// Qの設定
	for(int i = 0; i < SIZE_J; i++){
		for(int j = 0; j < SIZE_J; j++){
			for(int k = 0; k < SIZE_P; k++){
				aQP.matQ[i][j] += force_torque[k][i]*force_torque[k][j];
			}
			if(i == j){
				aQP.matQ[i][j] += ADD;
			}
		}
	}

	// Cの設定
	for(int i = 0; i < SIZE_J; i++){
		for(int j = 0; j < SIZE_P; j++){
			aQP.vecC[i] += force_torque_g[j]*force_torque[j][i];
		}
	}
	*/
}

void QDPRSolver::Calc(){
	timer.Start();
	count = quadPR.Solve();
	cout << "二次計画法の計算："<< timer.Stop()*1e-3 << "[ms]" << endl;
	cout << "試行回数：" << count << endl;
	timer.Clear();

	//　最適解
	for(int i = 0; i < useJointNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}

void QDPRSolver::Out(){
	/*
	// 結果の描画
	cout <<"試行回数："<< count << " times" << endl;
	// 二次計画法を解くのにかかった時間
	cout << "最適解を求めるのに時間：" << time << endl;
	for(int i=0;i<SIZE_J;i++){
		cout << aQP.vecX[i] << endl;
	}
	cout << endl;
	*/
}

void QDPRSolver::DataOut(){

	//std::ofstream of("data.xls",ios::app);
	std::ofstream of("data.xls");

	// 最適解による重心にかかる力とトルク
	x_cog = tempA*optTorque+tempB;
	of << std::endl;
	of << "最適解による重心にかかる力とトルク" << std::endl;
	for(int i = 0 ; i < x_cog.size(); i++){
		of << x_cog[i] << "\t";
	}

	// 最適解の出力
	of << std::endl;
	of << "最適解" << std::endl;
	for(int i = 0 ; i < useJointNum; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// 目標の力とトルク
	of << std::endl;
	of << " 目標の力とトルク" << std::endl;
	for(int i = 0; i < x_goal.size(); i++){
		of << x_goal[i] << "\t";
	}

	// Qの出力
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < useJointNum; i++){
		for(int j = 0 ; j < useJointNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// Cの出力
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < useJointNum; i++){
			of << quadPR.vecC[i] << "\t";
	}
	
	of.close();

}

void QDPRSolver::Set(int seed){

	/*
	srand(seed);

	//QとCを設定するために必要なパラメータ
	for(int i = 0; i < SIZE_J; i++){
		float norm = 0.0f;
		for(int j = 0; j < SIZE_P; j++){
			// 各関節にかかる力とトルク(6次元)の設定
			force_torque[j][i] += ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5))*STEP;
			norm += pow(force_torque[j][i],2);
		}
		for(int j = 0; j < SIZE_P; j++){
			force_torque[j][i] /= sqrt(norm);			// 正規化
		}
	}

	for(int j = 0; j < SIZE_P; j++){
		force_torque_g[j] += ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5))*STEP;	// 全身にかかる目標の力とトルクの設定
	}

	//Q,Cのクリア
	for(int i = 0; i < SIZE_P; i++){
		for(int j = 0; j < SIZE_J; j++){
			force_torque[i][j] = 0.0f;
		}
		force_torque_g[i] = 0.0f;
	}

	// Qの設定
	for(int i = 0; i < SIZE_J; i++){
		for(int j = 0; j < SIZE_J; j++){
			for(int k = 0; k < SIZE_P; k++){
				aQP.matQ[i][j] += force_torque[k][i]*force_torque[k][j];
			}
			if(i == j){
				aQP.matQ[i][j] += ADD;
			}
		}
	}

	// Cの設定
	for(int i = 0; i < SIZE_J; i++){
		for(int j = 0; j < SIZE_P; j++){
			aQP.vecC[i] += force_torque_g[j]*force_torque[j][i];
		}
	}
	*/
}

void QDPRSolver::ParamOut(){
	/*
	std::ofstream off("parameter.xls");
	for(int i = 0; i < SIZE_J; i++){
		for(int j = 0; j < SIZE_J; j++){
			off << aQP.matQ[j][i] <<"\t";	
		}
		//cout << aQP.vecC[i] << endl;
		off << std::endl;
	}
	off.close();
	*/
}

}	// end namespace
