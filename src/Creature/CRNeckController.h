// CRNeckController.h: CRNeckController クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRNECKCONTROLLER_H
#define CRNECKCONTROLLER_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Base/Affine.h>
#include <HIS/HIMouse.h>
#include <vector>


namespace Spr{;

class CRPuppet;

// VHの首のクラス... 頭部運動制御
class CRNeckController{
public:
	CRNeckController();
	virtual ~CRNeckController();

public:

//---------　基本インタフェース　---------//
	void Step();
	void Load(SGScene* scene,CRPuppet* crPuppet);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------　拡張インタフェース　-----------//
	/// 注視点の設定
	void SetAttentionPoint(Vec3f position);
	/// 注視モードの設定
	void SetAttentionMode();
	/// 目標値までの誤差
	float GetErrAbs();

//-----------------　処理　----------------//
	/// VirtualHumanの目を制御する
	void ControlNeck();

//-----------------　メンバ変数　----------------//
	// 関連モジュール
	SGScene* scene; // 汎用
	CRPuppet* crPuppet; // 入出力用?

	// 頭
	PHSolid* soHead;
	SGFrame* frHead;
	// 首
	PHJointBall* joNeck;
	PHJointBallPid* jpNeck;
	// 胸
	SGFrame* frChest;
	// 目
	PHSolid *soLEye, *soREye;

	/// 目標視線方向
	Vec3f goalVisualAxis;
	Vec3f attentionPoint;
	
	/// 注視モードか否か
	bool bAttention;

};

}		//	end namespace Spr
#endif 
