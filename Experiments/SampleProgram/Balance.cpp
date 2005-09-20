// Balance.cpp: Balance �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SampleProgram.h"
#include "Balance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TBalance::TBalance()
{
	bCalc = false;
	spring_no = 0.0005f;
	dampa_no = 0.0015f; 
}

TBalance::~TBalance()
{

}


Vec3f TBalance::CalcTargetZMP(SGScene* scene,PHHuman& human){

	// �e��p�����[�^�̐ݒ�
	SetTarget(scene,human);

	Quaterniond quatd;
	//�@�o�l�W���̍��i��ƂȂ�solid�̎p����߂�)
	for(int i = 0; i < human.solids.size(); i++){
		if(human.solids[i] != NULL){
			quatd = targetPostureQ * human.solids[i]->GetOrientation().inv();
			break;
		}
	}
	//�@�_���p�W���̍�(�S�p�^���ʂ�0�ɂ���)
	Vec3f L;
	for(int j = 0; j < human.solids.size(); j++){
		if(human.solids[j] != NULL){
			L += (human.solids[j]->GetCenterPosition() - cogPos) % (human.solids[j]->GetMass()*human.solids[j]->GetVelocity());
		}
	}

	torque.x = spring_all.x*quatd.rotation().x - dampa*L.x;
	torque.y = spring_all.y*quatd.rotation().y - dampa*L.y;
	torque.z = spring_all.z*quatd.rotation().z - dampa*L.z;
			
	//�@�����a
	Vec3f force = totalMass*(targetAccel-gravity);
	targetZmp = (force % torque) / force.square();
	//targetZmp += primaryPos;
	targetZmp += cogPos;
	targetZmp.Y() = 0;

	return targetZmp;

}

void TBalance::SetTarget(SGScene* scene,PHHuman& human){

	//�̑S�̂̏d�S�̈ʒu
	cogPos = GetCOG(human);
	
	//�ڕW
	targetVelocity = Vec3f(0.0,0.0,0.0);	//�ڕW���x
	targetPostureQ = Quaterniond(1.0,0.0,0.0,0.0);

	//�@�����`
	totalMass = human.GetTotalMass();
	Matrix3d cog_all_inartia = Matrix3d::Zero();		//�@�d�S�܂��̑S�g�̊����e���\��
	for(int i = 0; i < human.solids.size(); i++){
		if(human.solids[i] != NULL){
			Vec3d temp = human.solids[i]->GetCenterPosition() - cogPos;
			Matrix3d X = Matrix3d::Zero();
			X.xx = temp.y*temp.y + temp.z*temp.z;
			X.yx = -temp.x*temp.y;
			X.zx = -temp.x*temp.z;
			X.xy = -temp.x*temp.y;
			X.yy = temp.x*temp.x + temp.z*temp.z;
			X.zy = -temp.z*temp.y;
			X.xz = -temp.x*temp.z;
			X.yz = -temp.z*temp.y;
			X.zz = temp.x*temp.x + temp.y*temp.y;
			
			cog_all_inartia += human.solids[i]->GetRotation()*human.solids[i]->GetInertia()*human.solids[i]->GetRotation().trans()
									+ human.solids[i]->GetMass()*X;
		}
	}
	spring_all.x = (2*spring_no*cog_all_inartia.xx)/(scene->GetTimeStep()*scene->GetTimeStep());
	spring_all.y = (2*spring_no*cog_all_inartia.yy)/(scene->GetTimeStep()*scene->GetTimeStep());
	spring_all.z = (2*spring_no*cog_all_inartia.zz)/(scene->GetTimeStep()*scene->GetTimeStep());
	dampa = dampa_no/scene->GetTimeStep();

	if (!_finite(spring_all.x)){
		DSTR << spring_all << std::endl;
		DSTR << dampa << std::endl;
	}

	
	//spring = 200.0f;
	//dampa = 20.0f;

	//�@�����a
	gravity = Vec3f(0.0,-9.8,0.0);
	targetAccel = Vec3f(0.0,0.0,0.0);

	for(int i = 0; i < human.solids.size(); i++){
		if(human.solids[i] != NULL){
			primarrMass = human.solids[i]->GetMass();
			primaryPos = human.solids[i]->GetCenterPosition();
			break;
		}
	}


}

void TBalance::Draw(GRRender* render){
	render->SetModelMatrix(Affinef());
	render->SetMaterial(Mat(Spr::Vec4f(1,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = targetZmp;
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
}


void TBalance::Step(SGScene* scene,GRRender* render,PHHuman& human){

	if(human.solids.size() > 0)
		bCalc = true;
	
	if(bCalc){
		CalcTargetZMP(scene,human);
		Draw(render);
		Control(human);
	}

}
void TBalance::Load(SGScene* scene,PHContactEngine* contactEngine){

	if(scene){
		//PHContactEngine���擾����B
		balanceContactEngine = contactEngine;
		//scene->GetBehaviors().Find(contactEngine);
	}
}



void TBalance::Control(PHHuman& human){

	//�@�p���ɉ����ăg���N����������@
	if(human.joints.size()>0){

		//����̐���
		if(human.joints[27] != NULL){	//�E����
			Vec3f rightTorque = human.solids[11]->GetRotation()*torque;	//����̍��W�֕ϊ�
			human.joints[27]->SetTorque(-rightTorque.x);
			human.joints[28]->SetTorque(-rightTorque.z);
			human.joints[29]->SetTorque(-rightTorque.y);
		}
		if(human.joints[35] != NULL){	//������
			Vec3f leftTorque = human.solids[15]->GetRotation()*torque;
			human.joints[35]->SetTorque(-leftTorque.x);
			human.joints[36]->SetTorque(leftTorque.z);
			human.joints[37]->SetTorque(leftTorque.y);
		}

		//	�ܐ�̐���
		/*if(torque.x > 0){
			if(human.joints[30] != NULL){	//�E���ܐ�
				Vec3f rightToeTorque = human.solids[12]->GetRotation()*torque/5;
				human.joints[30]->SetTorque(rightToeTorque.x);
			}
			if(human.joints[38] != NULL){	//�����ܐ�
				Vec3f leftToeTorque = human.solids[16]->GetRotation()*torque/5;
				human.joints[38]->SetTorque(leftToeTorque.x);
			}
		}*/
		
	}

	// ZMP�̃Y���ɉ����ăg���N����������@
	/*
	if(balanceContactEngine){
		PHContactEngine::FramePairRecord* heel_floorRecord = balanceContactEngine->GetFramePairRecord(heel->GetFrame(),floor);
		PHContactEngine::FramePairRecord* toe_floorRecord = balanceContactEngine->GetFramePairRecord(toe->GetFrame(),floor);
		if(heel_floorRecord){
			//Vec3f realZmp = 
			DSTR <<"�R��"<<heel_floorRecord->GetReflexForce() << std::endl;
			DSTR <<"���C��"<<heel_floorRecord->GetFrictionForce() << std::endl;
			//torque = (targetZmp - realZmp)*reflexForce + (testJoint_X->GetInfo().v3fPositionParent)
		}
	}
	*/
}

void TBalance::OnKeyDown(UINT &nChar){
	if(nChar == 'E'){
		/*
		spring_no *= 2;
		DSTR << "Spring_no" <<spring_no << std::endl;
		dampa_no *= 2;
		DSTR << "dampa_no"<<dampa_no << std::endl;
		*/
	}
}

//VH�̏d�S���v�Z����B
Vec3f TBalance::GetCOG(PHHuman& human){
	Vec3f cog;
	float totalMass;
	totalMass = 0.0f;
	for(int i=0; i<human.solids.size(); ++i){
		if( human.solids[i] != NULL){
			float mass = human.solids[i]->GetMass();
			cog += mass * human.solids[i]->GetCenterPosition();
			totalMass += mass;
		}
	}
	return cog / totalMass;
}
