// CRAttention.h: CRAttention クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRATTENTION_H
#define CRATTENTION_H

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
#include <Creature/CREye.h>
#include <Creature/CRNeckController.h>
#include <fstream>

namespace Spr{;

class CRPuppet;

// 注意のクラス
class CRAttention{
public:
	CRAttention();
	virtual ~CRAttention();

public:
//---------　基本インタフェース　---------//
	void Step();
	void Load(SGScene* scene,CRPuppet* crPuppet, CREye* crEye,CRNeckController* crNeckController);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------　拡張インタフェース　-----------//
	/// 注視点の設定
	void SetAttentionPoint(Vec3f position, float ammount);
	void SetAttentionSolid(PHSolid* solid, float ammount);
	void StartExperiment();

//-----------------　処理　----------------//
	void CalcMaxAttentionPoint();

//-----------------　メンバ変数　----------------//
	// 関連モジュール
	SGScene* scene; // 汎用
	CRPuppet* crPuppet; // 入出力用
	CREye* crEye; // 出力用
	CRNeckController* crNeckController; // 出力用

	// 注意喚起対象のリスト
	class AttentionInfo{
	public:
		AttentionInfo(){ammount = 0.0f; position = Vec3f(0.0f, 0.0f, 0.0f);}
		AttentionInfo(float a, Vec3f p): ammount(a), position(p) {}
		float ammount;
		Vec3f position;
	};
	typedef std::map<PHSolid*,AttentionInfo>            AttentionList;
	typedef std::map<PHSolid*,AttentionInfo>::iterator  AttentionListIter;
	typedef std::pair<PHSolid*,AttentionInfo>           AttentionListItem;
	AttentionList attentionList;

	float    maxAttentionAmmount;
	PHSolid* maxAttentionSolid;
	Vec3f    maxAttentionPoint;
	bool bFoundAttention;
	bool bHeadControl;
	bool bChangeAttention;
	bool bActiveAttention;
	DWORD attentionChangeTime;

	// 実験
	bool bExperimentMode;
	std::ofstream ofs_head;
	std::ofstream ofs_hedg;
	std::ofstream ofs_leye;
	std::ofstream ofs_reye;
	std::ofstream ofs_gaze;
	std::ofstream ofs_eyes;
	std::ofstream ofs_info;
	std::vector<int>   timinglist;
	std::vector<Vec3f> poslist;
	int counter;
	DWORD startTiming;
	
};

}		//	end namespace Spr
#endif 
