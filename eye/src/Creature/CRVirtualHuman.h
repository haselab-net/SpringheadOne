// CRVirtualHuman.h: CRVirtualHuman �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRVIRTUALHUMAN_H
#define CRVIRTUALHUMAN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CRHuman.h"
#include "CRPuppet.h"
#include "CRBalanceQuadPR.h"
//#include "CRBalanceMultiLeg.h"
#include "CRReachingMovment.h"
#include "CRReachingMovmentPD.h"

#define ANKLE_NUM 2

namespace Spr{;

class CRVirtualHuman : public CRHuman  
{
public:
	SGOBJECTDEF(CRVirtualHuman);
///////////////     �ϐ�     ///////////////
	CRPuppet::PostureSpring  stayPost[2];
	CRBalanceQuadPR balanceQDPR;		// �o�����X���Ƃ�N���X
	//CRBalanceMultiLeg balanceQDPR;		// �o�����X���Ƃ�N���X
	CRReachingMovment rMovment;			// ���B�^���N���X
	CRReachingMovmentPD rMovmentPD;
	
///////////////     �֐�     ///////////////
	CRVirtualHuman();
	virtual ~CRVirtualHuman();

	void Draw(GRRender* render);					// �`��
	void OnKeyDown(unsigned int nChar,SGScene* scene);
		
	// �o�����X�֌W
	void MaintainBalance(SGScene* scene);			// �o�����X���Ƃ�
	virtual void RegistNoUSeJoint();				// �œK��(�o�����X)�Ŏg��Ȃ��֐߂�o�^

	// ���B�^��
	void ReachingMovment(SGScene* scene);

	// virtual(Load�ŌĂ΂����́j
	void LoadDerivedModel(SGScene* scene);						// ���f���̃��[�h&�Z�b�g
	virtual void SetJointSpring(float dt);						// �̂̍d����ݒ�
	virtual void SetJointInitAngle();									// �����p��
	float GetChildMass(PHJointBase* j);							// ����Joint�̎q��Solid�̏d�����擾
	virtual void SetBodyNum();									// �o�����X�ȂǂɕK�v��Solid,Joint�̔ԍ��̓o�^

	// TEST
	void OffSpring();

protected:
	// ����̈�֌W
	virtual void RegistSupportParts(UTRef<SGScene> scene);				// ����̈�v�Z�ŕK�v�Ȃ��̂�o�^
	void ExceptContactParts(CRContactDetector* contact);		// ����̈�̐ڐG�_�ɍl���Ȃ�Solid�o�^

};

}		// end namespace
#endif // !defined(CRVIRTUALHUMAN_H)
