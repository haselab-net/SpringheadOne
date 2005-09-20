// Balance.h: Balance �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef BALANCE_H
#define BALANCE_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>

#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PHHuman.h>

#include <Base/TQuaternion.h>
#include <Base/Affine.h>

#include <vector>

using namespace Spr;

class TBalance  
{
public:
	TBalance();
	virtual ~TBalance();
	//�`��̂��߂̊֐�
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}

//////////////////////////////////////////////////////////////////
//						�֐�								 �@//
////////////////////////////////////////////////////////////////

//---------�@��{�I�ȃC���^�t�F�[�X�@---------//
	void Step(SGScene* scene,GRRender* render,PHHuman& human);
	void Load(SGScene* scene,PHContactEngine* contactEngine);
	void OnKeyDown(UINT &nChar);
	void Draw(GRRender* render);


//-----------------�@�����@----------------//

	Vec3f CalcTargetZMP(SGScene* scene,PHHuman& human);	//ZMP�ƃg���N�̌v�Z
	void SetTarget(SGScene* scene,PHHuman& human);		//�v�Z�ɕK�v�ȃp�����[�^�̐ݒ�
	void Control(PHHuman& human);						//�o�����X����邽�߂̐���
	Vec3f GetCOG(PHHuman& human);						//VH�̏d�S�̌v�Z


//////////////////////////////////////////////////////////////////
//						�ϐ�								 �@//
////////////////////////////////////////////////////////////////

	//PHHuman human;
	PHContactEngine* balanceContactEngine;

	bool bCalc;			//�v�Z�A���䂵�Ă悢���ۂ�

	float totalMass;				//������

	// �����`
	Quaterniond targetPostureQ;		//�ڕW�̎p��
	Quaterniond nowPostureQ;		//���݂̎p��
	Vec3f targetVelocity;			//�ڕW�̉����x
	Vec3f torque;					//���݂̎p������ڕW�̎p���ɂ���̂ɕK�v�ȃg���N
	float spring;					//�o�l�W��
	float dampa;					//�_���p�W��
	Vec3f spring_all;
	float spring_no;				//�������������o�l�W��
	float dampa_no;					//�������������_���p�W��
		
	// �����a
	Vec3f targetZmp;				//�ڕW�y�l�o
	Vec3f gravity;					//�d�͉����x
	Vec3f cogPos;					//�d�S�̈ʒu
	Vec3f primaryPos;				//�̂̊�ƂȂ�solid�̈ʒu�i���j
	float primarrMass;				//�̂̊�ƂȂ�solid�̎���
	Vec3f targetAccel;				//�̂̊�ƂȂ�solid�̖ڕW�����x
	
};

#endif // !defined(AFX_BALANCE_H__75359E4A_F1E4_4B0F_978F_DD5F3792DF35__INCLUDED_)
