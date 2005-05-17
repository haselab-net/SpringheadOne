// CRBallHuman.h: CRBallHuman クラスのインターフェイス
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
//0:joWaist (Ball)
//	(胸）
//1:joChest (Ball)
//	(首）
//2:joNeck (Ball)
//	(右肩)
//3:joRShoulder (Ball)
//	(右肘)
//4:joRElbow (Univ)
//	(右手首)
//5:joRWrist (Univ)
//	(左肩)
//6:joLShoulder (Ball)
//	(左肘)
//7:joLElbow (Univ)
//	(左手首)
//8:joLWrist (Univ)
//-------	  下半身   --------------
//	(右股関節)
//9:joRHip (Ball)
//	(右膝)
//10:joRKnee (Hinge)
//	(右足首)
//11:joRAnkle (Ball)
//	(右土踏まず)
//12:joRToe (Hinge)
//	(左股関節)
//13:joLHip (Ball)
//	(左膝)
//14:joLKnee (Hinge)
//	(左足首)
//15:joLAnkle (Ball)
//	(左土踏まず)
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
	Vec3f scaleRatio;	// 各部位の身長に対する比/２
	Vec3f scale;		// 実寸
	float massRatio;	// 各部位の全体重に対する質量比

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRBallHumanJointInfo{
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

struct CRBallHumanJointBallInfo{
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
	CRSupportArea supportArea;						// 体全体の安定領域の計算
	std::vector<CRSupportArea> supportObject;		// 体を支持するSolidの接触点から安定領域を計算

///////////////////////  変数   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	ジョイントエンジン
	std::vector<PHSolid*> solids;					//	VHの各部位を収納
	std::vector<PHJointBase*> joints;				//	VHの各関節を収納
	std::vector<PHJointPid*> jointPids;				//	VHの各関節を収納
	std::vector<PHJointBallPid*> jointBallPids;		//  
	std::vector<unsigned int> supportSolidNum[2];	//　VHを支持する部位の番号を収納(踵,爪先)(0:右足,1:左足）
	std::vector<unsigned int> ankleJointNum[2];		//
	std::vector<int> noUseJoint;					// 最適化(バランス)で使わない関節番号


	CRBallHumanJointInfo	jinfo[JOINTS_NUM];			// VHのJoint情報
	CRBallHumanSolidInfo	sinfo[SOLID_NUM];			// VHのSolid情報

protected:
	bool bLoaded;	//　VHがロード(Connct)されたか否か

	//VHのパラメータ
	float totalMass;
	float totalHeight;


///////////////////////   関数   ////////////////////////////////

public:
	CRBallHuman();
	virtual ~CRBallHuman();
	int GetPriority(){return SGBP_CREATURE;}

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

	// 基本インターフェース
	virtual void Load(SGScene* scene);				// 基本設定。FrameWorkのLoadで呼ばれる。
	virtual void Loaded(SGScene* scene);			// 基本設定。
	virtual void LoadDerivedModel(SGScene* scene){}	//派生モデル(User,VH)の設定

	bool IsLoaded(){ return bLoaded; }				// ロードの確認
	void SetLoaded(bool load){bLoaded = load;}		// 体全体を読み込まずテストしたい場合
	bool HasFrame(SGFrame* f);

	// VHをコントロール可能にする(Solid,Jointの取得)
	virtual bool Connect(UTRef<SGScene> scene);
	void ConnectSolid(const char* name, UTRef<SGScene> scene);
	void ConnectJoint(const char* name, UTRef<SGScene> scene);

	//VHの各種パラメータの設定
	void SetModel(SGScene* scene);				// 以下の設定関数を使い、VHを使えるようにする。
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
	
	// モデルのスケールをセット
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	// モデルの初期姿勢
	virtual void SetJointInitAngle();

	// VHのSolid,Joint情報取得
	CRBallHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRBallHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	float GetTotalMass(){return totalMass;}						// VHの全体重を取得
	Vec3f GetCOG();												// VHの重心の位置取得
	Vec3d GetCogVelocity();										// VHの重心の速度取得


	void CalcContactForce();
	virtual void RegistSupportParts(UTRef<SGScene> scene){}		// バランスにおいて体支持するSolidを登録(右足、左足など)
	virtual void SetBodyNum(){}									// バランスなどに必要なSolid,Jointの番号の登録
protected:
	void LoadX(const XBallHuman& xh);
	void SaveX(XBallHuman& xh) const;

};

}	//	end namespace Spr
#endif // !defined(CRBallHuman_H)
