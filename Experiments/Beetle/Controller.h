#pragma once

#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>

//カブトムシコントローラ
//メモ：足のヤコビアン
/*
	(股関節Yaw, 股関節Pitch, 膝関節) = (q1, q2, q3)
	(腿長, 脛長) = (l1, l2)
	J = {
		-c1(l1c2 + l2c(2-3))	 s1(l1s2 + l2s(2-3))	-l2s1s(2-3)
		-s1(l1c2 + l2c(2-3))	-c1(l1s2 + l2s(2-3))	 l2c1s(2-3)
		0							l1c2 + l2c(2-3)		-l2c(2-3)
	}
	det(J) = -l1l2(l1c2 + l2c(2-3))s3
	ci := cos(qi), si := sin(qi)
*/
enum {LEFT, RIGHT};
enum {FRONT, MIDDLE, BACK};
enum {YAW, PITCH, KNEE, HAND};
enum BeetleState{STOP, NEUTRAL, FORWARD, BACKWARD, LEFTTURN, RIGHTTURN};
	
class Beetle
{
public:
	class Leg
	{
		Spr::Vec3f	Error, ErrorInt, ErrorDiff;
	public:
		//設計パラメータ
		float		fLength[2];			//腿と脛の長さ
		float		fTilt;				//チルト角
		Spr::Vec3f	v3BasePosition;		//ベース位置（ボディフレーム）
		Spr::Vec3f	v3ToePosition;		//ニュートラル時のつま先位置（ボディフレーム）
		Spr::Vec3f	v3DesiredPosition;	//つま先目標位置
		float		fDirection;			//ボディ中心からつま先への方位角
		float		fDistance;			//ボディ中心からつま先への距離

		Beetle*		pBody;				//胴体への参照
		Spr::PHJoint* pJoint[4];		//関節
		
		void Reset();
		void SetPosition(const Spr::Vec3f& pos);
		void Step(double dt);
		Leg(){}
		~Leg(){}
	};

	bool	bReady;

	//設計パラメータ
	float	fPValue, fIValue, fDValue;	//PID係数
	float	fStride;					//歩幅
	float	fTurnAngle;					//旋回時の脚の振り角
	float	fClearance;					//脚持ち上げ量
	float	fSpeed;						//往路での足先の速度
	float	fSwingBackRate;				//復路速度/往路速度

	//首、尾、脚
	Spr::PHJoint *pjntNeck[2];
	Spr::PHJoint *pjntTail;
	Leg Legs[2][3];

	//歩行制御
	BeetleState	nState;
	int		nNeutralStep;
	float	fCounter;
	
	bool ConnectToSpringhead();
	void SetState(BeetleState);
	void Step(double dt);
	void CheckForce();
	Beetle(void);
	~Beetle(void);
};
