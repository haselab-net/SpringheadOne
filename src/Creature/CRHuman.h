// CRHuman.h: CRHuman クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[solidsの順番]
//(上半身、体躯）
//0:Waist(腰),1:Abdomen(腹),2:Chest(胸),3:Head(頭),
//(上半身、右腕）
//4:soRUArm(上腕),5:soRFArm,(前腕)6:soRHand(手)
//(上半身、左腕）
//7:soLUArm(上腕),8:soLFArm(前腕),9:soLHand(手)
//(下半身、右）
//10:soRThigh(大腿),11:soRLThigh(下腿),12:soRHeel(踵),13:soRToe(爪先)
//(下半身、右）
//14:soLThigh(大腿),15:soLLThigh(下腿),16:soLHeel(踵),17:soLToe(爪先)
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[jointsの順番]
//-------	  上半身   --------------
//	(腰）
//0:joWaist1(-X), 1:joWaist2(Z), 2:joWaist3(-Y)
//	(胸）
//3:joChest1(-X), 4:joChest2(Z), 5:joChest3(-Y)
//	(首）
//6:joNeck1(-X), 7:joNeck2(Z), 8:joNeck3(-Y)
//	(右肩)
//9:joRShoulder1(X), 10:joRShoulder2(Z), 11:joRShoulder3(-Y)
//	(右肘)
//12:joRElbow1(X), 13:joRElbow2(-Y)
//	(右手首)
//14:joRWrist1(-Z), 15:joRWrist2(X)
//	(左肩)
//16:joLShoulder1(X), 17:joLShoulder2(-Z), 18:joLShoulder3(Y)
//	(左肘)
//19:joLElbow1(X), 20:joLElbow2(Y)
//	(左手首)
//21:joLWrist1(Z), 22:joLWrist2(X)
//-------	  下半身   --------------
//	(右股関節)
//23:joRHip1(X), 24:joRHip2(Z), 25:joRHip3(-Y)
//	(右膝)
//26:joRKnee(-X)
//	(右足首)
//27:joRAnkle1(X), 28:joRAnkle2(Z), 29:joRAnkle3(-Y)
//	(右土踏まず)
//30:joRToe(-X)
//	(左股関節)
//31:joLHip1(X), 32:joLHip2(-Z), 33:joLHip3(Y)
//	(左膝)
//34:joLKnee(-X)
//	(左足首)
//35:joLAnkle1(-X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
//	(左土踏まず)
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
	Vec3f scaleRatio;	// 各部位の身長に対する比/２
	Vec3f scale;		// 実寸
	float massRatio;	// 各部位の全体重に対する質量比

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRHumanJointInfo{
	Vec3f parentPosScale;	// 各親剛体からの関節位置の身長に対する比
	Vec3f childPosScale;	// 各子剛体からの関節位置の身長に対する比
	float rangeMax;			// 可動域最大値
	float rangeMin;			// 可動域最小値
	float danpa;			// ダンパ係数
	float spring;			// バネ係数
	float torqueMax;		// トルクの最大値
	float torqueMin;		// トルクの最小値
	float axis;				// ワールド座標系に対しての軸の正の方向
	float initPos;			// 基本姿勢
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

	enum TBodyParts{	//　体の部分
		BODY,
		RIGHT_ARM,
		LEFT_ARM,
		RIGHT_LEG,
		LEFT_LEG
	};

	enum TJointParts{	//　体の部分
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


///////////////////////  変数   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	ジョイントエンジン
	std::vector<PHSolid*> solids;					//	VHの各部位を収納
	std::vector<PHJoint1D*> joints;					//	VHの各関節を収納
	std::vector<PHJointPid*> jointPids;				//	VHの各関節を収納
	std::vector<PHSolid*> solidPart[BODYPARTNUM];	// VHを５つに分けて収納

	//std::vector<CRHumanSolidInfo*>	sinfo;		// VHのSolid情報
	//std::vector<CRHumanJointInfo*>	jinfo;		// VHのJoint情報
	CRHumanJointInfo	jinfo[39];					// VHのJoint情報
	CRHumanSolidInfo	sinfo[18];					// VHのSolid情報

	//Vec3f zmp;
	struct CDContact{
		CDContact(PHContactEngine::FramePairRecord* f, float s): fpr(f), sign(s){}
		PHContactEngine::FramePairRecord* fpr;
		float sign;
	};
	std::vector<CDContact> contactPairs;
	Vec3f contactForce;					//	接触力の総和
	Vec3f contactTorque;				//	接触トルクの総和

private:
	bool bLoaded;	//　VHがロード(Connct)されたか否か

	//VHのパラメータ
	float totalMass;
	float totalHeight;


///////////////////////   関数   ////////////////////////////////

public:
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects(){ return jointEngine ? 0 : 1; }
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i){ return jointEngine; }
	///	参照しているオブジェクトの数
	virtual size_t NReferenceObjects(){ return 0; }
	///	参照しているオブジェクト
	virtual SGObject* ReferenceObject(size_t i){ return NULL; };
	///	子オブジェクトの追加(所有・参照を問わない)
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s){ return false; }

	
	bool IsLoaded(){ return bLoaded; }			//ロードの確認
	void SetLoaded(bool load){bLoaded = load;}	//
	bool HasFrame(SGFrame* f);

	//VHをコントロール可能にする(Solid,Jointの取得)
	bool Connect(UTRef<SGScene> scene);
	bool ConnectUser(UTRef<SGScene> scene);
	void ConnectSolid(int bodyNum, const char* name, UTRef<SGScene> scene);
	void ConnectJoint(const char* name, UTRef<SGScene> scene);

	//VHの各種パラメータの設定
	void SetVH(SGScene* scene);				// 以下の設定関数を使い、VHを使えるようにする。
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

	// VHのSolid,Joint情報取得
	CRHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	void CalcContactForce();
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetContactPoint(float y=0);							// VHが他の物体と反力中心点を取得
	float GetTotalMass(){return totalMass;}						// VHの全体重を取得
	Vec3f GetCOG();												// VHの重心を取得

};

}	//	end namespace Spr
#endif // !defined(CRHuman_H)
