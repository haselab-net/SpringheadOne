// CRBalance.h: CRBalance �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBALANCE_H
#define CRBALANCE_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Base/TQuaternion.h>
#include <Base/Affine.h>
#include <vector>

#include "CRSupportArea.h"
#include "CRBalanceQuadPR.h"

namespace Spr{;

class CRHuman;

class CRBalance{
public:
	CRBalance();
	virtual ~CRBalance();

	PHContactEngine* contactEngine;	// �ڐG�G���W��
	PHJointEngine* jointEngine;		// �֐߃G���W��
	CRSupportArea supportArea;		// ����̈�̌v�Z�N���X
	CRBalanceQuadPR balanceQuadPR;

private:
	bool bCalc;						// �v�Z�A���䂵�Ă悢���ۂ�
	bool bReload;					// �����[�h���邩�ۂ�
	bool bControl;					// �͂������邩�ۂ�
	Vec3f gravity;					// �d�͉����x

	Vec3f cogPos;					// VH�̏d�S�̈ʒu
	Quaterniond targetPostureQ;		// �ڕW�̎p��
	Quaterniond nowPostureQ;		// ���݂̎p��
	Vec3f targetTorque;				// ���݂̎p������ڕW�̎p���ɂ���̂ɕK�v�ȃg���N
	Vec3f targetForce;
	float dampa;					// �_���p�W��
	Vec3f spring;					// �o�l�W��
	float spring_no;				// �������������o�l�W��
	float dampa_no;					// �������������_���p�W��
		
	Vec3f targetZmp;				// �ڕW�y�l�o
	Vec3f targetAccel;				// �d�S�̖ڕW�����x

	
public:
//---------�@��{�I�ȃC���^�t�F�[�X�@---------//
	void Step(SGScene* scene,GRRender* render,CRHuman* crHuman);
	void Load(SGScene* scene,CRHuman* crHuman);
	void OnKeyDown(UINT &nChar,CRHuman* crHuman);
	void Draw(GRRender* render);

//-----------------�@�����@----------------//

	void CalcBodyTorque(SGScene* scene,CRHuman* crHuman);	// �ڕW�̎p���ɂ��邽�߂ɏd�S�ɕK�v�ȃg���N�̌v�Z
	void CalcTargetZMP(SGScene* scene,CRHuman* crHuman);	// �ڕWZMP�̌v�Z
	void ControlBody(CRHuman* crHuman);						// �o�����X����邽�߂̐���
	void AppControl(SGScene* scene,CRHuman* crHuman);		// �A�v���P�[�V�����̐���i�͂�������A�����[�h����j
	void DrawZMP(GRRender* render);							// ZMP�̕`��
	bool GetReloadCheck(){ return bReload;}					// �����[�h���邩�ۂ�
	bool GetControlCheck(){ return bControl;}				// �R���g���[���𑱂��邩�ۂ�
	Vec3f GetCog(){return cogPos;}							// �d�S�̈ʒu��Ԃ�
	Vec3f GetTargetZMP(){return targetZmp;}					// �ڕWZMP�̎擾
	Vec3f GetTargetForce(){ return targetForce;}
	Vec3f GetTargetTorque(){ return targetTorque;}

	// ZMP������̈���ɂ��邩�Ď� 
	bool ZmpIncludeCheck(CRSupportArea& supportArea);
	bool PosIncludeTriangle(Vec3f t_a, Vec3f t_b, Vec3f t_c, Vec3f zmp);



};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_H)
