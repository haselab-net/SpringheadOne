// Balance.cpp: Balance �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Balance.h"
#include "Model.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
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
	noJoint.push_back(crHuman->joints.size()-1);	// �y���܂�
	noJoint.push_back(crHuman->joints.size()-2);	// ����֐�(-Y)
	noJoint.push_back(crHuman->joints.size()-3);	// ����֐�(Z)
	noJoint.push_back(crHuman->joints.size()-4);	// ����֐�(X)
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

	// �P�ʃx�N�g���̏�����
	unitVec.clear();
	unitVec.push_back(Vec3d(1.0,0.0,0.0));
	unitVec.push_back(Vec3d(0.0,1.0,0.0));
	unitVec.push_back(Vec3d(0.0,0.0,1.0));

	// �o�����X�Ɏg��Ȃ��֐߂�o�^
	RegistNoJoint(crHuman);

	//�@�o�����X�Ɏg�p����֐߂̐�
	useJoNum = crHuman->joints.size() - noJoint.size();

	// �`�d�s��̃T�C�Y�̐ݒ�&������
	transT.resize(ANKLE_DOF_*ANKLE_NUMBER, useJoNum);
	transF.resize(ANKLE_DOF_*ANKLE_NUMBER,ANKLE_DOF_*ANKLE_NUMBER);
	transM.resize(ANKLE_DOF_*ANKLE_NUMBER);
	transT.clear();
	transF.clear();
	transM.clear();

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
	cogForce.clear();

}
void Balance::SetBalanceParameter(CRHuman* crHuman){

	CRBalance::SetBalanceParameter(crHuman);

	// coefficient�̐ݒ�
	coefficient = 1.0f;
	coefficientP = 0.7f;
	coefficientD = 1.0f;
	for(int i = 0; i < useJoNum; i++){
		coefficientJo[i] = 0.8f;
	}
	//coefficientJo[2] = 0.0;

	// �񎟌v��@�̊���
	qdprBalanceRate = 0.8f;

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
void Balance::Measure(SGScene* scene,CRHuman* crHuman){
	CalcBalanceTorque(scene,crHuman);			// �o�����X�ɕK�v�ȃg���N���v�Z
	CalcTargetZMP(scene,crHuman);				// �ڕWZMP�̌v�Z
}
void Balance::ModifyBody(SGScene* scene,CRHuman* crHuman){
	
	///////�@����Ńo�����X�@///////
	/*
	Vec3d targetT_R = (crHuman->solids[crHuman->solids.size()-2]->GetRotation()).inv()*GetTargetTorque();
	crHuman->joints[crHuman->joints.size()-4]->SetTorque(-targetT_R.x);
	crHuman->joints[crHuman->joints.size()-3]->SetTorque(-targetT_R.z);
	crHuman->joints[crHuman->joints.size()-2]->SetTorque(-targetT_R.y);
	*/
	///////�@�񎟌v��@�Ńo�����X�@///////
	// �ړI(���B�^��)�g���N�̃Z�b�g
	SetPreQuadPR(scene,crHuman);
	// �`�d�s��̌v�Z
	static int i = 0;
	if(i/10){
		CalcTransMatrix(scene,crHuman);
		i = 0;
	}
	i++;
	// �񎟌v��@�̎����Z�b�g
	SetQuadPR(crHuman);
	// �񎟌v��@������
	QuadPRSolve();
	// �œK�g���N���Z�b�g
	SetJointTorque(crHuman);
	// ����Ńo�����X�̕⏞(�œK�g���N�ƃo�����X�g���N�̍�)
	SetAnkle(crHuman);
	// �f�[�^�o��
	//DataOut(crHuman);
}

void Balance::SetPreQuadPR(SGScene* scene,CRHuman* crHuman){
	// �e�֐߂̖ڕW�g���N T0,T0��P,D�����̎擾
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

	// ���̍��̗̂́A�g���N��ޔ�
	Vec3d AnkleForce,AnkleTorque;
	AnkleForce = crHuman->solids[crHuman->solids.size()-2]->GetForce();
	AnkleTorque = crHuman->solids[crHuman->solids.size()-2]->GetTorque();

	jointEngine->root->CompCoriolisAccelRecursive(scene->GetTimeStep());

	for(int i = 0; i <= useJoNum+ANKLE_DOF_*ANKLE_NUMBER; i++){
		MakeTorquePattern(crHuman,i);

		jointEngine->root->CompArticulatedInertia(scene->GetTimeStep());

		// �t�F�U�[�X�g�[���̈ꕔ���g���A���E����Ō��Joint�̉����x�v�Z
		crHuman->joints[crHuman->joints.size()-2]->CalcAccel(scene->GetTimeStep());	// �E����
		
		// ������̉����x
		PTM::TVector<ANKLE_DOF_,double> accelVec;
		accelVec = GetJointAccel(crHuman->joints[crHuman->joints.size()-2]);

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
		crHuman->solids[crHuman->solids.size()-2]->SetForce(AnkleForce);
		crHuman->solids[crHuman->solids.size()-2]->SetTorque(AnkleTorque);
	}

	DSTR << "�`�d�s��̌v�Z���ԁF"<< timer.Stop()*1e-3 << "[ms]" << std::endl;;
	timer.Clear();
}
Vec3d Balance::GetJointAccel(PHJoint1D* joint){
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
void Balance::MakeTorquePattern(CRHuman* crHuman, int t){
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
		// ����ɂ�����͂�0�ɁE�E�E����Ȃ����Ƃ�0
	}

//////	����ɂ�����͂��P�͂���	///////
	else if(t > useJoNum){
		// ����ɂ�����͂ɂP������
		int i = t - (useJoNum + 1);
		crHuman->solids[crHuman->solids.size()-2]->AddForce(unitVec[i]
			,crHuman->solids[crHuman->solids.size()-2]->GetFrame()->GetPosture()*crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos());

		// �e�֐߃g���N��0������
		for(int i = 0; i < crHuman->joints.size(); i++){
			if(crHuman->joints[i] != NULL){
				crHuman->joints[i]->torque = 0.0;
			}
		}
	}
}
void Balance::DataOut(CRHuman* crHuman){

	std::ofstream of("quadPR.xls");

	// �֐߂̖��O
	for(int i = 0 ; i < crHuman->joints.size(); i++){
		if(crHuman->joints[i] != NULL){
			of << crHuman->joints[i]->GetName() << "\t";
		}
	}
	of << std::endl;

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

	// �o�����X�ɕK�v�ȃg���N�̏o��
	of << std::endl;
	of << "�o�����X�ɕK�v�ȃg���N" << std::endl;
	for(int i = 0 ; i < useJoNum; i++){
		of << BalanceTorque[i] << "\t";
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
	
	of.close();
}

void Balance::SetQuadPR(CRHuman* crHuman){
	// �d�S���瑫��ւ̋����i�`�d�s������߂�O�ƌ�ł��l�ς�炸�j
	Vec3d cog =crHuman->GetCOG();
	Vec3d posCog_Ankle = crHuman->joints[crHuman->joints.size()-2]->solid->GetFrame()->GetPosture()
									* crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos() - cog;

	// T_f'�̐ݒ�
	PTM::TMatrixRow<ANKLE_DOF_*ANKLE_NUMBER,ANKLE_DOF_*ANKLE_NUMBER,double> tempT_f = - transF.inv();

	// T�̐ݒ�
	PTM::TMatrixRow<DOF_NUM,ANKLE_DOF_,double> tempT;
	Matrix3d e;
	tempT.sub_matrix(0,0, PTM::TMatDim<ANKLE_DOF_,ANKLE_DOF_>()) = e.Unit();
	tempT.sub_matrix(3,0, PTM::TMatDim<ANKLE_DOF_,ANKLE_DOF_>()) = Matrix3d::Cross(posCog_Ankle);
	//tempT *= -1;

	// A�̐ݒ�
	PTM::VMatrixRow<double> tempA;
	tempA.resize(DOF_NUM,useJoNum);
	tempA = tempT*tempT_f*transT;

	// b�̐ݒ�
	PTM::VVector<double> tempB;
	tempB.resize(DOF_NUM);
	PTM::TVector<DOF_NUM,double> f_m;		// �d��
	f_m.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetTotalMass()*gravityEngine->accel;
	f_m.sub_vector(3,PTM::TVecDim<3>()) = Vec3d();
	PTM::TVector<DOF_NUM,double> f_contact;		// �ڐG��
	f_contact.sub_vector(0,PTM::TVecDim<3>()) = crHuman->GetContactForce();
	f_contact.sub_vector(3,PTM::TVecDim<3>()) = crHuman->GetContactTorque();
	tempB = tempT*tempT_f*transM + f_m + f_contact;


	// Q�̐ݒ�
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

	// C�̐ݒ�
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
	DSTR << "�񎟌v��@�̌v�Z���ԁF"<< timer.Stop()*1e-3 << "[ms]" << std::endl;;
	timer.Clear();
	DSTR << "���s�񐔁F" << count << std::endl;
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

	//�@�o�����X�ɕK�v�ȃg���N�ƍœK���̍��𑫎񂪕⏞
	PTM::VVector<float> optTorque;
	optTorque.resize(useJoNum);
	for(int i = 0; i < useJoNum; i++){
		optTorque[i] = quadPR.vecX[i];
	}
	PTM::VVector<float> x_cog;
	x_cog.resize(DOF_NUM);
	x_cog = optA*optTorque+optB;

	/*
	DSTR << "�e�֐߂��d�S�ɗ^����g���N"<<x_cog.sub_vector(3,PTM::TVecDim<3>()) <<std::endl;
	DSTR << "�o�����X�ɕK�v�ȃg���N�g���N"<<GetTargetTorque() <<std::endl;
	*/

	Vec3d targetT = GetTargetTorque();
	Vec3d balanceTorque =  qdprBalanceRate*targetT - x_cog.sub_vector(3,PTM::TVecDim<3>());
	balanceTorque += (1-qdprBalanceRate)*targetT;

	Vec3d ankleForce = -transF.trans()*(transT*optTorque + transM);

	AdjustAnkleTorque(crHuman,ankleForce,balanceTorque,0);

	/*
	//����̍��W�֕ϊ�
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

	// ZMP���v�Z
	ankleZmp = (ankleForce % ankleTorque) / ankleForce.square();
	ankleZmp += crHuman->solids[crHuman->solids.size()-2]->GetFrame()->GetPosture()*crHuman->joints[crHuman->joints.size()-2]->GetPostureFromChild().Pos();
	ankleZmp.Y() = 0.0f;

	// �Б���ZMP���Б��̐ڐG���p�`�̒��ɂ��邩�ǂ����A�Ȃ��ꍇ�͍ŋߖT�_�����߂�
	if(!AnkleZmpCheck(ankleZmp,&crHuman->supportObject[ankle])){
		// �ŋߖT�_�����߂�
		ankleZmp=ClosestPoint(ankleZmp,&crHuman->supportObject[ankle]);

		// �ŋߖT�_�ł̑��ɂ�����g���N�����߂�
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