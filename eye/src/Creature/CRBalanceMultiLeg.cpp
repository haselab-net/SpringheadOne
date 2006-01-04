#include "Creature.h"
#pragma hdrstop
// CRBalanceMultiLeg.cpp: CRBalanceMultiLeg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRBalanceMultiLeg.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRBalanceMultiLeg::CRBalanceMultiLeg()
{
	balanceState = BOTH_LEG;
	changeQDPR = false;
	balanceStateChange = false;
	cogDownRate = 1.0;
}

CRBalanceMultiLeg::~CRBalanceMultiLeg()
{

}
void CRBalanceMultiLeg::ModifyBody(SGScene* scene,CRHuman* crHuman){

	///////�@�񎟌v��@�Ńo�����X�@///////
	// VH�̐ڐG�͌v�Z
	crHuman->CalcContactForce();

	// �ړI(���B�^��)�g���N�̃Z�b�g
	SetTargetJointTorque(scene,crHuman);

	if(balanceStateChange){
		ChangeParameterSize();
		balanceStateChange =false;
	}

	if(balanceState == BOTH_LEG || changeQDPR == false){
		// �`�d�s��̌v�Z
		static int i = 10;
		if(i/10){
			CalcTransMatrix(scene,crHuman);
			i = 0;
		}
		i++;
		// �񎟌v��@�̎����Z�b�g
		SetQuadPR(crHuman);
	}
	else{
		// �`�d�s��̌v�Z
		static int i = 10;
		if(i/10){
			CalcTransMatrixOneLeg(scene,crHuman);
			i = 0;
		}
		i++;
		// �񎟌v��@�̎����Z�b�g
		SetQuadPROneLeg(crHuman);
	}


	// �񎟌v��@������
	QuadPRSolve();

	// �œK�g���N���Z�b�g(����̕ۏ��)
	SetBalanceTorque(crHuman);
	// ��������̍X�V
	//RenewConstraint(crHuman);
	// �f�[�^�o��
	//DataOut(crHuman);
}

void CRBalanceMultiLeg::ChangeParameterSize(){
	if(balanceState == BOTH_LEG){
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
	}
	else{
		transT.resize(ANKLE_DOF, useJoNum);
		transF.resize(ANKLE_DOF,ANKLE_DOF);
		transM.resize(ANKLE_DOF);
		transT.clear();
		transF.clear();
		transM.clear();
	}

}

void CRBalanceMultiLeg::CalcBalanceForce(SGScene* scene,CRHuman* crHuman){

	std::vector<Vec3f> supportVetxs;
	// �x���鑫�ɂ���Ĉ���̈��ς���B
	if(balanceState == BOTH_LEG){
		supportVetxs = crHuman->supportArea.GetSupportArea();
	}
	else if(balanceState == RIGHT_LEG){
		supportVetxs = crHuman->supportObject[0].GetSupportArea();
	}
	else if(balanceState == LEFT_LEG){
		supportVetxs = crHuman->supportObject[1].GetSupportArea();
	}
	Vec3f center = Vec3f();
	for(int i = 0; i < supportVetxs.size(); i++){
		center += supportVetxs[i];
	}
	center /= supportVetxs.size();
	center.y = (crHuman->GetCOG().y)*cogDownRate;
	float dt = scene->GetTimeStep();
	float k = springCog_no*(2*crHuman->GetTotalMass()/(dt*dt));
	float b = dampaCog_no*(crHuman->GetTotalMass()/dt);
	Vec3f cogForce = k*(center - crHuman->GetCOG()) - b*crHuman->GetCogVelocity();
	DSTR << cogForce << std::endl;
	//cogForce.y = 0.0f;
	// �d�� + �d�S������̈�̒��S�ɋ߂Â����
	balanceForce = crHuman->GetTotalMass()*(targetAccel-gravityEngine->accel) + cogForce;

}
void CRBalanceMultiLeg::CalcBalanceTorque(SGScene* scene, CRHuman* crHuman){
	static Matrix3f mat;
	if(balanceState == BOTH_LEG){
		targetPostureQ = Quaterniond(1.0,0.0,0.0,0.0);
	}
	else if(balanceState == RIGHT_LEG){
		mat = Matrix3f::Rot((float)Rad(-4), 'z');
		targetPostureQ.from_matrix(mat);
		//crHuman->jointPids[crHuman->ankleJointNum[0][1]]->SetPGoal(Rad(4));
	}
	else if(balanceState == LEFT_LEG){
		mat = Matrix3f::Rot((float)Rad(3), 'z');
		targetPostureQ.from_matrix(mat);
		crHuman->jointPids[crHuman->ankleJointNum[1][1]]->SetPGoal(Rad(4));
	}

	CRBalance::CalcBalanceTorque(scene,crHuman);
}

void CRBalanceMultiLeg::SetQuadPROneLeg(CRHuman* crHuman){

	// T_f'�̐ݒ�
	static PTM::TMatrixRow<ANKLE_DOF,ANKLE_DOF,double> tempT_f;
	tempT_f = - transF.inv();

	// A�̐ݒ�
	tempA = tempT_f*transT;

	// b�̐ݒ�
	static PTM::TVector<BALANCE_DOF,double> f_mass;		// �d��
	f_mass.sub_vector(0,PTM::TVecDim<3>()) = Vec3d();
	f_mass.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;

	static PTM::TVector<BALANCE_DOF,double> f_contact;	// �ڐG��
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactTorque()-crHuman->GetCOG()%crHuman->supportArea.GetContactForce();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactForce();

	tempB = tempT_f*transM + f_mass + f_contact;


	// Q�̐ݒ�
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

	// C�̐ݒ�
	PTM::VVector<double> quadMatC;
	quadMatC.resize(useJoNum);
	targetX.sub_vector(0,PTM::TVecDim<3>()) = qdprBalanceRate*GetBalanceTorque();
	targetX.sub_vector(3,PTM::TVecDim<3>()) = GetBalanceForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	quadMatC = (targetX - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	quadPR.vecC = quadMatC;

}
void CRBalanceMultiLeg::CalcTransMatrixOneLeg(SGScene* scene,CRHuman* crHuman){
	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce,AnkleTorque;
	if(balanceState == RIGHT_LEG){
		AnkleForce = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetForce();
		AnkleTorque = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetTorque();
	}
	else if(balanceState == LEFT_LEG){
		AnkleForce = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetForce();
		AnkleTorque = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetTorque();
	}

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF; i++){

		MakeTorquePatternOneLeg(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());


		// ����̉����x
		static PTM::TVector<ANKLE_DOF,double> accelVec;
		if(balanceState == RIGHT_LEG){
			// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
			// �E����
			crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->CalcAccel(scene->GetTimeStep());

			// �E��
			accelVec.sub_vector(0,PTM::TVecDim<3>()) = 
				crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAngularAccel();
			accelVec.sub_vector(3,PTM::TVecDim<3>()) = 
				crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAccel();
		}
		else if(balanceState == LEFT_LEG){
			// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
			// ������
			crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->CalcAccel(scene->GetTimeStep());

			// ����
			accelVec.sub_vector(0,PTM::TVecDim<3>()) = 
				crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAngularAccel();
			accelVec.sub_vector(3,PTM::TVecDim<3>()) = 
				crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAccel();
		}

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
		if(balanceState == RIGHT_LEG){
			crHuman->solids[crHuman->supportSolidNum[0][0]]->SetForce(AnkleForce);
			crHuman->solids[crHuman->supportSolidNum[0][0]]->SetTorque(AnkleTorque);
		}
		else if(balanceState == LEFT_LEG){
			crHuman->solids[crHuman->supportSolidNum[1][0]]->SetForce(AnkleForce);
			crHuman->solids[crHuman->supportSolidNum[1][0]]->SetTorque(AnkleTorque);
		}
	}
}

void  CRBalanceMultiLeg::MakeTorquePatternOneLeg(CRHuman* crHuman, int t){
/////	�֐߂̃g���N�A����ɂ�����͂��ׂ�0	  //////
	if(t == 0){
		// �e�֐߃g���N��0�ɂ���
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL)
				crHuman->joints[i]->SetJointTorque(0.0, 0);
		}
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

/////// �g���N�ɂP���͂���@/////////////
	// �e�֐߃g���N��1������
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
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

//////	����ɂ�����͂��P�͂���	///////
	else if(t > useJoNum){
		// ����ɂ�����͂ɂP������
		int i = t - (useJoNum + 1);
		if(balanceState == RIGHT_LEG){
			// �E��
			if(i < 3){
				crHuman->solids[crHuman->supportSolidNum[0][0]]->AddTorque(unitVec[i]);
			}
			else if(i >=3 && i < 6){
				crHuman->solids[crHuman->supportSolidNum[0][0]]->AddForce(unitVec[i-3]);
			}
		}
		else if(balanceState == LEFT_LEG){
			// ����
			if(i < 3){
				crHuman->solids[crHuman->supportSolidNum[1][0]]->AddTorque(unitVec[i]);	
			}
			else if(i >=3 && i < 6){
				crHuman->solids[crHuman->supportSolidNum[1][0]]->AddForce(unitVec[i-3]);
			}
		}

		// �e�֐߃g���N��0������
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->SetJointTorque(0.0, 0);
			}
		}
	}
}

void CRBalanceMultiLeg::AnkleBalance(CRHuman* crHuman){

	if(balanceState == BOTH_LEG){
		CRBalanceQuadPR::AnkleBalance(crHuman);
	}
	CalcAnkleZMP(crHuman);
	/*
	else if(balanceState == RIGHT_LEG){
		// �E��̍œK�g���N(���[�J��)�����[���h
		Vec3d balanceAnkleTorqueR = Vec3d(crHuman->joints[27]->GetJointTorque(0),-crHuman->joints[29]->GetJointTorque(0),crHuman->joints[28]->GetJointTorque(0));
		balanceAnkleTorqueR = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()*balanceAnkleTorqueR;
		// �E����ɂ������(�ڐG��)
		Vec3d ankleForceR = crHuman->supportObject[0].GetContactForce();
		// ���������Ȃ��悤�Ƀg���N�𒲐�(ZMP�v�Z�������̈�ɓ���遨���̂��߂̃g���N)
		Vec3d addAnkleTorqueR = AdjustAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
		//�E����̍��W�֕ϊ�
		Vec3f AnkleTorqueR = (crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()).inv()*addAnkleTorqueR;
		crHuman->joints[crHuman->ankleJointNum[0][0]]->SetJointTorque(AnkleTorqueR.x, 0);
		crHuman->joints[crHuman->ankleJointNum[0][1]]->SetJointTorque(AnkleTorqueR.z, 0);
		crHuman->joints[crHuman->ankleJointNum[0][2]]->SetJointTorque(-AnkleTorqueR.y, 0);
	}
	else if(balanceState == LEFT_LEG){
		// ����̍œK�g���N(���[�J��)�����[���h
		Vec3d balanceAnkleTorqueL = Vec3d(crHuman->joints[35]->GetJointTorque(0),crHuman->joints[37]->GetJointTorque(0),-crHuman->joints[36]->GetJointTorque(0));
		balanceAnkleTorqueL = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()*balanceAnkleTorqueL;
		// �E����ɂ������(�ڐG��)
		Vec3d ankleForceL = crHuman->supportObject[1].GetContactForce();
		// ���������Ȃ��悤�Ƀg���N�𒲐�(ZMP�v�Z�������̈�ɓ���遨���̂��߂̃g���N)
		Vec3d addAnkleTorqueL = AdjustAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);
		//������̍��W�֕ϊ�
		Vec3f AnkleTorqueL = (crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()).inv()*addAnkleTorqueL;
		crHuman->joints[crHuman->ankleJointNum[1][0]]->SetJointTorque(AnkleTorqueL.x,0);
		crHuman->joints[crHuman->ankleJointNum[1][1]]->SetJointTorque(-AnkleTorqueL.z,0);
		crHuman->joints[crHuman->ankleJointNum[1][2]]->SetJointTorque(AnkleTorqueL.y,0);
	}
	*/
}
void CRBalanceMultiLeg::SetBalanceState(TBlance state){
		balanceState = state;
		balanceStateChange = true;
		if(state==BOTH_LEG)
			changeQDPR=false;
}

void CRBalanceMultiLeg::CalcAnkleZMP(CRHuman* crHuman){

	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	Vec3d cog_T_J = x_cog.sub_vector(0,PTM::TVecDim<3>());
	Vec3d cog_F_J = x_cog.sub_vector(3,PTM::TVecDim<3>());

	Vec3d cog_T = cog_T_J + crHuman->GetCOG()%(crHuman->GetTotalMass()*gravityEngine->accel);
	Vec3d cog_F = cog_F_J + crHuman->GetTotalMass()*gravityEngine->accel;

	cog_zmp = (cog_F%cog_T)/cog_F.square();

	PTM::VVector<float> x_ankle;
	x_ankle.resize(ANKLE_DOF*ANKLE_NUM);
	x_ankle = - transF.inv()*(transT*optTorque+transM);

	Vec3d ankleR_T_J = x_ankle.sub_vector(0,PTM::TVecDim<3>());
	Vec3d ankleR_F_J = x_ankle.sub_vector(3,PTM::TVecDim<3>());

	Vec3d ankleR_T = ankleR_T_J +
						crHuman->solids[crHuman->supportSolidNum[0][0]]->GetCenterPosition()
							%(crHuman->solids[crHuman->supportSolidNum[0][0]]->GetMass()*gravityEngine->accel);

	Vec3d ankleR_F = ankleR_F_J + crHuman->solids[crHuman->supportSolidNum[0][0]]->GetMass()*gravityEngine->accel;
	ankleR_zmp = (ankleR_F%ankleR_T)/ankleR_F.square();

	Vec3d ankleL_T_J = x_ankle.sub_vector(6,PTM::TVecDim<3>());
	Vec3d ankleL_F_J = x_ankle.sub_vector(9,PTM::TVecDim<3>());

	Vec3d ankleL_T = ankleL_T_J +
						crHuman->solids[crHuman->supportSolidNum[1][0]]->GetCenterPosition()
							%(crHuman->solids[crHuman->supportSolidNum[1][0]]->GetMass()*gravityEngine->accel);

	Vec3d ankleL_F = ankleR_F_J + crHuman->solids[crHuman->supportSolidNum[1][0]]->GetMass()*gravityEngine->accel;
	ankleL_zmp = (ankleL_F%ankleL_T)/ankleL_F.square();

}

}		// end namespace
