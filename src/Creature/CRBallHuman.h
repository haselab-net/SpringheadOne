// CRBallHuman.h: CRBallHuman �N���X�̃C���^�[�t�F�C�X
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
//0:joWaist (Ball)
//	(���j
//1:joChest (Ball)
//	(��j
//2:joNeck (Ball)
//	(�E��)
//3:joRShoulder (Ball)
//	(�E�I)
//4:joRElbow (Univ)
//	(�E���)
//5:joRWrist (Univ)
//	(����)
//6:joLShoulder (Ball)
//	(���I)
//7:joLElbow (Univ)
//	(�����)
//8:joLWrist (Univ)
//-------	  �����g   --------------
//	(�E�Ҋ֐�)
//9:joRHip (Ball)
//	(�E�G)
//10:joRKnee (Hinge)
//	(�E����)
//11:joRAnkle (Ball)
//	(�E�y���܂�)
//12:joRToe (Hinge)
//	(���Ҋ֐�)
//13:joLHip (Ball)
//	(���G)
//14:joLKnee (Hinge)
//	(������)
//15:joLAnkle (Ball)
//	(���y���܂�)
//16:joLToe (Hinge)
//////////////////////////////////////////////////////////////

#ifndef CRBallHuman_H
#define CRBallHuman_H

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

#define JOINTS_NUM 17
#define SOLID_NUM 18
#define JOINTHINGE_NUM 6
#define JOINTBALL_NUM 11

namespace Spr{;

struct CRBallHumanSolidInfo{
	Vec3f scaleRatio;	// �e���ʂ̐g���ɑ΂����/�Q
	Vec3f scale;		// ����
	float massRatio;	// �e���ʂ̑S�̏d�ɑ΂��鎿�ʔ�

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRBallHumanJointInfo{
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

struct CRBallHumanJointBallInfo{
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

DEF_RECORD(XBallHuman,{
	GUID Guid(){ return WBGuid("48CE97A5-A3C0-446f-A2A0-25D42168A1E4"); }
	FLOAT totalHeight;
	FLOAT totalMass;
});

class CRBallHuman: public SGBehaviorEngine{
public:
	friend class CRBallHumanLoader;
	friend class CRBallHumanSaver;
	SGOBJECTDEF(CRBallHuman);
	PHSolid* test;
	CRSupportArea supportArea;						// �̑S�̂̈���̈�̌v�Z
	std::vector<CRSupportArea> supportObject;		// �̂��x������Solid�̐ڐG�_�������̈���v�Z

///////////////////////  �ϐ�   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	�W���C���g�G���W��
	std::vector<PHSolid*> solids;					//	VH�̊e���ʂ����[
	std::vector<PHJointBase*> joints;				//	VH�̊e�֐߂����[
	std::vector<PHJointPid*> jointPids;				//	VH�̊e�֐߂����[
	std::vector<PHJointBallPid*> jointBallPids;		//  
	std::vector<unsigned int> supportSolidNum[2];	//�@VH���x�����镔�ʂ̔ԍ������[(��,�ܐ�)(0:�E��,1:�����j
	std::vector<unsigned int> ankleJointNum[2];		//
	std::vector<int> noUseJoint;					// �œK��(�o�����X)�Ŏg��Ȃ��֐ߔԍ�


	CRBallHumanJointInfo	jinfo[JOINTS_NUM];			// VH��Joint���
	CRBallHumanSolidInfo	sinfo[SOLID_NUM];			// VH��Solid���

protected:
	bool bLoaded;	//�@VH�����[�h(Connct)���ꂽ���ۂ�

	//VH�̃p�����[�^
	float totalMass;
	float totalHeight;


///////////////////////   �֐�   ////////////////////////////////

public:
	CRBallHuman();
	virtual ~CRBallHuman();
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

	virtual void SetSolidInfo();
	virtual void SetJointInfo();
	virtual void SetJointRange();
	void SetOneJointRange(PHJoint1D* j, float min, float max);

	virtual void SetInertia();
	void SetOneInertia(Spr::PHSolid* solid, Spr::Vec3f Axis);
	void JointPIDMul(PHJointPid* jo, float mulP, float mulD);
	void JointBallPIDMul(PHJointBallPid* jo, float mulP, float mulD);
	virtual void SetJointSpring(float dt);	

	void AddJointPassivityResistance();
	
	// ���f���̃X�P�[�����Z�b�g
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	// ���f���̏����p��
	virtual void SetJointInitAngle();

	// VH��Solid,Joint���擾
	CRBallHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRBallHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	float GetTotalMass(){return totalMass;}						// VH�̑S�̏d���擾
	Vec3f GetCOG();												// VH�̏d�S�̈ʒu�擾
	Vec3d GetCogVelocity();										// VH�̏d�S�̑��x�擾


	void CalcContactForce();
	virtual void RegistSupportParts(UTRef<SGScene> scene){}		// �o�����X�ɂ����đ̎x������Solid��o�^(�E���A�����Ȃ�)
	virtual void SetBodyNum(){}									// �o�����X�ȂǂɕK�v��Solid,Joint�̔ԍ��̓o�^
protected:
	void LoadX(const XBallHuman& xh);
	void SaveX(XBallHuman& xh) const;

};

}	//	end namespace Spr
#endif // !defined(CRBallHuman_H)
