// QDPR.cpp: QDPR �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QDPR.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
	DSTR << "�񎟌v��@�̌v�Z�F"<< timer.Stop()*1e-3 << "[ms]" << std::endl;
	DSTR << "���s�񐔁F" << count << std::endl;
	timer.Clear();

	//�@�œK��
	for(int i = 0; i < useJointNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}
void QDPR::Set(BalanceUnder* human){
	// tempA��tempB�̐ݒ�
	tempA = human->tempA;
	tempB = human->tempB;

	// x_goal�̐ݒ�
	x_goal.sub_vector(0,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);
	x_goal.sub_vector(3,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);

	// Q�̐ݒ�
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

	// C�̐ݒ�
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

	// ��������̐ݒ�
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

	// �œK���ɂ��d�S�ɂ�����͂ƃg���N
	x_cog = tempA*optTorque+tempB;
	of << std::endl;
	of << "�œK���ɂ��d�S�ɂ�����͂ƃg���N" << std::endl;
	for(int i = 0 ; i < x_cog.size(); i++){
		of << x_cog[i] << "\t";
	}

	// �œK���̏o��
	of << std::endl;
	of << "�œK��" << std::endl;
	for(int i = 0 ; i < quadPR.vecX.size(); i++){
		of << quadPR.vecX[i] << "\t";
	}

	// �ڕW�̗͂ƃg���N
	of << std::endl;
	of << " �ڕW�̗͂ƃg���N" << std::endl;
	for(int i = 0; i < x_goal.size(); i++){
		of << x_goal[i] << "\t";
	}

	// Q�̏o��
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < quadPR.matQ.height(); i++){
		for(int j = 0 ; j < quadPR.matQ.width(); j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// C�̏o��
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < quadPR.vecC.size(); i++){
			of << quadPR.vecC[i] << "\t";
	}
	
	of.close();

}

}		//	end namespace Spr
