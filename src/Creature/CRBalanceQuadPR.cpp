// CRBalanceQuadPR.cpp: CRBalanceQuadPR �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Creature.h"
#include "CRBalanceQuadPR.h"
#include "CRHuman.h"
#include "CRVirtualHuman.h"
#include <iostream.h>
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

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
	// �e�N���X�̏�����
	CRBalance::Init(scene,crHuman);

	// �P�ʃx�N�g���̏�����
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));


	//�@�o�����X�Ɏg�p����֐߂̐�
	unsigned int all_jo_num = 0;
	for(int i = 0; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL)
			all_jo_num++;
	}
	useJoNum = all_jo_num - crHuman->noUseJoint.size();

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
	coefficientJoint.resize(useJoNum);
	coefficientJoint.clear();
	coefficient = 0;
	coefficientP = 0;
	coefficientD = 0;

	// �œK�g���N�̏�����
	optTorque.resize(useJoNum);
	optTorque.clear();

	count = 0;
	qdprBalanceRate = 0;
	targetX.clear();

}

void CRBalanceQuadPR::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficient�̐ݒ�
	coefficient = 1.0f;
	coefficientP = 0.4f;
	coefficientD = 0.4f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJoint[i] = 0.01f;
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

void CRBalanceQuadPR::ModifyBody(SGScene* scene,CRHuman* crHuman){

	///////�@�񎟌v��@�Ńo�����X�@///////
	// VH�̐ڐG�͌v�Z
	crHuman->CalcContactForce();

	// �ړI(���B�^��)�g���N�̃Z�b�g
	SetTargetJointTorque(scene,crHuman);

	// �`�d�s��̌v�Z
	static int i = 10;
	if(i/10){
		CalcTransMatrix(scene,crHuman);
		i = 0;
	}
	i++;
	// �񎟌v��@�̎����Z�b�g
	SetQuadPR(crHuman);

	// �񎟌v��@������
	QuadPRSolve();

	// �œK�g���N���Z�b�g(����̕ۏ��)
	SetBalanceTorque(crHuman);

	// ��������̍X�V
	//RenewConstraint(crHuman);

	// �f�[�^�o��
	DataOut(crHuman);

}

void CRBalanceQuadPR::SetTargetJointTorque(SGScene* scene,CRHuman* crHuman){
	// �e�֐߂̖ڕW�g���N T0,T0��P,D�����̎擾
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

	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce[2],AnkleTorque[2];
	AnkleForce[0] = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetForce();
	AnkleForce[1] = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetForce();
	AnkleTorque[0] = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetTorque();
	AnkleTorque[1] = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF*ANKLE_NUM; i++){

		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
		// �E����
		crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->CalcAccel(scene->GetTimeStep());
		// ������
		crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->CalcAccel(scene->GetTimeStep());
		
		// ������̉����x
		static PTM::TVector<ANKLE_DOF*ANKLE_NUM,double> accelVec;
		// �E��
		accelVec.sub_vector(0,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAngularAccel();
		accelVec.sub_vector(3,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[0][crHuman->ankleJointNum[0].size()-1]]->GetSolidAccel();
		// ����
		accelVec.sub_vector(6,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAngularAccel();
		accelVec.sub_vector(9,PTM::TVecDim<3>()) = 
			crHuman->joints[crHuman->ankleJointNum[1][crHuman->ankleJointNum[1].size()-1]]->GetSolidAccel();


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
		crHuman->solids[crHuman->supportSolidNum[0][0]]->SetForce(AnkleForce[0]);
		crHuman->solids[crHuman->supportSolidNum[1][0]]->SetForce(AnkleForce[1]);
		crHuman->solids[crHuman->supportSolidNum[0][0]]->SetTorque(AnkleTorque[0]);
		crHuman->solids[crHuman->supportSolidNum[1][0]]->SetTorque(AnkleTorque[1]);
	}


	transTime = timer.Stop()*1e-3;
	timer.Clear();
//	DSTR << "�`�d�s��̌v�Z���ԁF"<< transTime << "[ms]" << std::endl;;
}

Vec3d CRBalanceQuadPR::GetJointAccel(PHJoint1D* joint){
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

void CRBalanceQuadPR::MakeTorquePattern(CRHuman* crHuman, int t){
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
		// �E��
		if(i < 3){
			crHuman->solids[crHuman->supportSolidNum[0][0]]->AddTorque(unitVec[i]);
		}
		else if(i >=3 && i < 6){
			crHuman->solids[crHuman->supportSolidNum[0][0]]->AddForce(unitVec[i-3]);
		}
		// ����
		else if(i >=6 && i < 9){
			crHuman->solids[crHuman->supportSolidNum[1][0]]->AddTorque(unitVec[i-6]);	
		}
		else{
			crHuman->solids[crHuman->supportSolidNum[1][0]]->AddForce(unitVec[i-9]);
		}


		// �e�֐߃g���N��0������
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->SetJointTorque(0.0, 0);
			}
		}
	}
}

void CRBalanceQuadPR::SetQuadPR(CRHuman* crHuman){

	// T_f'�̐ݒ�
	static PTM::TMatrixRow<ANKLE_DOF*ANKLE_NUM,ANKLE_DOF*ANKLE_NUM,double> tempT_f;
	tempT_f = - transF.inv();

	static PTM::TMatrixRow<3,12,double> tempT_T_R;	//�E���g���N
	tempT_T_R = tempT_f.sub_matrix(0,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_F_R;	//�E����
	tempT_F_R= tempT_f.sub_matrix(3,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_T_L;	//�����g���N
	tempT_T_L= tempT_f.sub_matrix(6,0, PTM::TMatDim<3,12>());
	static PTM::TMatrixRow<3,12,double> tempT_F_L;	//������
	tempT_F_L= tempT_f.sub_matrix(9,0, PTM::TMatDim<3,12>());
	
	// T�̐ݒ�
	static PTM::TMatrixRow<BALANCE_DOF,ANKLE_DOF*ANKLE_NUM,double> tempT;
	tempT.sub_matrix(0,0, PTM::TMatDim<3,12>()) = tempT_T_R + tempT_T_L;
	tempT.sub_matrix(3,0, PTM::TMatDim<3,12>()) = tempT_F_R + tempT_F_L;
	//tempT *= -1;

	// A�̐ݒ�
	tempA = tempT*transT;

	// b�̐ݒ�
	static PTM::TVector<BALANCE_DOF,double> f_mass;		// �d��
	f_mass.sub_vector(0,PTM::TVecDim<3>()) = Vec3d();
	f_mass.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;

	static PTM::TVector<BALANCE_DOF,double> f_contact;	// �ڐG��
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactTorque()-crHuman->GetCOG()%crHuman->supportArea.GetContactForce();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->supportArea.GetContactForce();

	tempB = tempT*transM + f_mass + f_contact;


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
	targetX.sub_vector(0,PTM::TVecDim<3>()) = qdprBalanceRate*GetTargetTorque();
	targetX.sub_vector(3,PTM::TVecDim<3>()) = GetTargetForce() - crHuman->GetTotalMass()*(GetTargetAccel()-gravityEngine->accel);
	quadMatC = (targetX - tempB)*tempA + coefficientP*targetTorqueP + coefficientD*targetTorqueD;
	quadPR.vecC = quadMatC;

}

void CRBalanceQuadPR::QuadPRSolve(){
	timer.Start();
	// �񎟌v��@������
	count = quadPR.Solve();
	quadTime = timer.Stop()*1e-3;
	timer.Clear();
//	DSTR << "�񎟌v��@�̌v�Z�F"<< quadTime << "[ms]" << std::endl;;
//	DSTR << "���s�񐔁F" << count << std::endl;

	// �œK�g���N
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
}

void CRBalanceQuadPR::SetBalanceTorque(CRHuman* crHuman){
	// �œK�g���N��̂̃o�����X�Ɏg�p�����֐߂ɃZ�b�g����
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
	// ����ZMP����������o�Ȃ��悤�ɑ���g���N�𒲐�
	AnkleBalance(crHuman);
}

void CRBalanceQuadPR::AnkleBalance(CRHuman* crHuman){

	// �ĂP
	// ������̍œK�g���N(���[�J��)�����[���h
	Vec3d balanceAnkleTorqueR = Vec3d(crHuman->joints[27]->GetJointTorque(0),-crHuman->joints[29]->GetJointTorque(0),crHuman->joints[28]->GetJointTorque(0));
	Vec3d balanceAnkleTorqueL = Vec3d(crHuman->joints[35]->GetJointTorque(0),crHuman->joints[37]->GetJointTorque(0),-crHuman->joints[36]->GetJointTorque(0));
	balanceAnkleTorqueR = crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()*balanceAnkleTorqueR;
	balanceAnkleTorqueL = crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()*balanceAnkleTorqueL;

	// ������ɂ������(�ڐG��)
	Vec3d ankleForceR = crHuman->supportObject[0].GetContactForce();
	Vec3d ankleForceL = crHuman->supportObject[1].GetContactForce();

	// ���������Ȃ��悤�Ƀg���N�𒲐�(ZMP�v�Z�������̈�ɓ���遨���̂��߂̃g���N)
	Vec3d addAnkleTorqueR = AdjustAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
	Vec3d addAnkleTorqueL = AdjustAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);

	//�E����̍��W�֕ϊ�
	Vec3f AnkleTorqueR = (crHuman->solids[crHuman->supportSolidNum[0][0]]->GetRotation()).inv()*addAnkleTorqueR;
	crHuman->joints[crHuman->ankleJointNum[0][0]]->SetJointTorque(AnkleTorqueR.x, 0);
	crHuman->joints[crHuman->ankleJointNum[0][1]]->SetJointTorque(AnkleTorqueR.z, 0);
	crHuman->joints[crHuman->ankleJointNum[0][2]]->SetJointTorque(-AnkleTorqueR.y, 0);

	//������̍��W�֕ϊ�
	Vec3f AnkleTorqueL = (crHuman->solids[crHuman->supportSolidNum[1][0]]->GetRotation()).inv()*addAnkleTorqueL;
	crHuman->joints[crHuman->ankleJointNum[1][0]]->SetJointTorque(AnkleTorqueL.x,0);
	crHuman->joints[crHuman->ankleJointNum[1][1]]->SetJointTorque(-AnkleTorqueL.z,0);
	crHuman->joints[crHuman->ankleJointNum[1][2]]->SetJointTorque(AnkleTorqueL.y,0);


	/*
	// �ĂQ
	//�@�œK���������ɂ�����͂ƃg���N
	PTM::VVector<float> x_a;
	x_a.resize(ANKLE_DOF*ANKLE_NUM);
	x_a = -transF.inv()*(transT*optTorque + transM);
	Vec3d ankleForceR = x_a.sub_vector(3,PTM::TVecDim<3>());
	Vec3d ankleForceL = x_a.sub_vector(9,PTM::TVecDim<3>());

	Vec3d balanceAnkleTorqueR = x_a.sub_vector(0,PTM::TVecDim<3>());
	Vec3d balanceAnkleTorqueL = x_a.sub_vector(6,PTM::TVecDim<3>());

	// ���������Ȃ��悤�Ƀg���N�𒲐�
	Vec3d addAnkleTorqueR = AdjustAnkleTorque(crHuman,ankleForceR,balanceAnkleTorqueR,0);
	Vec3d addAnkleTorqueL = AdjustAnkleTorque(crHuman,ankleForceL,balanceAnkleTorqueL,1);

	// �E��
	crHuman->joints[crHuman->ankleJointNum[0][0]]->AddTorque(addAnkleTorqueR.x/2);
	crHuman->joints[crHuman->ankleJointNum[0][1]]->AddTorque(addAnkleTorqueR.z/2);
	crHuman->joints[crHuman->ankleJointNum[0][2]]->AddTorque(addAnkleTorqueR.y/2);
	// ����
	crHuman->joints[crHuman->ankleJointNum[1][0]]->AddTorque(addAnkleTorqueL.x/2);
	crHuman->joints[crHuman->ankleJointNum[1][1]]->AddTorque(addAnkleTorqueL.z/2);
	crHuman->joints[crHuman->ankleJointNum[1][2]]->AddTorque(addAnkleTorqueL.y/2);
	*/
}


Vec3d CRBalanceQuadPR::AdjustAnkleTorque(CRHuman* crHuman,Vec3d ankleForce,Vec3d ankleTorque,int ankle){

	// �ĂP
	// ZMP���v�Z
	ankleGoalZmp[ankle] = (ankleForce % ankleTorque) / ankleForce.square();
	ankleGoalZmp[ankle] += crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetFrame()->GetPosture()
							*crHuman->joints[crHuman->ankleJointNum[ankle][2]]->GetPostureFromChild().Pos();
	ankleGoalZmp[ankle].Y() = 0.0f;

	// �Б���ZMP���Б��̐ڐG���p�`�̒��ɂ��邩�ǂ����A�Ȃ��ꍇ�͍ŋߖT�_�����߂�
	if(!AnkleZmpCheck(ankleGoalZmp[ankle],&crHuman->supportObject[ankle])){
		// �ŋߖT�_�����߂�
		ankleRealZmp[ankle]=crHuman->supportObject[ankle].GetSupportArea().ClosestPoint(ankleGoalZmp[ankle]);
		ankleTorque = (ankleRealZmp[ankle] - crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetFrame()->GetPosture()
						*crHuman->joints[crHuman->ankleJointNum[ankle][2]]->GetPostureFromChild().Pos())% ankleForce;
		
		return ankleTorque;
	}else{
		ankleRealZmp[ankle] = ankleGoalZmp[ankle];
	}
	return ankleTorque;

	/*
	// �ĂQ
	// ZMP���v�Z
	ankleGoalZmp[ankle] = (ankleForce % ankleTorque) / ankleForce.square();
	ankleGoalZmp[ankle] += crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetCenterPosition();
	ankleGoalZmp[ankle].Y() = 0.0f;

	// �Б���ZMP���Б��̐ڐG���p�`�̒��ɂ��邩�ǂ����A�Ȃ��ꍇ�͍ŋߖT�_�����߂�
	if(!AnkleZmpCheck(ankleGoalZmp[ankle],&crHuman->supportObject[ankle])){
		// �ŋߖT�_�����߂�
		ankleRealZmp[ankle]=crHuman->supportObject[ankle].GetSupportArea().ClosestPoint(ankleGoalZmp[ankle]);
		// �ŋߖT�_�ł̑��ɂ�����g���N�����߂�
		Vec3d newAnkleTorque = (ankleRealZmp[ankle] - crHuman->solids[crHuman->supportSolidNum[ankle][0]]->GetCenterPosition())
										% ankleForce;
		static PTM::VMatrixRow<float> tempFT;
		tempFT.resize(ANKLE_DOF*ANKLE_NUM, useJoNum);
		tempFT = (-transF.inv())*transT;
		static PTM::TMatrixRow<3,3,double> tempT_a;
		if(!ankle){ // �E��
			tempT_a = tempFT.vsub_matrix(0,27,3,3);
		}
		else{		// ����
			tempT_a = tempFT.vsub_matrix(6,34,3,3);
		}
		Vec3d addTorque = tempT_a.inv()*(ankleTorque - newAnkleTorque);
		return Vec3d(addTorque.x,addTorque.z,addTorque.y);	// ����֐߂̏��Ԃ�x,z,y������
	}else{
		ankleRealZmp[ankle] = ankleGoalZmp[ankle];
	}
	return Vec3d();
	*/
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

	// �E����
	Vec3f maxRangeR,minRangeR;
	NewConstraint(crHuman,crHuman->solids[crHuman->supportSolidNum[0][0]],&maxRangeR,&minRangeR,0);
	// ������
	Vec3f maxRangeL,minRangeL;
	NewConstraint(crHuman,crHuman->solids[crHuman->supportSolidNum[1][0]],&maxRangeL,&minRangeL,1);

	/*
	quadPR.SetOneRange(27,20,-50);
	quadPR.SetOneRange(28,17,-17);
	quadPR.SetOneRange(34,20,-50);
	quadPR.SetOneRange(35,17,-17);
	*/
	/*		
	// ��������̐ݒ�
	// �E����
	quadPR.SetOneRange(27,maxRangeR.x,minRangeR.x);
	quadPR.SetOneRange(28,maxRangeR.z,minRangeR.z);

	// ������
	quadPR.SetOneRange(34,maxRangeL.x,minRangeL.x);
	quadPR.SetOneRange(35,maxRangeL.z,minRangeL.z);
	*/
	
/*
	// ��������̐ݒ�
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
	
	// ��ԋ߂�&��������̈�̒��_
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

	if(!ankleSide){	//�E��
		maxRange->z = -std::min(maxTorque.z,minTorque.z);
		minRange->z = -std::max(maxTorque.z,minTorque.z);
	}
	else{			//����
		maxRange->z = std::max(maxTorque.z,minTorque.z);
		minRange->z = std::min(maxTorque.z,minTorque.z);
	}
	DSTR << ankleSide <<"maxTorque" << *maxRange <<std::endl;
	DSTR << ankleSide <<"minTorque" << *minRange <<std::endl;


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

	//�o�����X�ɕK�v�ȗ�
	of << "�o�����X�ɕK�v�ȃg���N�Ɨ�" << std::endl;
	for(int i = 0; i < targetX.size(); i++)
		of << targetX[i] << "\t";
	of << std::endl;

	//�@�œK�����d�S�ɂ�����͂ƃg���N
	of << "�œK�����d�S�ɂ�����g���N�Ɨ�" << std::endl;
	PTM::VVector<float> x_cog;
	x_cog.resize(BALANCE_DOF);
	x_cog = tempA*optTorque+tempB;

	for(int i =0; i < BALANCE_DOF; i++)
		of << x_cog[i] << "\t";
	of << std::endl;


	//���ۂ̏d�S�̃g���N
	of << "���ۂ̃g���N�Ɨ�" << std::endl;
	Vec3f realTorque = crHuman->supportArea.GetContactTorque()-crHuman->GetCOG()%crHuman->supportArea.GetContactForce();;
	for(int i =0; i < 3; i++)
		of << realTorque[i] << "\t";
	
	//���ۂ̏d�S�̗�
	Vec3f realForce = crHuman->supportArea.GetContactForce() + crHuman->GetTotalMass()*gravityEngine->accel;
	for(int i =0; i < 3; i++)
		of << realForce[i] << "\t";
	of<<std::endl;

	DSTR << "�g���N�̊���";
	for(int i = 0; i < 3; i ++){
		DSTR << abs(realTorque[i])/abs(targetX[i]) << "\t";
	}
	DSTR << std::endl;
	DSTR << "�͂̊���";
	for(int i = 0; i < 3; i ++){
		DSTR << abs(realForce[i])/abs(targetX[i+3]) << "\t";
	}
	DSTR << std::endl;




	// �œK���̏o��
	of << "�œK��" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}
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

	// �œK���̏o��
	of << std::endl;
	of << "�œK��" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << quadPR.vecX[i] << "\t";
	}

	// Q�̏o��
	of << std::endl;
	of << "Q" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		for(int j = 0 ; j < useJoNum; j++){
			of << quadPR.matQ[i][j] << "\t";
		}
		of << std::endl;
	}

	// C�̏o��
	of << std::endl;
	of << "C" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
			of << quadPR.vecC[i] << "\t";
	}

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


}		//	end namespace Spr