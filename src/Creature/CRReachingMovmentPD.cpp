// CRReachingMovmentPD.cpp: CRReachingMovmentPD �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRReachingMovmentPD.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRReachingMovmentPD::CRReachingMovmentPD()
{

}

CRReachingMovmentPD::~CRReachingMovmentPD()
{

}

void CRReachingMovmentPD::Step(SGScene* scene){

	// �o�^���ꂽ���̂����B�^�x���Ă��邩�ۂ�
	bool start = false;
	for(int i = 0; i < movmentSolid.size(); i++){
		start |= !movmentSolid[i].bGoal;
	}
	bMove = start;

	// ���B�^������
	if(start){
		// ���B���Ă��邩�`�F�b�N
		CheckReachingMiovment(scene);
		// ���x�ŏ����f��
		MinimumJerkModel(scene);
		// PD����
		PDControl(scene);
		//StayCurrentPos();
		//DataOut();
	}
}
void CRReachingMovmentPD::CheckReachingMiovment(SGScene* scene){
	for(int i = 0; i < movmentSolid.size(); i++){
		if(!movmentSolid[i].bGoal){
			movmentSolid[i].bGoal = CheckGoal(&movmentSolid[i],scene);

			// ���B�^�����I������ꍇ
			if(movmentSolid[i].bGoal){
				// ���̏�ɌŒ�
				//StayHere(&movmentSolid[i],i);
				// ������
				Init(&movmentSolid[i]);
				//�@�֐߂̍d�������Ƃɖ߂�
				for(int j = 0; j < movmentSolid[i].useJoints.size(); j++){
					model->jointPids[movmentSolid[i].useJoints[j]]->proportional = movmentSolid[i].useJointsSpring[j][0];
					model->jointPids[movmentSolid[i].useJoints[j]]->differential = movmentSolid[i].useJointsSpring[j][1];
					model->jointPids[movmentSolid[i].useJoints[j]]->integral = movmentSolid[i].useJointsSpring[j][2];
				}
			}
		}
	}

}

bool CRReachingMovmentPD::CheckGoal(CRMovmentSolid* moveSolid,SGScene* scene){

	moveSolid->nowTime = scene->GetTimeStep()*(scene->GetCount() - moveSolid->startStep);
	//if(moveSolid->nowTime > moveSolid->goalTime){
	if(moveSolid->bStyle){
		if(moveSolid->nowTime > moveSolid->goalTime){
			moveSolid->bStyle = false;
		}
		return false;
	}
	else{
		if(moveSolid->nowTime > moveSolid->goalTime*2){
			// ���B�^���I��
			return true;
		}
		else if((moveSolid->solid->GetCenterPosition() - moveSolid->goalPos).norm() < 0.02){
			// ���B�^���I��
			return true;
		}else{
			// ���B�^���p��
			return false;
		}
		

	}
}

void CRReachingMovmentPD::Load(CRHuman* crHuman){

	movmentSolid.clear();
	bMove = false;
	offSpring = false;

	model = NULL;
	model = crHuman;
}


void CRReachingMovmentPD::SetTarget(SGScene* scene,Vec3f goal){

	for(int i = 0; i < movmentSolid.size(); i++){
		if(movmentSolid[i].bStart){

			// �X�^�[�g�C���B�n�_
			movmentSolid[i].goalPos = goal;
			movmentSolid[i].startPos = movmentSolid[i].solid->GetCenterPosition();

			// �������B����
			movmentSolid[i].firstDistance = (movmentSolid[i].goalPos - movmentSolid[i].startPos).norm();

			// �J�n�X�e�b�v
			movmentSolid[i].startStep = scene->GetCount();

			//���̕t�߂̊֐߂��_�炩������
			for(int j = 0; j < movmentSolid[i].useJoints.size(); j++){
				if(j < 3){//��
					//model->JointPIDMul(model->jointPids[movmentSolid[i].useJoints[j]],0.05f,0.01f);
					if(offSpring)
						model->JointPIDMul(model->jointPids[movmentSolid[i].useJoints[j]],0.05f,0.01f);
					else
						model->JointPIDMul(model->jointPids[movmentSolid[i].useJoints[j]],0.5f,0.1f);
						
				}
				else if(j > 2 && j < 5){	//�I
					model->JointPIDMul(model->jointPids[movmentSolid[i].useJoints[j]],0.0001f,0.0001f);
				}
				else{
					model->JointPIDMul(model->jointPids[movmentSolid[i].useJoints[j]],5.0f,1.0f);
				}
			}

			// �J�n�ݒ�I���C���B�͂܂����Ă��Ȃ��D
			movmentSolid[i].bStart = false;
			movmentSolid[i].bGoal = false;
		}
	}
}

void CRReachingMovmentPD::PDControl(SGScene* scene){

	for(int i = 0; i < movmentSolid.size(); i++){
		if(!movmentSolid[i].bGoal){

			Vec3d dPos = movmentSolid[i].goalPos - movmentSolid[i].solid->GetCenterPosition();

			// �͌v�Z
			if(!(movmentSolid[i].bStyle)){ // ������PD����
				movmentSolid[i].pdForce = movmentSolid[i].spring*dPos - movmentSolid[i].damp*movmentSolid[i].solid->GetVelocity();
				StayCurrentPos();
			}
			else{ // ���x�ŏ����f�� + PD����
				movmentSolid[i].pdForce = movmentSolid[i].spring*(dPos/dPos.norm())*(dPos.norm() - movmentSolid[i].jerkPos)
													+ movmentSolid[i].damp*(dPos/dPos.norm()*movmentSolid[i].jerkVelocity.norm() - movmentSolid[i].solid->GetVelocity());
			}

			// ���B�^�����鍄�̂ɗ͂�������D
			movmentSolid[i].solid->AddForce(movmentSolid[i].pdForce);

			// ��̐߂ɗ͂�������D
			model->solids[0]->AddForce(-movmentSolid[i].pdForce);

			// ���݂̎p����ڕW�ɂ���
			//StayCurrentPos();

		}
	}
}

void CRReachingMovmentPD::MinimumJerkModel(SGScene* scene){

	for(int i = 0; i < movmentSolid.size(); i++){
		if(!movmentSolid[i].bGoal){

			// ���B�^�����n�߂Ă���̌o�ߎ���
			movmentSolid[i].nowTime = scene->GetTimeStep()*(scene->GetCount() - movmentSolid[i].startStep);

			double s = movmentSolid[i].nowTime/movmentSolid[i].goalTime;

			// ���x�ŏ����f���ɂ��c��̋������v�Z
			movmentSolid[i].jerkPos = movmentSolid[i].firstDistance + (movmentSolid[i].firstDistance - 0)
																	*(15*pow(s,4.0) - 6*pow(s,5.0) - 10*pow(s,3.0));

			// ���x�ŏ����f���ɂ�葬�x�v�Z
			movmentSolid[i].jerkVelocity = (movmentSolid[i].startPos - movmentSolid[i].goalPos)*(60*(pow(movmentSolid[i].nowTime,3)/pow(movmentSolid[i].goalTime,4.0))
							- 30*(pow(movmentSolid[i].nowTime,4)/pow(movmentSolid[i].goalTime,5.0)) - 30*(pow(movmentSolid[i].nowTime,2.0)/pow(movmentSolid[i].goalTime,3.0)));
	
		}
	}
}

void CRReachingMovmentPD::Draw(GRRender* render){
	
	if(pdType){
		for(int i = 0; i < movmentSolid.size(); i++){
			if(!movmentSolid[i].bGoal){
				render->SetModelMatrix(Affinef());
				render->SetMaterial(GRMaterialData(Spr::Vec4f(0.5,1.0,0.5,1)));
				render->SetLineWidth(10);
				if(movmentSolid[i].bStyle){
					double s = movmentSolid[i].nowTime/movmentSolid[i].goalTime;
					Vec3f v= movmentSolid[i].startPos + (movmentSolid[i].startPos - movmentSolid[i].goalPos)*(15*pow(s,4.0) - 6*pow(s,5.0) - 10*pow(s,3.0));
					render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
				}
				render->SetMaterial(GRMaterialData(Spr::Vec4f(1.0,0.5,0.5,1)));
				Vec3f y = movmentSolid[i].goalPos;
				render->DrawDirect(Spr::GRRender::POINTS, &y , &y+1);
			}
		}
	}
}


void CRReachingMovmentPD::RegistMovmentSolid(SGScene* scene,PHSolid* solid
											 ,double k,double b,double finishTime,std::vector<int>& joints){
	
	CRMovmentSolid moveSolid;

	// �ݒ�
	// ���B�^������Solid�Ɠ��B����
	moveSolid.solid = solid;
	moveSolid.goalTime = finishTime;

	// ���B�^����PD�̃o�l�E�_���p
	double dt = scene->GetTimeStep();
	float mass = solid->GetMass();
	moveSolid.spring = k * mass * 2 / (dt*dt);
	moveSolid.damp = b * mass / (dt);

	// �g�p����֐߂Ƃ��̃o�l�E�_���p�̋L��
	moveSolid.useJoints.clear();
	moveSolid.useJoints = joints;
	moveSolid.useJointsSpring.clear();
	for(int i = 0; i < moveSolid.useJoints.size(); i++){
		Vec3f p_d_i;
		p_d_i[0] = model->jointPids[moveSolid.useJoints[i]]->proportional;
		p_d_i[1] = model->jointPids[moveSolid.useJoints[i]]->differential;
		p_d_i[2] = model->jointPids[moveSolid.useJoints[i]]->integral;
		moveSolid.useJointsSpring.push_back(p_d_i);
	}

	// ������
	Init(&moveSolid);

	// �o�^
	movmentSolid.push_back(moveSolid);
}
void CRReachingMovmentPD::Init(CRMovmentSolid* moveSolid){
	moveSolid->bStart = false;
	moveSolid->bGoal = true;
	moveSolid->bStyle = true;
	moveSolid->nowTime = 0.0;
	moveSolid->startStep = 0;
	moveSolid->goalPos = Vec3d();
	moveSolid->startPos = Vec3d();
	moveSolid->pdForce = Vec3d();
	moveSolid->firstDistance = 0.0;
	moveSolid->jerkPos = 0.0;
	moveSolid->jerkVelocity = Vec3d();
	
}
void CRReachingMovmentPD::StayHere(CRMovmentSolid* moveSolid,int i){
	for(int j = 0; j < moveSolid->useJoints.size(); j++){
		model->jointPids[moveSolid->useJoints[j]]->goal = model->joints[moveSolid->useJoints[j]]->GetJointPosition(0);
	}
}
void CRReachingMovmentPD::StayCurrentPos(){
	for(int i = 0; i < movmentSolid.size(); i++){
		if(!movmentSolid[i].bGoal){
		//if(!(movmentSolid[i].bStyle)){
			for(int j = 0; j < movmentSolid[i].useJoints.size(); j++){
				model->jointPids[movmentSolid[i].useJoints[j]]->goal = model->joints[movmentSolid[i].useJoints[j]]->GetJointPosition(0);
			}
		}
	}
}
void CRReachingMovmentPD::DataOut(){
	static bool bFirst = true;
	std::ofstream of;
	if (bFirst){
		of.open("time.xls");
		bFirst = false;
	}else{
		of.open("pos.xls",std::ios::app);
	}
	for(int i = 0; i < movmentSolid.size(); i++){
		if(!movmentSolid[i].bGoal){
			Vec3f pos = movmentSolid[i].solid->GetCenterPosition();
			of << movmentSolid[i].nowTime <<'\t' << pos.x <<'\t' << pos.y <<'\t' << pos.z << std::endl;
		}
	}

	of.close();
}


}		// end namespace