// QDPR.cpp: QDPR クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QDPR.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

QDPR::QDPR()
{

}

QDPR::~QDPR()
{

}
void QDPR::Step(BalanceUnder* human){

	static int kk = 0;
	if(!kk){
		Init(human);
		SetBalanceParameter(human);
		kk++;
	}
	Set(human);
	Calc();
	DataOut();


}
void QDPR::Calc(){
	timer.Start();
	count = quadPR.Solve();
	DSTR << "二次計画法の計算："<< timer.Stop()*1e-3 << "[ms]" << std::endl;
	DSTR << "試行回数：" << count << std::endl;
	timer.Clear();

	//　最適解
	for(int i = 0; i < useJointNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}
void QDPR::Set(BalanceUnder* human){
	// tempAとtempBの設定
	tempA = human->tempA;
	tempB = human->tempB;

	// x_goalの設定
	x_goal.sub_vector(0,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);
	x_goal.sub_vector(3,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);

	// Qの設定
	PTM::VMatrixRow<double> quadMatQ;
	quadMatQ.resize(human->useJoNum,human->useJoNum);
	quadMatQ = tempA.trans()*tempA;
	for(int i = 0; i < human->useJoNum; i++){
		for(int j = 0; j < human->useJoNum; j++){
			if(i == j){
				quadPR.matQ[i][j] = quadMatQ[i][j] + coefficient;
				//quadPR.matQ[i][j] = quadMatQ[i][j];
				//quadPR.matQ[i][j] = quadMatQ[i][j] + coefficientP + coefficientD;
			}
			else{
				quadPR.matQ[i][j] = quadMatQ[i][j];
			}
		}
	}

	// Cの設定
	PTM::VVector<double> quadMatC;
	quadMatC.resize(human->useJoNum);
	quadMatC = (x_goal - tempB)*tempA;
	for(int i = 0; i < quadMatC.size(); i++){
		quadPR.vecC[i] = quadMatC[i];
	}



}
void QDPR::Init(BalanceUnder* human){

	useJointNum = 6;
	ankleDOF	= 3;
	ankleNum	= 2;
	balanceDOF	= 6;

	coefficient = 0.1f;


	tempA.resize(ankleDOF*ankleNum,human->useJoNum);
	tempB.resize(balanceDOF);
	tempA.clear();
	tempB.clear();

	x_cog.resize(balanceDOF);
	x_cog.clear();
	optTorque.resize(human->useJoNum);
	optTorque.clear();
	x_goal.resize(balanceDOF);
	x_goal.clear();


}

void QDPR::SetBalanceParameter(BalanceUnder* human){

	// 制約条件の設定
	PTM::VVector<float> vmin;
	PTM::VVector<float> vmax;
	vmin.resize(human->useJoNum);
	vmax.resize(human->useJoNum);
	for(int i=0; i<vmin.size(); ++i){
		vmin[i] = -10000;
		vmax[i] = 10000;
	}
	quadPR.Init(vmin, vmax);	


}
void QDPR::DataOut(){
	std::ofstream of("TargetZero.xls",std::ios::app);
	//std::ofstream of("data.xls");

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
	for(int i = 0 ; i < quadPR.vecX.size(); i++){
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
	for(int i = 0 ; i < quadPR.matQ.height(); i++){
		for(int j = 0 ; j < quadPR.matQ.width(); j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// Cの出力
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < quadPR.vecC.size(); i++){
			of << quadPR.vecC[i] << "\t";
	}
	
	of.close();

}

}		//	end namespace Spr
