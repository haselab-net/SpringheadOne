// CRReachingMovment.h: CRReachingMovment �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_REACHING_MOVEMENT_H
#define CR_REACHING_MOVEMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRHuman.h"

#define JARK_JOINT 7
#define HAND_DOF 3

namespace Spr{;

class CRReachingMovment  
{
public:
	CRReachingMovment();
	virtual ~CRReachingMovment();

	//////////�@ �֐��@�@//////////
	void Step(SGScene* scene,CRHuman* crHuman);
	void Load();
	void Draw(GRRender* render);
	void DataOut(CRHuman* crHuman);

	void MinimumJerkModel(SGScene* scene,CRHuman* crHuman);			// ���x�ŏ����f��
	void MakeJacobian(CRHuman* crHuman,Vec3d targetPos);			// ���R�r�A���̌v�Z
	Vec3f MakeOneJacobian(PHJointBase* joint,Vec3f targetPos);		// ��̊֐߂̃��R�r�A�����v�Z
	Vec3f GetWorldPos(PHJointBase* joint);
	void MakeTorque(SGScene* scene,CRHuman* crHuman);
	void SetTarget(SGScene* scene,CRHuman* crHuman,Vec3f goal);
	void StartReachingMovment(){bStart = true;}
	

	//////////�@ �ϐ��@�@//////////
/*
	PTM::VMatrixRow<float> jacobian;		// ���R�r�A��
	PTM::VMatrixRow<float> jacobianInv;		// ���R�r�A���̋[���t�s��
	PTM::VVector<float> jointTorque;		// ���B�^���Ɏg�p����֐߂̃g���N
	PTM::VVector<float> jointAngle;			// ���B�^���Ɏg�p����֐߂̊p�x
	PTM::VVector<float> jointVelocity;		// ���B�^���Ɏg�p����֐߂̊p���x
*/
	PTM::TMatrixRow<HAND_DOF,JARK_JOINT,double> jacobian;		// ���R�r�A��
	PTM::TMatrixRow<JARK_JOINT,HAND_DOF,double> jacobianInv;		// ���R�r�A���̋[���t�s��
	PTM::TMatrixRow<JARK_JOINT,JARK_JOINT,double> weightMat;		// ���R�r�A���̋[���t�s��
	PTM::TVector<JARK_JOINT,double> jointTorque;					// ���B�^���Ɏg�p����֐߂̃g���N
	PTM::TVector<JARK_JOINT,double> jointAngle;					// ���B�^���Ɏg�p����֐߂̊p�x
	PTM::TVector<JARK_JOINT,double> jointVelocity;				// ���B�^���Ɏg�p����֐߂̊p���x
	Vec3d jerkPos;												// ���x�ŏ����f���ŏo�����̈ʒu
	Vec3d jerkVelocity;											// ���x�ŏ����f���ŏo�����̑��x

	std::vector< PTM::TVector<6,double> > jacobian6;				//���R�r�A�����i����]

	bool bStart;
	int startStep;
	double goalTime;
	Vec3d goalPos;
	Vec3d startPos;
	double nowTime;

};

}		// end namespace
#endif // !defined(CR_REACHING_MOVEMENT_H)
