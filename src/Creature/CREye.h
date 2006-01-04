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

// VHの目のクラス... 視野、眼球運動制御など
class CREye{
public:
	CREye();
	virtual ~CREye();

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
	/// 視線を得る
	Vec3f GetVisualAxis();
	/// 注視モードの設定
	void SetAttentionMode();
	/// 注視点が眼球の可動範囲を超えた位置にあるかどうか
	bool IsOverRange();
	/// ベクトルgoalが眼球の可動範囲を超えた位置にあるかどうか
	bool IsOverRange(Vec3f goal);
	/// 対象Solidが視野内にあるかどうか
	bool IsVisible(PHSolid* solid);
	float GetVisibility(PHSolid* solid);

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
	/// VirtualHumanの目を制御する(個別)
	void ControlEye(SGFrame* frEye, PHSolid* soEye, PHJointBallPid* jpEye, const int eyeType);

//-----------------　メンバ変数　----------------//
	// 関連モジュール
	SGScene* scene; // 汎用
	CRPuppet* crPuppet; // 入出力用

	// VHの目
	PHSolid* soREye;
	PHSolid* soLEye;
	SGFrame* frREye;
	SGFrame* frLEye;
	SGFrame* frHead;
	// 目のPIDコントローラ
	PHJointBallPid* jpREye;
	PHJointBallPid* jpLEye;

	/// 注視点,	注視方向
	Vec3f attentionPoint;
	Vec3f attentionDir[2];
	Vec3f lastInRangeDir[2];

	/// 注視モードか否か
	bool bAttention;
	/// VH視点表示の切り替え
	bool bVHPointOfView;

	// VH視点表示前の視点の保存用
	Affinef viewOrig;
	float frontOrig;
public:
	int bEyeMode;
};

}		//	end namespace Spr
#endif 
