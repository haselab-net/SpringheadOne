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
//35:joLAnkle1(X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
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

#include "CRSupportArea.h"

#define JOINT_NUM 39
#define SOLID_NUM 18

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


DEF_RECORD(XHuman,{
	GUID Guid(){ return WBGuid("48CE97A5-A3C0-446f-A2A0-25D42168A1E4"); }
	FLOAT totalHeight;
	FLOAT totalMass;
});

class CRHuman: public SGBehaviorEngine{
public:
	friend class CRHumanLoader;
	friend class CRHumanSaver;
	SGOBJECTDEF(CRHuman);
	PHSolid* test;
	CRSupportArea supportArea;						// �̑S�̂̈���̈�̌v�Z
	std::vector<CRSupportArea> supportObject;		// �̂��x������Solid�̐ڐG�_�������̈���v�Z

///////////////////////  �ϐ�   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	�W���C���g�G���W��
	std::vector<PHSolid*> solids;					//	VH�̊e���ʂ����[
	std::vector<PHJointBase*> joints;				//	VH�̊e�֐߂����[
	std::vector<PHJointPid*> jointPids;				//	VH�̊e�֐߂����[
	std::vector<unsigned int> supportSolidNum[2];	//�@VH���x�����镔�ʂ̔ԍ������[(��,�ܐ�)(0:�E��,1:�����j
	std::vector<unsigned int> ankleJointNum[2];		//
	std::vector<int> noUseJoint;					// �œK��(�o�����X)�Ŏg��Ȃ��֐ߔԍ�


	CRHumanJointInfo	jinfo[JOINT_NUM];			// VH��Joint���
	CRHumanSolidInfo	sinfo[SOLID_NUM];			// VH��Solid���

private:
	bool bLoaded;	//�@VH�����[�h(Connct)���ꂽ���ۂ�

	//VH�̃p�����[�^
	float totalMass;
	float totalHeight;


///////////////////////   �֐�   ////////////////////////////////

public:
	CRHuman();
	virtual ~CRHuman();
	int GetPriority(){return SGBP_CREATURE;}

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

	// ��{�C���^�[�t�F�[�X
	virtual void Load(SGScene* scene);				// ��{�ݒ�BFrameWork��Load�ŌĂ΂��B
	virtual void Loaded(SGScene* scene);			// ��{�ݒ�B
	virtual void LoadDerivedModel(SGScene* scene){}	//�h�����f��(User,VH)�̐ݒ�

	bool IsLoaded(){ return bLoaded; }				// ���[�h�̊m�F
	void SetLoaded(bool load){bLoaded = load;}		// �̑S�̂�ǂݍ��܂��e�X�g�������ꍇ
	bool HasFrame(SGFrame* f);

	// VH���R���g���[���\�ɂ���(Solid,Joint�̎擾)
	virtual bool Connect(UTRef<SGScene> scene);
	void ConnectSolid(const char* name, UTRef<SGScene> scene);
	void ConnectJoint(const char* name, UTRef<SGScene> scene);

	//VH�̊e��p�����[�^�̐ݒ�
	void SetModel(SGScene* scene);				// �ȉ��̐ݒ�֐����g���AVH���g����悤�ɂ���B
	void SetTotalMass(float mass);
	void SetTotalHeight(float height);
	void SetMass();

	void SetSolidInfo();
	void SetJointInfo();
	virtual void SetJointRange();
	void SetOneJointRange(PHJoint1D* j, float min, float max);

	virtual void SetInertia();
	void SetOneInertia(Spr::PHSolid* solid, Spr::Vec3f Axis);
	void JointPIDMul(PHJointPid* jo, float mulP, float mulD);
	virtual void SetJointSpring(float dt);	

	void AddJointPassivityResistance();
	
	// ���f���̃X�P�[�����Z�b�g
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	// ���f���̏����p��
	virtual void SetJointInitAngle();

	// VH��Solid,Joint���擾
	CRHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	float GetTotalMass(){return totalMass;}						// VH�̑S�̏d���擾
	Vec3f GetCOG();												// VH�̏d�S�̈ʒu�擾
	Vec3d GetCogVelocity();										// VH�̏d�S�̑��x�擾


	void CalcContactForce();
	virtual void RegistSupportParts(UTRef<SGScene> scene){}		// �o�����X�ɂ����đ̎x������Solid��o�^(�E���A�����Ȃ�)
	virtual void SetBodyNum(){}									// �o�����X�ȂǂɕK�v��Solid,Joint�̔ԍ��̓o�^
protected:
	void LoadX(const XHuman& xh);
	void SaveX(XHuman& xh) const;

};

}	//	end namespace Spr
#endif // !defined(CRHuman_H)
