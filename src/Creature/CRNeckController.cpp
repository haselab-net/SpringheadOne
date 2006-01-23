#define DSTRCHK(var) DSTR << #var << " : " << (var) << GetTickCount() << std::endl;

#include "Creature.h"
#pragma hdrstop
// CRNeckController.cpp: CRNeckController �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRNeckController.h"
#include "CRPuppet.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRNeckController::CRNeckController(){
	// �R���X�g���N�^
}

CRNeckController::~CRNeckController(){
	// �f�X�g���N�^
}

//------------------�@��{�I�ȃC���^�t�F�[�X�@------------------//

void CRNeckController::Step(){
	if (bAttention){
		ControlNeck();
	}else{
		jpNeck->proportional = 30.0f;
		jpNeck->differential = 10.0f;
		jpNeck->integral     = 0.0f;
	}

	bAttention = false;
}

void CRNeckController::Load(SGScene* scene,CRPuppet* crPuppet){
	this->scene = scene;
	this->crPuppet = crPuppet;

	if (crPuppet->IsLoaded()){
		// VH�̎�Ƃ���
		//// Solid
		scene->FindObject(soHead, "soHead");
		//// Frame
		scene->FindObject(frHead, "frHead");
		scene->FindObject(frChest, "frChest");
		//// BallJoint
		scene->FindObject(joNeck, "joNeck");
		scene->FindObject(jpNeck, "jpNeck");

		// ��
		scene->FindObject(soLEye, "soLEye");
		scene->FindObject(soREye, "soREye");
	}

	Init();
}

void CRNeckController::Init(){
	// ������
	bAttention = false;
}

void CRNeckController::OnKeyDown(UINT &nChar){
	// �L�[����
}

void CRNeckController::Draw(GRRender* render){
	// �f�o�b�O�p�\��
	render->SetModelMatrix(Affinef());
	Vec3f v[2];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,0,1,1)));
	render->SetLineWidth(10);

	//// ���̐��ʕ����̕\��
	v[0] = frHead->GetPosture().Pos();
	v[1] = v[0] + frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -100.0f);
	render->DrawDirect(Spr::GRRender::LINES,  v, v+2);
}

//------------�@�g���C���^�t�F�[�X�@-----------//

void CRNeckController::SetAttentionPoint(Vec3f position){
	if ((position - attentionPoint).norm() > 0.1f) {
		startVisualAxis = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
		s=0.0f;
	}

	goalVisualAxis = position - frHead->GetPosture().Pos();
	goalVisualAxis = goalVisualAxis / goalVisualAxis.norm();
	attentionPoint = position;
}

void CRNeckController::SetAttentionMode(){
	bAttention = true;
}

float CRNeckController::GetErrAbs(){
	Vec3f goal = goalVisualAxis;
	Vec3f current = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
	return PTM::cross(goal,current).norm();
}

//-----------------�@�����@----------------//

void CRNeckController::ControlNeck(){
	jpNeck->proportional = 1.0f;  // 30.0f
	jpNeck->differential = 5.0f;
	jpNeck->integral     = 0.0f;
		
	double length = (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
	Vec3f dir = goalVisualAxis - startVisualAxis;
	Vec3f goal = startVisualAxis + dir*length;
		
	Vec3f current = frHead->GetPosture().Rot() * Vec3f(0.0f, 0.0f, -1.0f);
		
	// Yaw
	Vec3f goalY = goal; 	 goalY[1] = 0.0f;
	Vec3f currY = current; currY[1] = 0.0f;
	float errorY = PTM::cross(goalY, currY)[1];
		
	// Pitch
	Vec3f goalP = goal; 	 goalP[0] = 0.0f;
	Vec3f currP = current; currP[0] = 0.0f;
	float errorP = PTM::cross(goalP, currP)[0];
		
	float Kp = 5.0f;
		
	// DSTR << atan2(currY[0],currY[2]) << ":" << atan2(goalY[0],goalY[2]) << std::endl;
		
	// Vec3f(PanDown,  TiltRight,  PanRight);
	Vec3f torque = frHead->GetPosture().Rot() * (Vec3f(-errorP,0.0f,errorY) * Kp);
		
	joNeck->AddTorque(torque);

	s += 0.01f;
	if(s > 1.0f){s=1.0f;}

	headpos = atan2(currY[0],currY[2]);
}

}	// end of namespace Spr
