// CRBalanceQuadPR.cpp: CRBalanceQuadPR �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Creature.h"
#include "CRBalanceQuadPR.h"
#include "CRHuman.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRBalanceQuadPR::CRBalanceQuadPR()
{
	// �P�ʍs��̏�����
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// �s��̈ꕔ�����o���s��̏�����
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
	// �`�d�s��̏�����
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
	DSTR << "�W���̌v�Z���ԁF" <<timer.Stop() << "[��s]" <<std::endl;
	timer.Clear();
	QuadPRSolve();
	SetJointTorque(crHuman);
	//TEST
	//SetAnkleForce(crHuman);
	DataOut(crHuman);

}
void CRBalanceQuadPR::TransTorque(SGScene* scene,CRHuman* crHuman){

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[R_HEEL_S]->GetForce();
	AnkleForce[1] = crHuman->solids[L_HEEL_S]->GetForce();
	AnkleTorque[0] = crHuman->solids[R_HEEL_S]->GetTorque();
	AnkleTorque[1] = crHuman->solids[L_HEEL_S]->GetTorque();
	
	for(int i = 0; i <= JOINT_NUM+DOF_NUM; i++){

		// �g���N&�̓p�^�[���̐���
		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
		crHuman->joints[R_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// �E����
		crHuman->joints[L_ANKLE_J]->CalcAccel(scene->GetTimeStep());	// ������

		// ������̉����x
		PTM::TVector<DOF_NUM,double> accelVec;
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[R_ANKLE_J]);
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = GetJointAccel(crHuman->joints[L_ANKLE_J]);

		//  �W���s��ɑ��
		// �`�d�s��C
		if(i==0){
			transmissionC = accelVec;
		}
		// �`�d�s��M
		else if(i > 0 && i <= JOINT_NUM){
			transmissionM.col(i-1) = accelVec - transmissionC;
		}
		// �`�d�s��D
		else if(i > JOINT_NUM){
			transmissionD.col(i - (JOINT_NUM+1)) = accelVec - transmissionC;
		}

		
		// ���̗́A�g���N��߂�
		crHuman->solids[R_HEEL_S]->SetForce(AnkleForce[0]);
		crHuman->solids[L_HEEL_S]->SetForce(AnkleForce[1]);
		crHuman->solids[R_HEEL_S]->SetTorque(AnkleTorque[0]);
		crHuman->solids[L_HEEL_S]->SetTorque(AnkleTorque[1]);
	}
}

void CRBalanceQuadPR::MakeTorquePattern(CRHuman* crHuman, int t){
/*	i = 0 ���Ƃ��ׂ�0��C
	0 < i ��= crHuman->joints.size()����,i-1�Ԗڂ̊֐߃g���N���P �� M
	crHuman->joints.size()+1 �� i �� crHuman->joints.size()+7����,
											����ɂ�����͂������珇�ԂɂP ���@D
	C��M��D�̏��Ԃŋ��߂�g���N�p�^�[�������
*/

/////	�֐߂̃g���N�A����ɂ�����͂��ׂ�0	  //////
	if(t == 0){
		// �e�֐߃g���N��0�ɂ���
		for(int i = 0; i < JOINT_NUM; i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

/////// �g���N�ɂP���͂���@/////////////
	// �e�֐߃g���N��1������
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
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

//////	����ɂ�����͂��P�͂���	///////
	else if(t > JOINT_NUM){
		// ����ɂ�����͂ɂP������
		int i = t - (JOINT_NUM + 1);
		if(i < 3){
			crHuman->solids[R_HEEL_S]->AddForce(unitVec[i]
				,crHuman->solids[R_HEEL_S]->GetRotation()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());
		}
		else{
			crHuman->solids[L_HEEL_S]->AddForce(unitVec[i-3]
				,crHuman->solids[L_HEEL_S]->GetRotation()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());
		}

		// �e�֐߃g���N��0������
		for(int i = 0; i < JOINT_NUM; i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}

Vec3d CRBalanceQuadPR::GetJointAccel(PHJoint1D* joint){
	// ���ׂ�World���W�n
	Vec3d soPosAccel = joint->GetSolidAccel();					// �q��Solid�̉����x
	Vec3d soRotAccel = joint->GetSolidAngularAccel();			// �q��Solid�̊p�����x
	Vec3d soPosVelocity = joint->GetSolidVelocity();			// �q��Solid�̑��x
	Vec3d soRotVelocity = joint->GetSolidAngularVelocity();		// �q��Solid�̊p���x
	Vec3d joPos = joint->solid->GetRotation() 
			* joint->GetPostureFromChild().Pos();				// Joint�̈ʒu

	// Joint�̉����x
	Vec3d relPos = joPos - joint->solid->GetCenterPosition();
	Vec3d joPosAccel = soPosAccel + soRotVelocity % (soPosVelocity + soRotVelocity%relPos)
						+ soRotAccel % relPos;
	return joPosAccel;
}

void CRBalanceQuadPR::SetQuadPR(CRHuman* crHuman){

	// �d�S���瑫��ւ̋���
	Vec3d cog =crHuman->GetCOG();
	Vec3d posCog_AnkleR = ((crHuman->joints[R_ANKLE_J]->solid->GetRotation()
				* crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos()) + crHuman->joints[R_ANKLE_J]->solid->GetCenterPosition()) - cog;
	Vec3d posCog_AnkleL = ((crHuman->joints[L_ANKLE_J]->solid->GetRotation()
				* crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos()) + crHuman->joints[L_ANKLE_J]->solid->GetCenterPosition()) - cog;

	// D'�̐ݒ�
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempD = - transmissionD.inv();

	// K�̐ݒ�
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempK;
	tempK.sub_matrix(0,0, PTM::TMatDim<3,6>()) = tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())
												+ tempD.sub_matrix(3,0, PTM::TMatDim<3,6>());
	tempK.sub_matrix(3,0, PTM::TMatDim<3,6>()) = Matrix3d::Cross(posCog_AnkleR)*tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())
												+ Matrix3d::Cross(posCog_AnkleL)*tempD.sub_matrix(3,0, PTM::TMatDim<3,6>());

	// A�̐ݒ�
	PTM::TMatrixRow<DOF_NUM,JOINT_NUM,double> tempA;
	tempA = tempK*transmissionM;

	// �e�֐߂̖ڕW�g���N T0
	//PTM::TVector<JOINT_NUM,double> targetTorqe;
	for(int i = 0; i < JOINT_NUM; i++){
		targetTorqe[i] = crHuman->joints[i]->GetTorque();
	}

	// Q�̐ݒ�
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

	// C�̐ݒ�
	PTM::TVector<JOINT_NUM,double> quadMatC;
	PTM::TVector<DOF_NUM,double> targetF;
	targetF.sub_vector(0,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetForce();
	targetF.sub_vector(3,PTM::TVecDim<3>()) = crHuman->crBalance.GetTargetTorque();
	//quadMatC = targetF*tempA - (tempK*transmissionC)*tempA;
	quadMatC = targetF*tempA - (tempK*transmissionC)*tempA + coefficient*targetTorqe;

/*	//���ʂ͏�Ɠ���������
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
	DSTR << "�񎟌v��@�̌v�Z�F"<< timer.Stop() << "[��s]" << std::endl;;
	timer.Clear();
	DSTR << "���s�񐔁F" << count << std::endl;
}

void CRBalanceQuadPR::Init(SGScene* scene,CRHuman* crHuman){
	
	CRBalanceQuadPR();

	// �񎟌v��@�̎����ݒ�
	quadPR.SetDim(JOINT_NUM);

	// ��������̐ݒ�
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

	// D'�̐ݒ�
	PTM::TMatrixRow<DOF_NUM,DOF_NUM,double> tempD = - transmissionD.inv();

	rForce = tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())*transmissionM*optToruqe
							+ tempD.sub_matrix(0,0, PTM::TMatDim<3,6>())*transmissionC;

	lForce = tempD.sub_matrix(3,0, PTM::TMatDim<3,6>())*transmissionM*optToruqe
							+ tempD.sub_matrix(3,0, PTM::TMatDim<3,6>())*transmissionC;

	// ����ɗ͂�������
	Vec3d grav = crHuman->GetTotalMass()*gravity->accel;
	crHuman->solids[R_HEEL_S]->AddForce(rForce
			,crHuman->solids[R_HEEL_S]->GetRotation()*crHuman->joints[R_ANKLE_J]->GetPostureFromChild().Pos());

	crHuman->solids[L_HEEL_S]->AddForce(lForce
			,crHuman->solids[L_HEEL_S]->GetRotation()*crHuman->joints[L_ANKLE_J]->GetPostureFromChild().Pos());

}

void CRBalanceQuadPR::DataOut(CRHuman* crHuman){

	std::ofstream of("quadPR.xls");

	// �֐߂̖��O
	for(int i = 0 ; i < JOINT_NUM; i++){
		if(crHuman->joints[i] != NULL){
			of << crHuman->joints[i]->GetName() << "\t";
		}
	}
	of << std::endl;

	// �e�֐߂̖ړI�g���N(t0)
	of << std::endl;
	of << "�e�֐߂̖ړI�g���N" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		of << targetTorqe[i] << "\t";
	}
	of << std::endl;
	// �ړI�֐��̒l
	//of << " �ړI�֐��̒l" << std::endl;
	//of << quadPR.vecX*quadPR.matQ*quadPR.vecX-2*quadPR.vecC << std::endl;
	//DSTR << quadPR.vecX*quadPR.matQ*quadPR.vecX-2*quadPR.vecC*quadPR.vecX << std::endl;

	// �œK���̏o��
	of << std::endl;
	of << "�œK��" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Q�̏o��
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
		for(int j = 0 ; j < JOINT_NUM; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// C�̏o��
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < JOINT_NUM; i++){
			of << quadPR.vecC[i] << "\t";
	}
	of << std::endl;
	
	//�@�`�d�s��M
	of << std::endl;
	of << "�`�d�s��M" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		for(int j = 0 ; j < JOINT_NUM; j++){
			of << transmissionM[i][j] << "\t";
		}
		of << std::endl;
	}
	
	//�@�`�d�s��D
	of << std::endl;
	of << "�`�d�s��D" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		for(int j = 0 ; j < DOF_NUM; j++){
			of << transmissionD[i][j] << "\t";
		}
		of << std::endl;
	}
	
	
	//�@�`�d�s��C
	of << std::endl;
	of << "�`�d�s��C" << std::endl;
	for(int i = 0 ; i < DOF_NUM; i++){
		of << transmissionC[i] << "\t";
	}
	
	of.close();
}

}		//	end namespace Spr