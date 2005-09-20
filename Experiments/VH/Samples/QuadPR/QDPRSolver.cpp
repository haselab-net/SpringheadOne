// QDPRSolver.cpp: QDPRSolver �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "QDPRSolver.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// �\�z/����
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


	// ��������̐ݒ�
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

	// tempA��tempB�̐ݒ�
	for(int i = 0; i < tempA.height(); i++){
		for(int j = 0; j < tempA.width(); j++){
			tempA[i][j] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
			//tempA[i][j] = rand();
		}
	}
	for(int i = 0; i < tempB.size(); i++)
		tempB[i] =  ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
		//tempB[i] = rand();

	// x_goal�̐ݒ�
	x_goal.resize(balanceDOF);
	x_goal.clear();
	x_goal.sub_vector(0,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);
	x_goal.sub_vector(3,PTM::TVecDim<3>()) = Vec3d(0.0,0.0,0.0);

	x_cog.resize(balanceDOF);
	x_cog.clear();
	optTorque.resize(useJointNum);
	optTorque.clear();


	// Q�̐ݒ�
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

	// C�̐ݒ�
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJointNum);
	quadMatC = (x_goal - tempB)*tempA;
	for(int i = 0; i < useJointNum; i++){
		quadPR.vecC[i] = quadMatC[i];
	}


/*
	//Q��C��ݒ肷�邽�߂ɕK�v�ȃp�����[�^
	for(int i = 0; i < SIZE_J; i++){
		float norm = 0.0f;
		for(int j = 0; j < SIZE_P; j++){
			//force_torque[j][i] = rand()/(RAND_MAX +1.0);	// �e�֐߂ɂ�����͂ƃg���N(6����)�̐ݒ�
			force_torque[j][i] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));	// �e�֐߂ɂ�����͂ƃg���N(6����)�̐ݒ�
			norm += pow(force_torque[j][i],2);
		}
		for(int j = 0; j < SIZE_P; j++){
			force_torque[j][i] /= sqrt(norm);			// ���K��
		}
	}

	for(int j = 0; j < SIZE_P; j++){
		//force_torque_g[j] = rand()/(RAND_MAX +1.0);	// �S�g�ɂ�����ڕW�̗͂ƃg���N�̐ݒ�
		force_torque_g[j] = ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5));
	}

	// Q�̐ݒ�
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

	// C�̐ݒ�
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
	cout << "�񎟌v��@�̌v�Z�F"<< timer.Stop()*1e-3 << "[ms]" << endl;
	cout << "���s�񐔁F" << count << endl;
	timer.Clear();

	//�@�œK��
	for(int i = 0; i < useJointNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}

void QDPRSolver::Out(){
	/*
	// ���ʂ̕`��
	cout <<"���s�񐔁F"<< count << " times" << endl;
	// �񎟌v��@�������̂ɂ�����������
	cout << "�œK�������߂�̂Ɏ��ԁF" << time << endl;
	for(int i=0;i<SIZE_J;i++){
		cout << aQP.vecX[i] << endl;
	}
	cout << endl;
	*/
}

void QDPRSolver::DataOut(){

	//std::ofstream of("data.xls",ios::app);
	std::ofstream of("data.xls");

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
	for(int i = 0 ; i < useJointNum; i++){
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
	for(int i = 0 ; i < useJointNum; i++){
		for(int j = 0 ; j < useJointNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// C�̏o��
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

	//Q��C��ݒ肷�邽�߂ɕK�v�ȃp�����[�^
	for(int i = 0; i < SIZE_J; i++){
		float norm = 0.0f;
		for(int j = 0; j < SIZE_P; j++){
			// �e�֐߂ɂ�����͂ƃg���N(6����)�̐ݒ�
			force_torque[j][i] += ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5))*STEP;
			norm += pow(force_torque[j][i],2);
		}
		for(int j = 0; j < SIZE_P; j++){
			force_torque[j][i] /= sqrt(norm);			// ���K��
		}
	}

	for(int j = 0; j < SIZE_P; j++){
		force_torque_g[j] += ((rand()-(RAND_MAX*0.5))/(RAND_MAX*0.5))*STEP;	// �S�g�ɂ�����ڕW�̗͂ƃg���N�̐ݒ�
	}

	//Q,C�̃N���A
	for(int i = 0; i < SIZE_P; i++){
		for(int j = 0; j < SIZE_J; j++){
			force_torque[i][j] = 0.0f;
		}
		force_torque_g[i] = 0.0f;
	}

	// Q�̐ݒ�
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

	// C�̐ݒ�
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
