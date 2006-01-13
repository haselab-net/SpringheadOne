// CRNeckController.h: CRNeckController �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRNECKCONTROLLER_H
#define CRNECKCONTROLLER_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Base/Affine.h>
#include <HIS/HIMouse.h>
#include <vector>


namespace Spr{;

class CRPuppet;

// VH�̎�̃N���X... �����^������
class CRNeckController{
public:
	CRNeckController();
	virtual ~CRNeckController();

public:

//---------�@��{�C���^�t�F�[�X�@---------//
	void Step();
	void Load(SGScene* scene,CRPuppet* crPuppet);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------�@�g���C���^�t�F�[�X�@-----------//
	/// �����_�̐ݒ�
	void SetAttentionPoint(Vec3f position);
	/// �������[�h�̐ݒ�
	void SetAttentionMode();
	/// �ڕW�l�܂ł̌덷
	float GetErrAbs();

//-----------------�@�����@----------------//
	/// VirtualHuman�̖ڂ𐧌䂷��
	void ControlNeck();

//-----------------�@�����o�ϐ��@----------------//
	// �֘A���W���[��
	SGScene* scene; // �ėp
	CRPuppet* crPuppet; // ���o�͗p?

	// ��
	PHSolid* soHead;
	SGFrame* frHead;
	// ��
	PHJointBall* joNeck;
	PHJointBallPid* jpNeck;
	// ��
	SGFrame* frChest;
	// ��
	PHSolid *soLEye, *soREye;

	/// �ڕW��������
	Vec3f goalVisualAxis;
	Vec3f attentionPoint;
	
	/// �������[�h���ۂ�
	bool bAttention;

};

}		//	end namespace Spr
#endif 
