// CRVisionMotionAnalysis.h: CRVisionMotionAnalysis �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRVISIONMOTIONANALYSIS_H
#define CRVISIONMOTIONANALYSIS_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Creature/CREye.h>
#include <Creature/CRAttention.h>
#include <Creature/CRActionPlanner.h>
#include <Creature/CRInternalModel.h>
#include <Base/Affine.h>
#include <HIS/HIMouse.h>
#include <vector>
#include <map>
#include <Creature/CRPuppet.h>


namespace Spr{;


// SceneGraph���瑊��̓����Ɋւ��鎋�o����ǂݎ��N���X
class CRVisionMotionAnalysis{
public:
	CRVisionMotionAnalysis();
	virtual ~CRVisionMotionAnalysis();

public:
//---------�@��{�C���^�t�F�[�X�@---------//
	void Step();
	void Load(SGScene* scene, CRPuppet* crPuppet, CREye* crEye, CRAttention* crAttention);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//---------�@�g���C���^�t�F�[�X�@---------//
	void SetInternalModel(CRInternalModel* crInternalModel){
		this->crInternalModel = crInternalModel;
	}

//-----------------�@�����@----------------//
	/// �ڗ��_�����߂�
	void ListupAttractivePoints();

//-----------------�@�����o�ϐ��@----------------//
	// �֌W�̂��郂�W���[��
	SGScene* scene; // �ėp
	CRPuppet* crPuppet; // soild���������ǂ����̔��ʗp
	CREye* crEye; // ���씻��p
	CRAttention* crAttention; // �{�g���A�b�v�̒��ӗp
	CRInternalModel* crInternalModel; // �����̂���̓������f���\�z�p

	// User��Solid
	PHSolid *soWaistU, *soAbdomenU, *soChestU, *soHeadU;
	PHSolid *soRUArmU, *soRFArmU,   *soRHandU;
	PHSolid *soLUArmU, *soLFArmU,   *soLHandU;
	std::vector<PHSolid*> userSolids;

	// ��
	PHSolid *soLEye, *soREye;

};

}		//	end namespace Spr
#endif // !defined(CRVISIONMOTIONANALYSIS_H)
