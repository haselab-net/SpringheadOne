// Model.h: Model �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//	[solids�̏���]
//(�㔼�g�A�̋�j
//0:Waist(��),1:Abdomen(��),2:Chest(��),3:Head(��),
//(�㔼�g�A�E�r�j
//4:soRUArm(��r),5:soRFArm,(�O�r)6:soRHand(��)
//(�㔼�g�A���r�j
//7:soLUArm(��r),8:soLFArm(�O�r),9:soLHand(��)
//(�����g�A�E�j
//10:soRThigh(���),11:soRLThigh(����),12:soRHeel(��),13:soRToe(�ܐ�)
//(�����g�A�E�j
//14:soLThigh(���),15:soLLThigh(����),16:soLHeel(��),17:soLToe(�ܐ�)
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[joints�̏���]
//-------	  �㔼�g   --------------
//	(���j
//0:joWaist1(-X), 1:joWaist2(Z), 2:joWaist3(-Y)
//	(���j
//3:joChest1(-X), 4:joChest2(Z), 5:joChest3(-Y)
//	(��j
//6:joNeck1(-X), 7:joNeck2(Z), 8:joNeck3(-Y)
//	(�E��)
//9:joRShoulder1(X), 10:joRShoulder2(Z), 11:joRShoulder3(-Y)
//	(�E�I)
//12:joRElbow1(X), 13:joRElbow2(-Y)
//	(�E���)
//14:joRWrist1(-Z), 15:joRWrist2(X)
//	(����)
//16:joLShoulder1(X), 17:joLShoulder2(-Z), 18:joLShoulder3(Y)
//	(���I)
//19:joLElbow1(X), 20:joLElbow2(Y)
//	(�����)
//21:joLWrist1(Z), 22:joLWrist2(X)
//-------	  �����g   --------------
//	(�E�Ҋ֐�)
//23:joRHip1(X), 24:joRHip2(Z), 25:joRHip3(-Y)
//	(�E�G)
//26:joRKnee(-X)
//	(�E����)
//27:joRAnkle1(X), 28:joRAnkle2(Z), 29:joRAnkle3(-Y)
//	(�E�y���܂�)
//30:joRToe(-X)
//	(���Ҋ֐�)
//31:joLHip1(X), 32:joLHip2(-Z), 33:joLHip3(Y)
//	(���G)
//34:joLKnee(-X)
//	(������)
//35:joLAnkle1(X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
//	(���y���܂�)
//38:joLToe(-X)
//////////////////////////////////////////////////////////////

#ifndef MODEL_H
#define MODEL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<Creature/CRHuman.h>
#include "Balance.h"

namespace Spr{;
class Model : public CRHuman  
{
public:
	Model();
	virtual ~Model();

	Balance balance;

	void MaintainBalance(SGScene* scene,GRRender* render);	// �o�����X���Ƃ�
	void RegistSupportParts(UTRef<SGScene> scene);
	void ExceptContactParts(CREnumContactVertex* contact);

	void Load(SGScene* scene);
	void LoadDerivedModel(SGScene* scene);
	void SetJointSpring(float dt);
	bool Connect(UTRef<SGScene> scene);
	void SetJointRange();
	void SetInertia();
	
};

}		// end namespace
#endif // !defined(MODEL_H)
