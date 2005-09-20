#pragma once

#include <Physics/PHSolid.h>
#include <Physics/PHJointPid.h>
#include <Physics/PHContactEngine.h>

namespace Spr{;
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
enum CRBeetleState{STOP, NEUTRAL, FORWARD, BACKWARD, LEFTTURN, RIGHTTURN};

typedef float FLOAT;
typedef Vec3f VECTOR;
DEF_RECORD(XBeetle,{
	GUID Guid(){ return WBGuid("9EFE25A4-2BBF-4395-B6A7-2084207D9211"); }
	FLOAT	fPValue;
	FLOAT	fIValue;
	FLOAT	fDValue;					//PID係数
	FLOAT	fStride;					//歩幅
	FLOAT	fTurnAngle;					//旋回時の脚の振り角
	FLOAT	fClearance;					//脚持ち上げ量
	FLOAT	fSpeed;						//往路での足先の速度
	FLOAT	fSwingBackRate;				//復路速度/往路速度
	VECTOR	toe[3];						//	足先位置
	DWORD	bUseAi;						//AIを使うかどうか
});


class CRBeetle:public SGBehaviorEngine, public XBeetle{
SGOBJECTDEF(CRBeetle);
public:
	class Leg{
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

		CRBeetle*		pBody;				//胴体への参照
		Spr::PHJoint1D* pJoint[4];			//関節
		Spr::PHJointPid* pidJoint[4];		//関節
		
		void Reset();
		void SetPosition(const Spr::Vec3f& pos);
		void Step(double dt);
		Leg(){}
		~Leg(){}
	};

	bool	bReady;

	//首、尾、脚
	Spr::PHJoint1D *pjntNeck[2];
	Spr::PHJointPid *pidNeck[2];
	Spr::PHJoint1D *pjntTail;
	Spr::PHJointPid *pidTail;
	Leg Legs[2][3];
	UTRef<PHContactEngine> contactEngine;

	//歩行制御
	CRBeetleState	nState;
	int		nNeutralStep;
	float	fCounter;

	//	AI
	Spr::UTRef<Spr::PHSolid> beetle;
	std::vector< UTRef<Spr::PHSolid> > attractors;
	std::vector< UTRef<Spr::PHContactEngine::FramePairRecord> > fpRecords;
	
	///	ドキュメントからのロード時，参照が解決したあとで呼び出される．
	virtual void Loaded(SGScene* scene);
	void SetState(CRBeetleState);
	void Step(SGScene* s);
	void AiStep();
	void CheckForce();
	CRBeetle(void);
	~CRBeetle(void);
};

class CRBeetleLoader : public FIObjectLoader<CRBeetle>
{
public:
	CRBeetleLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(VECTOR);
		db->REG_RECORD_PROTO(XBeetle);
	}
	bool LoadData(FILoadScene* ctx, CRBeetle* b){
		ctx->docs.Top()->GetWholeData((XBeetle&)*b);
		return true;
	}
};

class CRBeetleSaver : public FIObjectSaver<CRBeetle>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRBeetle* b){
		doc->SetWholeData((XBeetle&)*b);
	}
};

DEF_REGISTER_BOTH(CRBeetle);


}	//	end namespace Spr