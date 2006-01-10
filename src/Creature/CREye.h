// CREye.h: CREye �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CREYE_H
#define CREYE_H

#include <Creature/CRPuppet.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Base/Affine.h>
#include <vector>


namespace Spr{;

class CRPuppet;

// VH�̖ڂ̃N���X... ����A�ዅ�^������Ȃ�
class CREye{
public:
	CREye();
	virtual ~CREye();

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
	/// �����𓾂�
	Vec3f GetVisualAxis();
	/// �������[�h�̐ݒ�
	void SetAttentionMode();
	/// �����_���ዅ�̉��͈͂𒴂����ʒu�ɂ��邩�ǂ���
	bool IsOverRange();
	/// �x�N�g��goal���ዅ�̉��͈͂𒴂����ʒu�ɂ��邩�ǂ���
	bool IsOverRange(Vec3f goal);
	/// �Ώ�Solid��������ɂ��邩�ǂ���
	bool IsVisible(PHSolid* solid);
	float GetVisibility(PHSolid* solid);

	void ToggleEyeMode(){
		bEyeMode++;
		if (bEyeMode > 2) bEyeMode = 0;
	}

//-----------------�@�����@----------------//
	/*
	/// ���_�\���̐؂�ւ�
	void ToggleVHPointOfView(GRRender* render,HIMouse* mouse);
	/// VirtualHuman�̎��_�ɂ���
	void SetPointOfView(HIMouse* mouse);
	*/
	/// VirtualHuman�̖ڂ𐧌䂷��
	void ControlEyes();
	/// VirtualHuman�̖ڂ�ڕW�����ɂȂ�悤PD���䂷��
	void ControlEyePD(SGFrame* frEye, PHSolid* soEye, Vec3f aim);
	/// �ዅ�^���̃��f��
	void DeterminAttentionDir();

//-----------------�@�����o�ϐ��@----------------//
	// �֘A���W���[��
	SGScene* scene; // �ėp
	CRPuppet* crPuppet; // ���o�͗p

	// VH�̖�
	PHSolid* soREye;
	PHSolid* soLEye;
	PHSolid* soHead;
	SGFrame* frREye;
	SGFrame* frLEye;
	SGFrame* frHead;
	// �ڂ�PID�R���g���[��
	PHJointBallPid* jpREye;
	PHJointBallPid* jpLEye;

	/// �����_,	��������
	Vec3f attentionPoint;
	Vec3f attentionDirL;
	Vec3f attentionDirR;

	/// �O��̖ڕW�ʒu(���p)
	float last_t1_a, last_t2_a;

	/// �ϕ���
	float integrator_L, integrator_R;

	/// �������[�h���ۂ�
	bool bAttention;
	/// VH���_�\���̐؂�ւ�
	bool bVHPointOfView;
	/// Saccade��
	bool  bSaccade;
	DWORD saccadeStartTime;
	Vec3f saccadePos;

	// VH���_�\���O�̎��_�̕ۑ��p
	Affinef viewOrig;
	float frontOrig;
public:
	int bEyeMode;
};

}		//	end namespace Spr
#endif 
