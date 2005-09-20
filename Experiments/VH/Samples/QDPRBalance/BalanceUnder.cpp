// BalanceUnder.cpp: BalanceUnder �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BalanceUnder.h"
#include "ModelUnder.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// �\�z/����
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
#if 1	//	�㔼�g�𐧌䂷��Ȃ�1�C�����g�����Ȃ�0
	// �㔼�g
	// (��:X,Z,Y)
	//noJoint.push_back(0);
	//noJoint.push_back(1);
	//noJoint.push_back(2);

	// (��:X,Z,Y�j
	//noJoint.push_back(3);
	//noJoint.push_back(4);
	//noJoint.push_back(5);

	// (��:X,Z,Y�j
	//noJoint.push_back(6);
	noJoint.push_back(7);
	noJoint.push_back(8);

	// (�E��:X,Z,Y�j
	//noJoint.push_back(9);
	//noJoint.push_back(10);
	noJoint.push_back(11);

	// (�E�I:X,Y�j
	noJoint.push_back(12);
	noJoint.push_back(13);

	//	(�E���:Z,X)
	noJoint.push_back(14);
	noJoint.push_back(15);

	// (����:X,Z,Y�j
	//noJoint.push_back(16);
	//noJoint.push_back(17);
	noJoint.push_back(18);

	// (���I:X,Y�j
	noJoint.push_back(19);
	noJoint.push_back(20);

	//	(�����:Z,X)
	noJoint.push_back(21);
	noJoint.push_back(22);
#endif
	// �����g
	// (�E�Ҋ֐�:X,Z,Y�j
	//noJoint.push_back(23);
	//noJoint.push_back(24);
	//noJoint.push_back(25);

	// (�E�G:X�j
	//noJoint.push_back(26);

	// (���Ҋ֐�:X,Z,Y�j
	//noJoint.push_back(31);
	//noJoint.push_back(32);
	//noJoint.push_back(33);

	// (���G:X�j
	//noJoint.push_back(34);
	
	// �ȉ��͕K�{
	//	(�E����)
	/*
	noJoint.push_back(27);
	noJoint.push_back(28);
	noJoint.push_back(29);
	*/
	//	(�E�y���܂�)
	noJoint.push_back(30);

	//	(������)
	/*
	noJoint.push_back(35);
	noJoint.push_back(36);
	noJoint.push_back(37);
	*/

	//	(���y���܂�)
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
	// �e�N���X�̏�����
	CRBalance::Init(scene,crHuman);

	// �P�ʃx�N�g���̏�����
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// �o�����X�Ɏg��Ȃ��֐߂�o�^
	RegistNoJoint();

	//�@�o�����X�Ɏg�p����֐߂̐�
	unsigned int all_jo_num = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL)
			all_jo_num++;
	}
	useJoNum = all_jo_num - noJoint.size();

	// �`�d�s��̃T�C�Y�̐ݒ�&������
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

	// �o�����X�Ɏg�p����֐߂̖ڕW�g���N�̃T�C�Y�̐ݒ�&������
	targetTorque.resize(useJoNum);
	targetTorqueP.resize(useJoNum);
	targetTorqueD.resize(useJoNum);
	targetTorque.clear();
	targetTorqueP.clear();
	targetTorqueD.clear();

	// coefficient�̏�����
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

	///////�@����Ńo�����X�@///////
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
	///////�@�񎟌v��@�Ńo�����X�@///////
	crHuman->CalcContactForce();
	// �ړI(���B�^��)�g���N�̃Z�b�g
	SetPreQuadPR(scene,crHuman);
	// �`�d�s��̌v�Z
	static int i = 10;
	if(i/10){
		//CalcTransMatrix(scene,crHuman);
		CalcTransMatrix2(scene,crHuman);
		i = 0;
	}
	i++;
	// �񎟌v��@�̎����Z�b�g
	//SetQuadPR(crHuman);
	SetQuadPR2(crHuman);
	// �񎟌v��@������
	QuadPRSolve();
	// �œK�g���N���Z�b�g
	SetJointTorque(crHuman);
	// ����Ńo�����X�̕⏞(�œK�g���N�ƃo�����X�g���N�̍�)
	//SetAnkle(crHuman);
	// �f�[�^�o��
	DataOut(crHuman);

}

void BalanceUnder::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficient�̐ݒ�
	coefficient = 1.0f;
	coefficientP = 0.2f;
	coefficientD = 0.5f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJo[i] = 0.01f;
	}

	// �񎟌v��@�̊���
	qdprBalanceRate = 1.0f;

	// ��������̐ݒ�
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
	// �e�֐߂̖ڕW�g���N T0,T0��P,D�����̎擾
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

	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// �E����
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// ������
		
		// ������̉����x
		PTM::TVector<BALANCE_DOF,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[R_ANKLE_J]);
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[L_ANKLE_J]);

		//  �W���s��ɑ��
		// �`�d�s��M
		if(i==0){
			transM = accelVec;
		}
		// �`�d�s��T
		else if(i > 0 && i <= useJoNum){
			transT.col(i-1) = accelVec - transM;
		}
		// �`�d�s��F
		else if(i > useJoNum){
			transF.col(i - (useJoNum+1)) = accelVec - transM;
		}

		// ���̗́A�g���N��߂�
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
	//DSTR << "�`�d�s��̌v�Z���ԁF"<< transTime << "[ms]" << std::endl;;
}

void BalanceUnder::MakeTorquePattern(CRHuman* crHuman, int t){
/////	�֐߂̃g���N�A����ɂ�����͂��ׂ�0	  //////
	if(t == 0){
		// �e�֐߃g���N��0�ɂ���
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				crHuman->joints[i]->SetTorque(0.0);
		}
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

/////// �g���N�ɂP���͂���@/////////////
	// �e�֐߃g���N��1������
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
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

//////	����ɂ�����͂��P�͂���	///////
	else if(t > useJoNum){
		// ����ɂ�����͂ɂP������
		int i = t - (useJoNum + 1);
		if(i < 3){
			crHuman->solids[R_HEEL_S]->AddForce(unitVec[i]
				,crHuman->solids[R_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());
		}
		else{
			crHuman->solids[L_HEEL_S]->AddForce(unitVec[i-3]
				,crHuman->solids[L_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());
		}

		// �e�֐߃g���N��0������
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}

Vec3d BalanceUnder::GetJointAccel(PHJoint1D* joint){
	// ���ׂ�World���W�n
	Vec3d soPosAccel = joint->GetSolidAccel();					// �q��Solid�̉����x
	Vec3d soRotAccel = joint->GetSolidAngularAccel();			// �q��Solid�̊p�����x
	Vec3d soPosVelocity = joint->GetSolidVelocity();			// �q��Solid�̑��x
	Vec3d soRotVelocity = joint->GetSolidAngularVelocity();		// �q��Solid�̊p���x
	Vec3d joPos = joint->solid->GetFrame()->GetPosture()
			* joint->GetPostureFromChild().Pos();				// Joint�̈ʒu
	//	�֐߈ʒu�F��]��World�n�ŁCSolid�d�S�����_
	Vec3d relPos = joPos - joint->solid->GetCenterPosition();

	// Joint�̉����x
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
		of << "���ۂ̏d�S�̗� " << "\t" << "\t"<< "\t"<<
			"���ۂ̏d�S�̃g���N " << "\t" << "\t"<< "\t"<<
			"�o�����X�ɕK�v�ȗ� " << "\t" << "\t"<< "\t"<<
			"�o�����X�ɕK�v�ȃg���N " << "\t" << "\t"<< "\t"<<
			"�œK���ɂ��� " << "\t" << "\t"<< "\t"<<
			"�œK���ɂ��g���N " << "\t" << "\t"<< "\t"<<std::endl;
		kk++;
	}

	//���ۂ̏d�S�̗�
	Vec3f realForce = crHuman->GetContactForce() + crHuman->GetTotalMass()*gravityEngine->accel;
	for(int i =0; i < 3; i++)
		of << realForce[i] << "\t";

	//���ۂ̏d�S�̃g���N
	Vec3f realTorque = crHuman->GetContactTorque();
	for(int i =0; i < 3; i++)
		of << realTorque[i] << "\t";
	*/
	//�o�����X�ɕK�v�ȗ�
	of << "�o�����X�ɕK�v�ȗ͂ƃg���N" << std::endl;
	for(int i = 0; i < targetX.size(); i++)
		of << targetX[i] << "\t";
	of << std::endl;
	/*Vec3f balanceForce = GetTargetForce();
	for(int i =0; i < 3; i++)
		of << balanceForce[i] << "\t";

	//�o�����X�ɕK�v�ȃg���N
	Vec3f balanceTorquee = GetTargetTorque();
	for(int i =0; i < 3; i++)
		of << balanceTorquee[i] << "\t";
	of << std::endl;
	*/
	//�@�œK�����d�S�ɂ�����͂ƃg���N
	of << "�œK�����d�S�ɂ�����͂ƃg���N" << std::endl;
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
	// �`�d�s��̌v�Z����
	of << transTime << std::endl;
	// �񎟌v��@�̌v�Z����
	//of << quadTime << std::endl;
	*/
	/*
	// �֐߂̖��O
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
	// �e�֐߂̖ړI�g���N(t0)
	of << std::endl;
	of << "�e�֐߂̖ړI�g���N" << std::endl;
	for(int i = 0 ; i < targetTorque.size(); i++){
		of << targetTorque[i] << "\t";
	}
	of << std::endl;
	*/
	// �œK���̏o��
	of << "�œK��" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Q�̏o��
	of << "Q" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		for(int j = 0 ; j < useJoNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// C�̏o��
	of << "C" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
			of << quadPR.vecC[i] << "\t";
	}
	of << std::endl;
	/*
	of << std::endl;	
	//�@�`�d�s��T
	of << std::endl;
	of << "�`�d�s��T" << std::endl;
	for(int i = 0 ; i < transT.height(); i++){
		for(int j = 0 ; j < transT.width(); j++){
			of << transT[i][j] << "\t";
		}
		of << std::endl;
	}

	//�@�`�d�s��F
	of << std::endl;
	of << "�`�d�s��F" << std::endl;
	for(int i = 0 ; i < transF.height(); i++){
		for(int j = 0 ; j < transF.width(); j++){
			of << transF[i][j] << "\t";
		}
		of << std::endl;
	}
	
	
	//�@�`�d�s��M
	of << std::endl;
	of << "�`�d�s��M" << std::endl;
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
	DSTR << "�񎟌v��@�̌v�Z�F"<< quadTime << "[ms]" << std::endl;
	DSTR << "���s�񐔁F" << count << std::endl;
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

/////�@�o�����X�ɕK�v�ȃg���N�ƍœK���̍��𑫎񂪕⏞	/////

	//�@�œK�����d�S�ɂ�����͂ƃg���N
	PTM::VVector<float> optTorque;
	optTorque.resize(useJoNum);
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	Vec3d targetT = GetTargetTorque();
	// �œK���ɂ��d�S�ւ̗́��g���N�ƃo�����X�̗́��g���N�̍��i�o�����X�̎c��j
	Vec3d balanceAnkleTorque =  qdprBalanceRate*targetT - x_cog.sub_vector(3,PTM::TVecDim<3>());

	// �񎟌v��@�o�����X�Ƒ��񐧌�o�����X�𕪂����Ƃ��̑��񐧌�o�����X�̃g���N
	Vec3d targetT_ankle = (1-qdprBalanceRate)*targetT;

	// balanceAnkleTorque��targetT_ankle�𗼑��ɕ��z����(f_ar��f_al�̐�Βl�ɔ��)
	PTM::TVector<ANKLE_DOF*ANKLE_NUM,float> ankleForce;
	ankleForce = -transF.trans()*(transT*optTorque + transM);
	Vec3d ankleForceR = ankleForce.sub_vector(0,PTM::TVecDim<3>());
	Vec3d ankleForceL = ankleForce.sub_vector(3,PTM::TVecDim<3>());
	double rateR = ankleForceR.norm()/(ankleForceR.norm() + ankleForceL.norm());
	Vec3d balanceAnkleTorqueR = rateR * (balanceAnkleTorque + targetT_ankle);
	Vec3d balanceAnkleTorqueL = (1 - rateR) * (balanceAnkleTorque + targetT_ankle);

	// ���������Ȃ��悤�Ƀg���N�𒲐�
	SaveAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
	SaveAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);

	// �⏞�g���N��������
	//�E����̍��W�֕ϊ�
	Vec3f AnkleTorqueR = (crHuman->solids[R_HEEL_S]->GetRotation()).inv()*balanceAnkleTorqueR;
	crHuman->joints[R_ANKLE_J-2]->SetTorque(-AnkleTorqueR.x);
	crHuman->joints[R_ANKLE_J-1]->SetTorque(-AnkleTorqueR.z);
	crHuman->joints[R_ANKLE_J]->SetTorque(-AnkleTorqueR.y);

	//������̍��W�֕ϊ�
	Vec3f AnkleTorqueL = (crHuman->solids[L_HEEL_S]->GetRotation()).inv()*balanceAnkleTorqueL;
	crHuman->joints[L_ANKLE_J-2]->SetTorque(-AnkleTorqueL.x);
	crHuman->joints[L_ANKLE_J-1]->SetTorque(AnkleTorqueL.z);
	crHuman->joints[L_ANKLE_J]->SetTorque(AnkleTorqueL.y);

}

void BalanceUnder::SaveAnkleTorque(CRHuman* crHuman,Vec3d& ankleForce,Vec3d& ankleTorque,int ankle){

	// ZMP���v�Z
	zmp[ankle] = (ankleForce % ankleTorque) / ankleForce.square();
	if(!ankle)
		zmp[ankle] += crHuman->solids[R_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos();
	else
		zmp[ankle] += crHuman->solids[L_HEEL_S]->GetFrame()->GetPosture()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos();

	zmp[ankle].Y() = 0.0f;

	// �Б���ZMP���Б��̐ڐG���p�`�̒��ɂ��邩�ǂ����A�Ȃ��ꍇ�͍ŋߖT�_�����߂�
	if(!AnkleZmpCheck(zmp[ankle],&crHuman->supportObject[ankle])){
		// �ŋߖT�_�����߂�
		zmp[ankle]=ClosestPoint(zmp[ankle],&crHuman->supportObject[ankle]);

		// �ŋߖT�_�ł̑��ɂ�����g���N�����߂�
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
	double max = 0;					//��Βl�̍ő�l
	int maxNum = 0;

	// ��ԋ߂�����̈�̒��_
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

	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern2(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// �E����
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// ������
		
		// ������̉����x
		PTM::TVector<ANKLE_DOF*ANKLE_NUM,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = crHuman->joints[R_ANKLE_J]->GetSolidAngularAccel();
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = crHuman->joints[R_ANKLE_J]->GetSolidAccel();
		accelVec.sub_vector(6,PTM::TVecDim<3>()) = crHuman->joints[L_ANKLE_J]->GetSolidAngularAccel();
		accelVec.sub_vector(9,PTM::TVecDim<3>()) = crHuman->joints[L_ANKLE_J]->GetSolidAccel();


		//  �W���s��ɑ��
		// �`�d�s��M
		if(i==0){
			transM = accelVec;
		}
		// �`�d�s��T
		else if(i > 0 && i <= useJoNum){
			transT.col(i-1) = accelVec - transM;
		}
		// �`�d�s��F
		else if(i > useJoNum){
			transF.col(i - (useJoNum+1)) = accelVec - transM;
		}

		// ���̗́A�g���N��߂�
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
	DSTR << "�`�d�s��̌v�Z���ԁF"<< transTime << "[ms]" << std::endl;;
}

void BalanceUnder::MakeTorquePattern2(CRHuman* crHuman, int t){
/////	�֐߂̃g���N�A����ɂ�����͂��ׂ�0	  //////
	if(t == 0){
		// �e�֐߃g���N��0�ɂ���
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				crHuman->joints[i]->SetTorque(0.0);
		}
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

/////// �g���N�ɂP���͂���@/////////////
	// �e�֐߃g���N��1������
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
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

//////	����ɂ�����͂��P�͂���	///////
	else if(t > useJoNum){
		// ����ɂ�����͂ɂP������
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

		// �e�֐߃g���N��0������
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->SetTorque(0.0);
			}
		}
	}
}

void BalanceUnder::SetQuadPR2(CRHuman* crHuman){

	// T_f'�̐ݒ�
	static PTM::TMatrixRow<ANKLE_DOF*ANKLE_NUM,ANKLE_DOF*ANKLE_NUM,double> tempT_f;
	tempT_f = - transF.inv();

	PTM::TMatrixRow<3,12,double> tempT_T_R = tempT_f.sub_matrix(0,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_F_R = tempT_f.sub_matrix(3,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_T_L = tempT_f.sub_matrix(6,0, PTM::TMatDim<3,12>());
	PTM::TMatrixRow<3,12,double> tempT_F_L = tempT_f.sub_matrix(9,0, PTM::TMatDim<3,12>());
	

	// T�̐ݒ�
	static PTM::TMatrixRow<BALANCE_DOF,ANKLE_DOF*ANKLE_NUM,double> tempT;
	tempT.sub_matrix(0,0, PTM::TMatDim<3,12>()) = tempT_T_R + tempT_T_L;
	tempT.sub_matrix(3,0, PTM::TMatDim<3,12>()) = tempT_F_R + tempT_F_L;
	//tempT *= -1;

	// A�̐ݒ�
	tempA = tempT*transT;

	// b�̐ݒ�
	PTM::TVector<BALANCE_DOF,double> f_mass;		// �d��
	f_mass.sub_vector(0,PTM::TVecDim<3>()) = Vec3d();
	f_mass.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;

	PTM::TVector<BALANCE_DOF,double> f_contact;		// �ڐG��
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetContactTorque();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetContactForce();

	tempB = tempT*transM + f_mass + f_contact;


	// Q�̐ݒ�
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

	// C�̐ݒ�
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJoNum);
	targetX.sub_vector(0,PTM::TVecDim<3>()) = qdprBalanceRate*GetTargetTorque();
	targetX.sub_vector(3,PTM::TVecDim<3>()) = GetTargetForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	quadMatC = (targetX - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	quadPR.vecC = quadMatC;
}

}		// end namespace