// CREye.h: CREye クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CREYE_H
#define CREYE_H

#include <Creature/CRPuppet.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Base/Affine.h>
#include <vector>


namespace Spr{;

class CRPuppet;
class CRNeckController;

// VHの目のクラス... 視野、眼球運動制御など
class CREye{
public:
	CREye();
	virtual ~CREye();

public:

//---------　基本インタフェース　---------//
	void Step();
	void Load(SGScene* scene,CRPuppet* crPuppet,CRNeckController* crNeck);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------　拡張インタフェース　-----------//
	/// 注視点の設定
	void SetAttentionPoint(Vec3f position);
	/// 視線を得る
	Vec3f GetVisualAxis();
	/// 注視モードの設定
	void SetAttentionMode();
	/// 注視点が眼球の可動範囲を超えた位置にあるかどうか
	bool IsOverRange();
	bool IsOverRange(float vertLimit, float horizLimit);
	/// ベクトルgoalが眼球の可動範囲を超えた位置にあるかどうか
	bool IsOverRange(Vec3f goal);
	bool IsOverRange(Vec3f goal, float vertLimit, float horizLimit);
	/// 対象Solidが視野内にあるかどうか
	bool IsVisible(PHSolid* solid);
	float GetVisibility(PHSolid* solid);
	/// 眼球可動範囲ぎりぎりまで動かした時の視線方向
	Vec3f LimitRange(Vec3f goal);
	Vec3f LimitRange(Vec3f goal, float vertLimit, float horizLimit);

	void ToggleEyeMode(){
		bEyeMode++;
		if (bEyeMode > 2) bEyeMode = 0;
	}

//-----------------　処理　----------------//
	/*
	/// 視点表示の切り替え
	void ToggleVHPointOfView(GRRender* render,HIMouse* mouse);
	/// VirtualHumanの視点にする
	void SetPointOfView(HIMouse* mouse);
	*/
	/// VirtualHumanの目を制御する
	void ControlEyes();
	/// VirtualHumanの目を目標視線になるようPD制御する
	void ControlEyePD(SGFrame* frEye, PHSolid* soEye, Vec3f aim);
	/// 眼球運動のモデル
	void DeterminAttentionDir();

//-----------------　メンバ変数　----------------//
	// 関連モジュール
	SGScene* scene; // 汎用
	CRPuppet* crPuppet; // 入出力用
	CRNeckController* crNeck; // 協調動作

	// VHの目
	PHSolid* soREye;
	PHSolid* soLEye;
	PHSolid* soHead;
	SGFrame* frREye;
	SGFrame* frLEye;
	SGFrame* frHead;
	// 目のPIDコントローラ
	PHJointBallPid* jpREye;
	PHJointBallPid* jpLEye;

	/// 注視点,	注視方向
	Vec3f attentionPoint;
	Vec3f attentionDirL;
	Vec3f attentionDirR;

	/// 前回の目標位置(視角)
	float last_t1_a, last_t2_a, last_t3_a, last_t4_a;

	/// 積分器
	float integrator_L, integrator_R, integrator_Lv, integrator_Rv;

	/// 注視モードか否か
	bool bAttention;
	/// VH視点表示の切り替え
	bool bVHPointOfView;
	/// Saccade中
	bool  bSaccade;
	DWORD saccadeStartTime;
	Vec3f saccadeStartDirL, saccadeStartDirR;
	Vec3f saccadeGoalDirL, saccadeGoalDirR;
	int saccadeCount;

	// VH視点表示前の視点の保存用
	Affinef viewOrig;
	float frontOrig;
public:
	int bEyeMode;

	// 実験記録用
	float eyeposR, eyeposL;
	float overrange;
};

}		//	end namespace Spr
#endif 
