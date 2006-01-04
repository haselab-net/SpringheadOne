// CRInternalModel.h: CRInternalModel �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRINTERNALMODEL_H
#define CRINTERNALMODEL_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Base/Affine.h>
#include <Framework/FWFileIOD3D.h>
#include <Creature/CRUser.h>
#include <Creature/CRPuppet.h>
#include <vector>


namespace Spr{;

class CRPuppet;
class CRBaseBrain;
class CRAttention;

// VH�̎��A���Ǝ����Ɋւ���������f��
class CRInternalModel{
public:
	CRInternalModel();
	virtual ~CRInternalModel();

public:
//---------�@��{�C���^�t�F�[�X�@---------//
	void Step();
	void Load(std::string fn, GRRender* render, CRPuppet* realPuppet);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------�@�g���C���^�t�F�[�X�@-----------//
	SGScene*  GetScene(){  return internalModel;  }
	CRUser*   GetUser(){   return &internalUser;   }
	CRPuppet* GetPuppet(){ return &internalPuppet; }
	void SetPosition(int i, Vec3f pos); // Obsolete�\��
	void EnableVisualUpdate();
	void DisableVisualUpdate();
	void UpdateCertainty(int i, Affinef posture, float certainty);
	void AttentionFromCertainty(CRAttention* crAttention);
	bool IsStable(){ return bStability;}
	bool IsPredicting(){ return !bVisualUpdate; }

//-----------------�@�����@----------------//
	bool ApplyVision();
	bool ApplyRealPuppet();
	void DecayCertainty();

//-----------------�@�����o�ϐ��@----------------//
	/// �p�����[�^�萔(Init�ŏ�����) �ق�Ƃ̓t�@�C������ǂ݂���
	float Kp;
	float Kd;
	float Ktp;
	float Ktd;

	/// �������f����ێ�����Scene Graph
	UTRef<SGScene> internalModel;
	SGBehaviorStates stateTemp;
	/// �t�@�C�����[�_
	FWFileIOD3D fileIO;
	/// ���[�U�̓������f��
	CRUser internalUser;
	/// VH�̓������f��
	CRPuppet internalPuppet;
	/// �{����VH
	CRPuppet* realPuppet;
	/// VH�̍s���̓������f��
	CRBaseBrain* iPuppetBrain;
	/// User��Solid�̈ʒu�̊m�M�x�t���e�[�u��
	//// �v�f
	class SolidCertainty{
	public:
		SolidCertainty(Affinef p, float c): posture(p), certainty(c){}
		// Solid��Posture
		Affinef posture;
		// Solid��posture�ɂ���Ƃ����m�M�x(= pos�̑z��덷)
		float certainty;
	};
	//// ���X�g
	std::vector<SolidCertainty> certaintyTable;

	/// ���o���ōX�V���邩�ǂ���(=�V�~�����[�V���������ǂ���)
	bool bVisualUpdate;
	/// �������ʒu�ƃ��f���̈ʒu�̐����������������ǂ��� (���݂͎g�p���Ă��Ȃ��͂�)
	bool bStability;

	/// �e�X�g�p
	PHSolid *soRFArmU, *soRHandU, *soLFArmU, *soLHandU;
	PHSolid *soRFArm,  *soRHand,  *soLFArm,  *soLHand;

};

}		//	end namespace Spr
#endif 
