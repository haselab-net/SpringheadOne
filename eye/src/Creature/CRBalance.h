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
#include <Base/Affine.h>
#include <vector>

#include "CRSupportArea.h"

namespace Spr{;

class CRHuman;

// �o�����X���Ƃ邽�߂̊�{�N���X
class CRBalance{
public:
	CRBalance();
	virtual ~CRBalance();

	PHJointEngine*		jointEngine;		// �֐߃G���W��
	PHGravityEngine*	gravityEngine;		// PHGravityEngine

//protected:
public:
	Vec3f cogPos;					// VH�̏d�S�̈ʒu
	Quaterniond targetPostureQ;		// �ڕW�̎p��
	Quaterniond nowPostureQ;		// ���݂̎p��
	Vec3f balanceTorque;				// ���݂̎p������ڕW�̎p���ɂ���̂ɕK�v�ȃg���N
	Vec3f balanceForce;
	Vec3f targetZmp;				// �ڕW�y�l�o
	Vec3f targetAccel;				// �d�S�̖ڕW�����x

	// �������������o�l,�_���p�W��
	Vec3f spring_noV;				// �o�����X�g���N�̃o�l�W��
//	float spring_no;				// �o�����X�g���N�̃o�l�W��
	float dampa_no;					// �o�����X�g���N�̃_���p�W��
	float springCog_no;				// �d�S������̈���ɂ���͂̃o�l�W��
	float dampaCog_no;				// �d�S������̈���ɂ���͂̃_���p�W��

public:
//---------�@��{�C���^�t�F�[�X�@---------//
	void Step(SGScene* scene,CRHuman* crHuman);
	void Load(SGScene* scene,CRHuman* crHuman);
	virtual void OnKeyDown(UINT &nChar,CRHuman* crHuman);
	virtual void Draw(GRRender* render,CRSupportArea* supportArea);
	virtual void Init(SGScene* scene,CRHuman* crHuman);


//-----------------�@�����@----------------//
	virtual void Measure(SGScene* scene,CRHuman* crHuman);		// �o�����X�ɕK�v�Ȋ�{�I�Ȃ��Ƃ��v��
	virtual void ModifyBody(SGScene* scene,CRHuman* crHuman);	// �̂Ƀg���N�������o�����X���Ƃ�
	virtual void SetBalanceParameter(CRHuman* crHuman);			// �o�����X���Ƃ邽�߂̖ڕW�̃p�����[�^��ݒ�

	virtual void CalcBalanceTorque(SGScene* scene,CRHuman* crHuman);	// �o�����X���Ƃ邽�߂ɏd�S�ɕK�v�ȃg���N�̌v�Z
	virtual void CalcBalanceForce(SGScene* scene,CRHuman* crHuman);		// �o�����X���Ƃ邽�߂ɏd�S�ɕK�v�ȗ͂̌v�Z
	void CalcTargetZMP(SGScene* scene,CRHuman* crHuman);		// �ڕWZMP�̌v�Z
	void ControlBody(CRHuman* crHuman);							// �o�����X����邽�߂̐���
	void DrawZMP(GRRender* render);								// ZMP�̕`��
	Vec3f GetTargetZMP(){return targetZmp;}						// �ڕWZMP�̎擾
	Vec3f GetBalanceForce(){ return balanceForce;}				// �d�S�ɂ�����ڕW�̗͂��擾
	Vec3f GetBalanceTorque(){ return balanceTorque;}				// �o�����X���Ƃ�̂ɕK�v�ȏd�S�ɂ�����g���N���擾
	Vec3f GetTargetAccel(){ return targetAccel;}				// �d�S�̖ڕW�̉����x���擾

	// ZMP������̈���ɂ��邩�Ď� 
	bool ZmpIncludeCheck(CRSupportArea& supportArea);
	bool PosIncludeTriangle(Vec3f t_a, Vec3f t_b, Vec3f t_c, Vec3f zmp);

};

}		//	end namespace Spr
#endif // !defined(CRBALANCE_H)
