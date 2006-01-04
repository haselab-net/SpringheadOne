// CRReachingMovmentPD.h: CRReachingMovmentPD �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_REACHING_MOVEMENT_PD_H
#define CR_REACHING_MOVEMENT_PD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRHuman.h"
#include <iostream>

namespace Spr{;


struct CRMovmentSolid{
	PHSolid* solid;

	double goalTime;		// ���B�^������
	double nowTime;			// ���B�^�����n�߂Ă���̌o�ߎ���
	int startStep;			// �J�n���̃V�~�����[�^�̃X�e�b�v��
	Vec3d goalPos;			// ���B�n�_
	Vec3d startPos;			// �X�^�[�g�n�_
	Vec3d pdForce;			// PD����ɂ���
	double spring,damp;		// PD����̃o�l�E�_���p�W��
	double firstDistance;	// �X�^�[�g�n�_���瓞�B�n�_�܂ł̋���

	bool bStart;			// ���B�^�����邩�ۂ�
	bool bGoal;				// ���B���Ă��邩�ۂ�
	bool bStyle;			// true:���x�ŏ��Cfalse:PD�̂�
	
	double jerkPos;			// ���B�n�_�܂ł̖ڕW�̎c�苗��(���x�ŏ��ɂ���ċ��߂�ꂽ)
	Vec3d jerkVelocity;		// �ڕW���x(���x�ŏ��ɂ���ċ��߂�ꂽ)

	std::vector<int> useJoints;	// ���B�^���Ŏg�p����֐�
	std::vector<Vec3f> useJointsSpring;	// ���B�^���Ŏg�p����֐߂̂��Ƃ̃o�l�ƃ_���p
};

class CRReachingMovmentPD  
{
public:
	CRReachingMovmentPD();
	virtual ~CRReachingMovmentPD();

	CRHuman*			model;

	//////////�@ �֐��@�@//////////
	void Step(SGScene* scene);		// ���B�^�����s
	void Load(CRHuman* crHuman);
	void Draw(GRRender* render);


	void MinimumJerkModel(SGScene* scene);			// ���x�ŏ����f���Ŏc��̋����C�ڕW���x���v�Z
	void PDControl(SGScene* scene);				// ���B�^���̂��߂�PD����̗�
	void StartReachingMovment(int i)
			{movmentSolid[i].bStart = true;}		// ���B�^�����X�^�[�g������
	void SetTarget(SGScene* scene,Vec3f goal);		// ���B�^���̃S�[����ݒ肷��
	void RegistMovmentSolid(SGScene* scene			// ���B�^�����鍄�̂̓o�^
		,PHSolid* solid,double k,double b,double finishTime,std::vector<int>& joints);
	void CheckReachingMiovment(SGScene* scene);		// ���B�^�����I�����邩�ۂ�
	bool CheckGoal(CRMovmentSolid* moveSolid,SGScene* scene);
	void Init(CRMovmentSolid* moveSolid);			// ������
	void StayCurrentPos();
	void DataOut();

	void StayHere(CRMovmentSolid* moveSolid,int i);
	
	//////////�@ �ϐ��@�@//////////
	std::vector<CRMovmentSolid> movmentSolid;		// ���B�^�����鍄�̂̐�
	bool pdType;									// true:���x�ŏ�+PD�Cfalse:PD
	bool bMove;
	bool offSpring;

};

}		// end namespace
#endif // !defined(CR_REACHING_MOVEMENT_PD_H)
