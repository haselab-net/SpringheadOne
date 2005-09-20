// CRReachingMovment.cpp: CRReachingMovment �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRReachingMovment.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;
using namespace std;

CRReachingMovment::CRReachingMovment()
{

}

CRReachingMovment::~CRReachingMovment()
{

}

void CRReachingMovment::Load(){
	bStart = false;
	startStep =0;
	goalTime = 0.0;
	goalPos = Vec3f();
	startPos = Vec3f();
	jerkPos = Vec3f();
	jerkVelocity = Vec3f();

	jacobian.clear();
	jacobianInv.clear();
	weightMat.clear();
	jointTorque.clear();
	jointAngle.clear();
	jointVelocity.clear();


	// �d�ݍs��i������������蓮���j�A�P�ʍs��
	for(int i = 0; i < weightMat.height(); i++){
		for(int j = 0; j < weightMat.width(); j++){
			if(i==j){
				//�@��
				if(i <= 2){
					weightMat[i][j] = 10;
				}
				// �I
				else if(i >2 && i < 5){
					weightMat[i][j] = 0.1;
				}
				// ���
				else{
					weightMat[i][j] = 0.3;
				}
			}
		}
	}
	
}

void CRReachingMovment::Step(SGScene* scene,CRHuman* crHuman){

	if(bStart){
		if(nowTime > goalTime){
			bStart = false;
			nowTime = 0.0;
		}
		else{
			MinimumJerkModel(scene,crHuman);
			MakeTorque(scene,crHuman);
			//DataOut(crHuman);
		}
	}

}

void CRReachingMovment::Draw(GRRender* render){

	if(bStart){
		render->SetModelMatrix(Affinef());
		render->SetMaterial(GRMaterialData(Spr::Vec4f(0.5,1.0,0.5,1)));
		render->SetLineWidth(10);

		Vec3f v= startPos + (startPos - goalPos)*(15*pow((nowTime/goalTime),4.0) - 6*pow((nowTime/goalTime),5.0) - 10*pow((nowTime/goalTime),3.0));
		render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
		/*
		for(float time = 0; time <= 10; time++){
			v = startPos + (startPos - goalPos)*(15*pow((time/goalTime),4.0) - 6*pow((time/goalTime),5.0) - 10*pow((time/goalTime),3.0));
			render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
		}
		*/
	}
}

void CRReachingMovment::MinimumJerkModel(SGScene* scene,CRHuman* crHuman){
	// ���x�ŏ����f���ɂ����̈ʒu�A���x�v�Z
	double time = scene->GetTimeStep()*(scene->GetCount() - startStep);
	nowTime = time;
	jerkPos = startPos + (startPos - goalPos)*(15*pow((time/goalTime),4.0) - 6*pow((time/goalTime),5.0) - 10*pow((time/goalTime),3.0));
	jerkVelocity = (startPos - goalPos)*(60*(pow(time,3)/pow(goalTime,4.0))
							- 30*(pow(time,4)/pow(goalTime,5.0)) - 30*(pow(time,2.0)/pow(goalTime,3.0)));
	DSTR << time << std::endl;
	DSTR << nowTime << std::endl;
	DSTR << goalTime << std::endl;
	// ���x�ŏ����f���ƃ��R�r�A���Ŋe�֐߂̈ʒu�A���x�v�Z
	// ���R�r�s��̍쐬
	MakeJacobian(crHuman,jerkPos);

	// �[���t�s��(�d�ݕt��)
	//jacobianInv = jacobian.trans()*((jacobian*jacobian.trans()).inv());
	jacobianInv = weightMat.inv()*jacobian.trans()*((jacobian*weightMat.inv()*jacobian.trans()).inv());

	// ���x
	jointVelocity = jacobianInv*jerkVelocity;
	//jointVelocity =jacobianInv*jerkVelocity +(unitMat - jacobianInv*jacobian)*optionVec;

	// �ʒu
	static PTM::TVector<7,float> currentAngle;
	for(int i = 9; i <= 15; i++){
		currentAngle[i-9] = crHuman->joints[i]->GetJointPosition(0);
	}
	jointAngle = currentAngle + jacobianInv*(jerkPos - crHuman->solids[6]->GetCenterPosition());
	for(int i = 0; i < jointAngle.size(); i++){
		while(jointAngle[i] < -M_PI){
			jointAngle[i] += 2*M_PI;
		}
		while(jointAngle[i] >M_PI){
			jointAngle[i] -= 2*M_PI;
		}

	}
	//DSTR << "jointAngle" << jointAngle << std::endl;
	//DSTR << "jointVelocity" << jointVelocity << std::endl;
}

void CRReachingMovment::MakeJacobian(CRHuman* crHuman,Vec3d targetPos){

	for(int i = 9; i < 9+JARK_JOINT; i++){
		jacobian.col(i-9) = MakeOneJacobian(crHuman->joints[i],targetPos);
	}
}

Vec3f CRReachingMovment::MakeOneJacobian(PHJointBase* joint,Vec3f targetPos){
	// ���i�̂�
	Vec3f jacobi;
	Matrix3f jointRot =joint->GetPostureFromParent().Rot();
	Vec3f jointRotVec = joint->GetParent()->solid->GetRotation()*jointRot.Ez();
	jacobi = jointRotVec%(targetPos - GetWorldPos(joint));

	//���i�Ɖ�]
	/*
	PTM::TVector<6,float> jacobi;
	Matrix3f jointRot =joint->GetPostureFromParent().Rot();
	Vec3f jointRotVec = joint->GetParent()->solid->GetRotation()*jointRot.Ez();
	jacobi.sub_vector(0,PTM::TVecDim<3>()) = jointRotVec%(targetPos - GetWorldPos(joint));
	jacobi.sub_vector(3,PTM::TVecDim<3>()) = jointRotVec;
	*/

	return jacobi;
}

Vec3f CRReachingMovment::GetWorldPos(PHJointBase* joint){
	return joint->solid->GetFrame()->GetPosture()*joint->GetPostureFromChild().Pos();
}

void CRReachingMovment::MakeTorque(SGScene* scene,CRHuman* crHuman){
	
	for(int i = 9; i < 9+JARK_JOINT; i++){
		crHuman->jointPids[i]->SetPGoal(jointAngle[i-9] - crHuman->joints[i]->GetJointPosition(0));
		crHuman->jointPids[i]->SetDGoal(jointVelocity[i-9] - crHuman->joints[i]->GetJointVelocity(0));
	}
}

void CRReachingMovment::SetTarget(SGScene* scene,CRHuman* crHuman,Vec3f goal){

	startPos = crHuman->solids[6]->GetCenterPosition();		// ���B�^���̍��̂̃X�^�[�g�ʒu
	startStep = scene->GetCount();

	// ���x�ŏ����f���Ɏg���^�����ԂƓ��B�ʒu
	goalTime = 1.0;
	goalPos = goal;
	// ���B�^���X�^�[�g
	StartReachingMovment();
}

void CRReachingMovment::DataOut(CRHuman* crHuman){
	static bool bFirst = true;
	std::ofstream of;
	if (bFirst){
		of.open("ReachingMovment.xls");
		bFirst = false;
		
	}else{
		//of.open("ReachingMovment.xls");
		of.open("ReachingMovment.xls",ios::app);
	}

	for(int i = 0; i < jointVelocity.size(); i++){
		of << jointVelocity[i] << "\t";
	}
	of << "\t";
	for(int i = 0; i < jointAngle.size(); i++){
		of << jointAngle[i] << "\t";
	}
	of << std::endl;

/*
	// ���R�r�A��
	of << "���R�r�A��" << std::endl;
	for(int i = 0; i < jacobian.height(); i++){
		for(int j = 0; j < jacobian.width(); j++){
			of << jacobian[i][j] << "\t";
		}
		of << std::endl;
	}
	of << std::endl;

	of << "�[���t�s��" << std::endl;
	for(int i = 0; i < jacobianInv.height(); i++){
		for(int j = 0; j < jacobianInv.width(); j++){
			of << jacobianInv[i][j] << "\t";
		}
		of << std::endl;
	}
	of << std::endl;


	// ���x�ŏ����f���ɂ����̑��x
	Vec3f v;
	for(float time = 0; time <= goalTime; time += 0.1){
		v = (startPos - goalPos)*(60*(pow(time,3)/pow(goalTime,4.0)) - 30*(pow(time,4)/pow(goalTime,5.0)) - 30*(pow(time,2.0)/pow(goalTime,3.0)));
		for(int i = 0; i < 3; i++){
			of << v[i] << "\t";
		}
		of << std::endl;
	}
*/
	of.close();
}

}		// end namespace
