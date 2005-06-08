// CRPuppet.h: CRPuppet クラスのインターフェイス
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
	//////////////////// PositionSpringクラス ////////////////////
	class PositionSpring{
	public:
		PHSolid* solid;				// 制御対象のSolid
		Vec3f pos;					// 位置制御をするSolid 内の位置

		Vec3f targetPos, targetVel;	// 目標位置・速度
		Vec3f force;				// 加える力
		float sprRate, dmpRate;		// バネ・ダンパ係数の減少率
		bool bForce;				// 位置制御をするか

		PositionSpring():solid(NULL){bForce = false;}

		Vec3f GetPos();
		Vec3f GetVel();
		Vec3f GetForce();

		void SetSolid(PHSolid* s, Vec3f p, float spr, float dmp);
		void SetTarget(Vec3f pos, Vec3f vel, bool b);

		void AddSpringForce(float dt);
		void AddForce(Vec3f f);
	};

	//////////////////// PositionSpringsクラス ////////////////////
	class PositionSprings:public std::vector<PositionSpring>{
	public:
		void Draw(GRRender* render);
	};

	//////////////////// PostureSpringsクラス ////////////////////
	class PostureSpring {
	public:
		PHSolid* solid;				// 制御対象のSolid

		Quaterniond targetQuat;		// 目標姿勢
		Vec3f targetAngV;			// 目標角速度
		Vec3f torque;				// 加えるトルク
		float sprRate, dmpRate;		// バネ・ダンパ係数の減少率
		bool bTorque;				// 姿勢制御をするか

		PostureSpring():solid(NULL){bTorque = false;}

		Quaterniond GetQuaternion();
		Vec3f GetAngularVelocity();
		Vec3f GetTorque();

		void SetSolid(PHSolid* s, float spr, float dmp);
		void SetTarget(Quaterniond quat, Vec3f angVel, bool b);

		void AddSpringTorque(float dt);
		void AddTorque(Vec3f t);
	};
	//////////////////// ReachingMovementクラス ////////////////////
	class ReachingMovement : public PositionSpring{
	public:
		float time;					// 目標時間
		float offset;				// 達成後の待機時間

		Vec3f firstPos, finalPos, finalVel;	// 最終目標位置・速度
		PHSolid* targetSolid;		// 目標剛体
		Vec3f localPos;				// 目標剛体のローカル座標
		int state;					// 状態 (待機・攻撃・防御・回避)
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
	//////////////////// LocusGeneratorクラス ////////////////////
	class LocusGenerator{
	#define	LogNum 10
	#define	Deg 1
	public:
		Vec3f logPos[4][LogNum];	// 過去位置の記録
		Vec3f ak[4][Deg+1];			// 軌跡関数の係数
		bool  bPrediction[4];		// 予測に軌跡関数を使うかどうか
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
	//////////////////// HumanContactInfoクラス ////////////////////
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
	//////////////////// CRPuppetクラス ////////////////////

	CRPuppet();
	
	void LoadDerivedModel(SGScene* scene);	// 派生モデル(User,VH)の設定
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
	ReachingMovement reaching[2][3];	// rm[0][i]: 通常の到達運動，rm[1][i]: 予測中に使用
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
