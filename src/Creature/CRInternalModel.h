// CRInternalModel.h: CRInternalModel クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRINTERNALMODEL_H
#define CRINTERNALMODEL_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Base/Affine.h>
#include <Framework/FWFileIOD3D.h>
#include <Creature/CRUser.h>
#include <Creature/CRPuppet.h>
#include <vector>


namespace Spr{;

class CRPuppet;
class CRBaseBrain;
class CRAttention;

// VHの持つ、環境と自分に関する内部モデル
class CRInternalModel{
public:
	CRInternalModel();
	virtual ~CRInternalModel();

public:
//---------　基本インタフェース　---------//
	void Step();
	void Load(std::string fn, GRRender* render, CRPuppet* realPuppet);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------　拡張インタフェース　-----------//
	SGScene*  GetScene(){  return internalModel;  }
	CRUser*   GetUser(){   return &internalUser;   }
	CRPuppet* GetPuppet(){ return &internalPuppet; }
	void SetPosition(int i, Vec3f pos); // Obsolete予定
	void EnableVisualUpdate();
	void DisableVisualUpdate();
	void UpdateCertainty(int i, Affinef posture, float certainty);
	void AttentionFromCertainty(CRAttention* crAttention);
	bool IsStable(){ return bStability;}
	bool IsPredicting(){ return !bVisualUpdate; }

//-----------------　処理　----------------//
	bool ApplyVision();
	bool ApplyRealPuppet();
	void DecayCertainty();

//-----------------　メンバ変数　----------------//
	/// パラメータ定数(Initで初期化) ほんとはファイルから読みたい
	float Kp;
	float Kd;
	float Ktp;
	float Ktd;

	/// 内部モデルを保持するScene Graph
	UTRef<SGScene> internalModel;
	SGBehaviorStates stateTemp;
	/// ファイルローダ
	FWFileIOD3D fileIO;
	/// ユーザの内部モデル
	CRUser internalUser;
	/// VHの内部モデル
	CRPuppet internalPuppet;
	/// 本物のVH
	CRPuppet* realPuppet;
	/// VHの行動の内部モデル
	CRBaseBrain* iPuppetBrain;
	/// UserのSolidの位置の確信度付きテーブル
	//// 要素
	class SolidCertainty{
	public:
		SolidCertainty(Affinef p, float c): posture(p), certainty(c){}
		// SolidのPosture
		Affinef posture;
		// Solidがpostureにあるという確信度(= posの想定誤差)
		float certainty;
	};
	//// リスト
	std::vector<SolidCertainty> certaintyTable;

	/// 視覚情報で更新するかどうか(=シミュレーション中かどうか)
	bool bVisualUpdate;
	/// 見えた位置とモデルの位置の整合が完了したかどうか (現在は使用していないはず)
	bool bStability;

	/// テスト用
	PHSolid *soRFArmU, *soRHandU, *soLFArmU, *soLHandU;
	PHSolid *soRFArm,  *soRHand,  *soLFArm,  *soLHand;

};

}		//	end namespace Spr
#endif 
