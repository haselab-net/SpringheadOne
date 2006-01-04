#include "Creature.h"
#pragma hdrstop
// CRVisionMotionAnalysis.cpp: CRVisionMotionAnalysis �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRVisionMotionAnalysis.h"
#include "CRPuppet.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRVisionMotionAnalysis::CRVisionMotionAnalysis(){
	// �R���X�g���N�^
	crInternalModel = NULL;
}

CRVisionMotionAnalysis::~CRVisionMotionAnalysis(){
	// �f�X�g���N�^
}

//------------------�@��{�I�ȃC���^�t�F�[�X�@------------------//

void CRVisionMotionAnalysis::Step(){
	ListupAttractivePoints();
}

void CRVisionMotionAnalysis::Load(SGScene* scene, CRPuppet* crPuppet, CREye* crEye, CRAttention* crAttention){
	this->scene = scene;
	this->crPuppet = crPuppet;
	this->crEye = crEye;
	this->crAttention = crAttention;

	if (crPuppet->IsLoaded()){
		userSolids.clear();
		// User��Solid��ǂݍ���
		///  ...����͂��₵���Iscene->solids���g���[ (Future Works)
		scene->FindObject(soWaistU,   "soWaistU"  ); userSolids.push_back(soWaistU);
		scene->FindObject(soAbdomenU, "soAbdomenU"); userSolids.push_back(soAbdomenU);
		scene->FindObject(soChestU,   "soChestU"  ); userSolids.push_back(soChestU);
		scene->FindObject(soHeadU,    "soHeadU"   ); userSolids.push_back(soHeadU);
		scene->FindObject(soRUArmU,   "soRUArmU"  ); userSolids.push_back(soRUArmU);
		scene->FindObject(soRFArmU,   "soRFArmU"  ); userSolids.push_back(soRFArmU);
		scene->FindObject(soRHandU,   "soRHandU"  ); userSolids.push_back(soRHandU);
		scene->FindObject(soLUArmU,   "soLUArmU"  ); userSolids.push_back(soLUArmU);
		scene->FindObject(soLFArmU,   "soLFArmU"  ); userSolids.push_back(soLFArmU);
		scene->FindObject(soLHandU,   "soLHandU"  ); userSolids.push_back(soLHandU);
		
		// ��
		scene->FindObject(soLEye,   "soLEye"  );
		scene->FindObject(soREye,   "soREye"  );
	}
}

void CRVisionMotionAnalysis::Init(){
	// �������c�Ȃɂ����ĂȂ�
}

void CRVisionMotionAnalysis::OnKeyDown(UINT &nChar){
	// �L�[���́c�Ȃɂ����Ȃ�
}

void CRVisionMotionAnalysis::Draw(GRRender* render){
	// �f�o�b�O�p�\��
	render->SetModelMatrix(Affinef());
	Vec3f v[50];
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
	render->SetLineWidth(10);

	int cnt=0;
	for(int i=0; i<userSolids.size(); i++){
		if (crEye->IsVisible(userSolids[i])){
			v[cnt] = userSolids[i]->GetCenterPosition();
			cnt++;
		}
	}
	render->DrawDirect(Spr::GRRender::POINTS,  v, v+cnt);
}

//-----------------�@�����@----------------//

void CRVisionMotionAnalysis::ListupAttractivePoints(){
	Vec3f visualAxis = crEye->GetVisualAxis();

	// ��̓f�t�H���g�ň��ʂ̒��ӂ������t����
	float headAttentionAmmount = 1.0f;
	crAttention->SetAttentionSolid(soHeadU, headAttentionAmmount);

	// �eSolid�̉^�������ӂ������t���鋭�����v�Z�AcrAttention�ɓn��
	for(int i=0; i<userSolids.size(); i++){
		// ����̓����ɂ��镨�̂Ɍ���
		if (crEye->IsVisible(userSolids[i])){
			Vec3f position = userSolids[i]->GetCenterPosition();
			Vec3f velocity = userSolids[i]->GetVelocity();
			Vec3f angVelocity = userSolids[i]->GetAngularVelocity();
			float r = 1.0f + (position - ((soLEye->GetCenterPosition() + soREye->GetCenterPosition())*0.5)).norm();
			
			float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() / r;
			float divAmmount = abs(PTM::dot(velocity,visualAxis)) / r;
			float rotAmmount = abs(PTM::dot(angVelocity,visualAxis));
			
			crAttention->SetAttentionSolid(userSolids[i], trnAmmount);
			crAttention->SetAttentionSolid(userSolids[i], divAmmount);
			crAttention->SetAttentionSolid(userSolids[i], rotAmmount);

			if (crInternalModel!=NULL){
				float certainty = crEye->GetVisibility(userSolids[i]);
				Affinef posture = userSolids[i]->GetFrame()->GetPosture();
				crInternalModel->UpdateCertainty(i, posture, certainty);
			}
		}
	}

}

}	// end of namespace Spr
