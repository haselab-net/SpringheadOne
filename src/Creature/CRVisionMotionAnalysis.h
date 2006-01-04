// CRVisionMotionAnalysis.h: CRVisionMotionAnalysis クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRVISIONMOTIONANALYSIS_H
#define CRVISIONMOTIONANALYSIS_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Creature/CREye.h>
#include <Creature/CRAttention.h>
#include <Creature/CRActionPlanner.h>
#include <Creature/CRInternalModel.h>
#include <Base/Affine.h>
#include <HIS/HIMouse.h>
#include <vector>
#include <map>
#include <Creature/CRPuppet.h>


namespace Spr{;


// SceneGraphから相手の動きに関する視覚情報を読み取るクラス
class CRVisionMotionAnalysis{
public:
	CRVisionMotionAnalysis();
	virtual ~CRVisionMotionAnalysis();

public:
//---------　基本インタフェース　---------//
	void Step();
	void Load(SGScene* scene, CRPuppet* crPuppet, CREye* crEye, CRAttention* crAttention);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//---------　拡張インタフェース　---------//
	void SetInternalModel(CRInternalModel* crInternalModel){
		this->crInternalModel = crInternalModel;
	}

//-----------------　処理　----------------//
	/// 目立つ点を求める
	void ListupAttractivePoints();

//-----------------　メンバ変数　----------------//
	// 関係のあるモジュール
	SGScene* scene; // 汎用
	CRPuppet* crPuppet; // soildが自分かどうかの判別用
	CREye* crEye; // 視野判定用
	CRAttention* crAttention; // ボトムアップの注意用
	CRInternalModel* crInternalModel; // 可視物体からの内部モデル構築用

	// UserのSolid
	PHSolid *soWaistU, *soAbdomenU, *soChestU, *soHeadU;
	PHSolid *soRUArmU, *soRFArmU,   *soRHandU;
	PHSolid *soLUArmU, *soLFArmU,   *soLHandU;
	std::vector<PHSolid*> userSolids;

	// 眼
	PHSolid *soLEye, *soREye;

};

}		//	end namespace Spr
#endif // !defined(CRVISIONMOTIONANALYSIS_H)
