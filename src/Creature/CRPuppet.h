// CRPuppet.h: CRPuppet �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Creature/CRBallHuman.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Graphics/GRSound.h>
#include <Base/Affine.h>

#if !defined(AFX_CRPUPPET_H__FDCEF5A1_DD97_452F_A5F7_382CB8002AB4__INCLUDED_)
#define AFX_CRPUPPET_H__FDCEF5A1_DD97_452F_A5F7_382CB8002AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Spr{;

class CRPuppet : public CRBallHuman{
public:
	SGOBJECTDEF(CRPuppet);
	//////////////////// PositionSpring�N���X ////////////////////
	class PositionSpring{
	public:
		PHSolid* solid;				// ����Ώۂ�Solid
		Vec3f pos;					// �ʒu���������Solid ���̈ʒu

		Vec3f targetPos, targetVel;	// �ڕW�ʒu�E���x
		Vec3f force;				// �������
		float sprRate, dmpRate;		// �o�l�E�_���p�W���̌�����
		bool bForce;				// �ʒu��������邩

		PositionSpring():solid(NULL){bForce = false;}

		Vec3f GetPos();
		Vec3f GetVel();
		Vec3f GetForce();

		void SetSolid(PHSolid* s, Vec3f p, float spr, float dmp);
		void SetTarget(Vec3f pos, Vec3f vel, bool b);

		void AddSpringForce(float dt);
		void AddForce(Vec3f f);
	};

	//////////////////// PositionSprings�N���X ////////////////////
	class PositionSprings:public std::vector<PositionSpring>{
	public:
		void Draw(GRRender* render);
	};

	//////////////////// PostureSprings�N���X ////////////////////
	class PostureSpring {
	public:
		PHSolid* solid;				// ����Ώۂ�Solid

		Quaterniond targetQuat;		// �ڕW�p��
		Vec3f targetAngV;			// �ڕW�p���x
		Vec3f torque;				// ������g���N
		float sprRate, dmpRate;		// �o�l�E�_���p�W���̌�����
		bool bTorque;				// �p����������邩

		PostureSpring():solid(NULL){bTorque = false;}

		Quaterniond GetQuaternion();
		Vec3f GetAngularVelocity();
		Vec3f GetTorque();

		void SetSolid(PHSolid* s, float spr, float dmp);
		void SetTarget(Quaterniond quat, Vec3f angVel, bool b);

		void AddSpringTorque(float dt);
		void AddTorque(Vec3f t);
	};
	//////////////////// ReachingMovement�N���X ////////////////////
	class ReachingMovement : public PositionSpring{
	public:
		float time;					// �ڕW����
		float offset;				// �B����̑ҋ@����

		Vec3f firstPos, finalPos, finalVel;	// �ŏI�ڕW�ʒu�E���x
		PHSolid* targetSolid;		// �ڕW����
		Vec3f localPos;				// �ڕW���̂̃��[�J�����W
		int state;					// ��� (�ҋ@�E�U���E�h��E���)
		bool bActive;
		
		ReachingMovement();
		void Init();

		void SetSpring(PHSolid* s, Vec3f r);
		void SetTimer(float t, float o);
		void SetTargetPos(Vec3f p, Vec3f v);
		void SetTargetSolid(PHSolid* so, Vec3f p, Vec3f v);
		void SetType(int type);

		void Draw(GRRender* render);
		void Step(SGScene* scene);
	};
	//////////////////// LocusGenerator�N���X ////////////////////
	class LocusGenerator{
	#define	LogNum 10
	#define	Deg 1
	public:
		Vec3f logPos[4][LogNum];	// �ߋ��ʒu�̋L�^
		Vec3f ak[4][Deg+1];			// �O�Պ֐��̌W��
		bool  bPrediction[4];		// �\���ɋO�Պ֐����g�����ǂ���
		int degree;

		LocusGenerator(){
			degree = Deg;
		}
		void SaveLog(CRPuppet* puppet);
		void WeightedLeastSquares(float* y, int n, int m, PTM::VVector<float>* a);
		void CalFutureLocus(Vec3f* past, int n, int m, Vec3f* a);
		void DrawFutureLocus(Vec3f* a, int t, int k, GRRender* render, Vec4f color);
		Vec3f CalcFuturePosition(Vec3f* a, int t);
		Vec3f CalcFutureVelocity(Vec3f* a, int t, float dt);
		void SetLocusCoefficient(CRPuppet* puppet, SGScene* scene);

		void PrintLocusCoefficient(CRPuppet* puppet);
		void PrintLogPos(CRPuppet* puppet);
	};
	//////////////////// HumanContactInfo�N���X ////////////////////
	class HumanContactInfo{
	public:
		HumanContactInfo();
		
		Vec3f ContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
		void SetContactPointOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene, Vec3f* pos);
		bool ContactCheckOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
		bool ContactCheckOfSolid(PHSolid* so, CRBallHuman* human, SGScene* scene);

		Vec3f GetContactForceOfSolidPair(PHSolid* so1, PHSolid* so2, SGScene* scene);
		Vec3f GetContactForceOfSolid(PHSolid* so, CRBallHuman* human, SGScene* scene);
	};
	//////////////////// CRPuppet�N���X ////////////////////

	CRPuppet();
	
	void LoadDerivedModel(SGScene* scene);	// �h�����f��(User,VH)�̐ݒ�
	bool Connect(UTRef<SGScene> scene);
	void SetSolidInfo();
	void SetJointSpring(float dt);
	void SetJointBasicPos();
	void ChangeJointRange();

	void Draw(GRRender* render);

	virtual void SetSprings();
	virtual void CRPuppet::SetFixedPos();
	void SetExpectedPos(float dt);
	void SetSpringForce(float dt);

	void Step(SGScene* scene);

	void Attack(CRPuppet* puppet);
	void AttackTest(CRPuppet* puppet);
	bool IsAimed(CRPuppet* puppet, SGScene* scene);
	void GuardTest(CRPuppet* puppet, SGScene* scene);
	void ReachingMovemantsSafetyCheck(CRPuppet* puppet, SGScene* scene);
	void HittedCheck(CRPuppet* puppet, SGScene* scene);
	virtual void PlayHitSound();

	PositionSprings positionSprings;
	PostureSpring postureSpring;
	ReachingMovement reaching[2][3];	// rm[0][i]: �ʏ�̓��B�^���Crm[1][i]: �\�����Ɏg�p
	LocusGenerator locus;
	HumanContactInfo humanContactInfo;

	bool bDraw;
	bool bAttack;
	bool bGuard;
	bool bHitted;
	int atc;
	int hittingCount;
	
};

}

#endif // !defined(AFX_CRPUPPET_H__FDCEF5A1_DD97_452F_A5F7_382CB8002AB4__INCLUDED_)
