// CRHuman.h: CRHuman �N���X�̃C���^�[�t�F�C�X
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
//35:joLAnkle1(-X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
//	(���y���܂�)
//38:joLToe(-X)
//////////////////////////////////////////////////////////////

#ifndef CRHuman_H
#define CRHuman_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Physics/PHSolid.h>
#include <Physics/PHJointPid.h>
#include <Physics/PHContactEngine.h>

#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>

#include <Base/Affine.h>
#include <vector>

#include "CRBalance.h"

#define BODYPARTNUM 5

namespace Spr{;

struct CRHumanSolidInfo{
	Vec3f scaleRatio;	// �e���ʂ̐g���ɑ΂����/�Q
	Vec3f scale;		// ����
	float massRatio;	// �e���ʂ̑S�̏d�ɑ΂��鎿�ʔ�

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRHumanJointInfo{
	Vec3f parentPosScale;	// �e�e���̂���̊֐߈ʒu�̐g���ɑ΂����
	Vec3f childPosScale;	// �e�q���̂���̊֐߈ʒu�̐g���ɑ΂����
	float rangeMax;			// ����ő�l
	float rangeMin;			// ����ŏ��l
	float danpa;			// �_���p�W��
	float spring;			// �o�l�W��
	float torqueMax;		// �g���N�̍ő�l
	float torqueMin;		// �g���N�̍ŏ��l
	float axis;				// ���[���h���W�n�ɑ΂��Ă̎��̐��̕���
	float initPos;			// ��{�p��
};


class CRHuman: public SGBehaviorEngine{
public:
	friend class CRHumanLoader;
	friend class CRHumanSaver;
	SGOBJECTDEF(CRHuman);
	PHSolid* test;

	CRHuman();
	virtual ~CRHuman();
	CRBalance crBalance;

///////////////////////  Enum   /////////////////////////////////

	enum TBodyParts{	//�@�̂̕���
		BODY,
		RIGHT_ARM,
		LEFT_ARM,
		RIGHT_LEG,
		LEFT_LEG
	};

	enum TJointParts{	//�@�̂̕���
		JO_WAIST,
		JO_CHEST,
		JO_NECK,
		JO_R_SHOULDER,
		JO_R_ELBOW,
		JO_R_WRIST,
		JO_L_SHOULDER,
		JO_L_ELBOW,
		JO_L_WRIST
	};


///////////////////////  �ϐ�   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	�W���C���g�G���W��
	std::vector<PHSolid*> solids;					//	VH�̊e���ʂ����[
	std::vector<PHJoint1D*> joints;					//	VH�̊e�֐߂����[
	std::vector<PHJointPid*> jointPids;				//	VH�̊e�֐߂����[
	std::vector<PHSolid*> solidPart[BODYPARTNUM];	// VH���T�ɕ����Ď��[

	//std::vector<CRHumanSolidInfo*>	sinfo;		// VH��Solid���
	//std::vector<CRHumanJointInfo*>	jinfo;		// VH��Joint���
	CRHumanJointInfo	jinfo[39];					// VH��Joint���
	CRHumanSolidInfo	sinfo[18];					// VH��Solid���

	//Vec3f zmp;
	struct CDContact{
		CDContact(PHContactEngine::FramePairRecord* f, float s): fpr(f), sign(s){}
		PHContactEngine::FramePairRecord* fpr;
		float sign;
	};
	std::vector<CDContact> contactPairs;
	Vec3f contactForce;					//	�ڐG�͂̑��a
	Vec3f contactTorque;				//	�ڐG�g���N�̑��a

private:
	bool bLoaded;	//�@VH�����[�h(Connct)���ꂽ���ۂ�

	//VH�̃p�����[�^
	float totalMass;
	float totalHeight;


///////////////////////   �֐�   ////////////////////////////////

public:
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects(){ return jointEngine ? 0 : 1; }
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i){ return jointEngine; }
	///	�Q�Ƃ��Ă���I�u�W�F�N�g�̐�
	virtual size_t NReferenceObjects(){ return 0; }
	///	�Q�Ƃ��Ă���I�u�W�F�N�g
	virtual SGObject* ReferenceObject(size_t i){ return NULL; };
	///	�q�I�u�W�F�N�g�̒ǉ�(���L�E�Q�Ƃ���Ȃ�)
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s){ return false; }

	
	bool IsLoaded(){ return bLoaded; }			//���[�h�̊m�F
	void SetLoaded(bool load){bLoaded = load;}	//
	bool HasFrame(SGFrame* f);

	//VH���R���g���[���\�ɂ���(Solid,Joint�̎擾)
	bool Connect(UTRef<SGScene> scene);
	bool ConnectUser(UTRef<SGScene> scene);
	void ConnectSolid(int bodyNum, const char* name, UTRef<SGScene> scene);
	void ConnectJoint(const char* name, UTRef<SGScene> scene);

	//VH�̊e��p�����[�^�̐ݒ�
	void SetVH(SGScene* scene);				// �ȉ��̐ݒ�֐����g���AVH���g����悤�ɂ���B
	void SetTotalMass(float mass);
	void SetTotalHeight(float height);
	void SetMass();

	void SetSolidInfo();
	void SetJointInfo();
	void SetJointRange();
	void SetOneJointRange(PHJoint1D* j, float min, float max);

	void SetInertia();
	void SetOneInertia(Spr::PHSolid* solid, Spr::Vec3f Axis);
	void SetJointSpring(float dt);	

	void AddJointPassivityResistance();
	
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	void SetJointInitAngle();

	// VH��Solid,Joint���擾
	CRHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	void CalcContactForce();
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetContactPoint(float y=0);							// VH�����̕��̂Ɣ��͒��S�_���擾
	float GetTotalMass(){return totalMass;}						// VH�̑S�̏d���擾
	Vec3f GetCOG();												// VH�̏d�S���擾

};

}	//	end namespace Spr
#endif // !defined(CRHuman_H)
