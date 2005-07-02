#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include "PHSolid.h"
#include <SceneGraph/SGScene.h>
#include <vector>

namespace Spr{;

//この辺の機能は出来る限りCollisionとマージしたいところ...
//PHWサフィックスはPHWater内部クラスの意

typedef std::vector<UTRef<CDConvex> > CDConveces;

//ジオメトリが持つ凸多面体をリストアップしたもの
class PHWGeometry : public UTRefCount{
public:
	UTRef<SGFrame>	frame;			//このジオメトリが属する子フレーム
	Vec3f			bbmin, bbmax;	//このジオメトリのBBOX
	CDConveces		conveces;		//このジオメトリを構成する凸多面体
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

//剛体が持つ形状データをリストアップしたもの
class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//剛体
	UTRef<SGFrame>		frame;		//剛体フレーム
	Affinef				posture;	//剛体フレームのワールドフレームに対するposture
	PHWGeometries		geometries;	//剛体のフレームの形状データ
};
typedef std::vector<UTRef<PHWSolid> >		PHWSolids;

/**	剛体と流体との接触力計算エンジン.
	現状ではPHWater 1 対 PHSolid 多
	もちろんPHSolid同士の接触は別途PHContactEngineでやってください．
 */
class PHWaterContactEngine : public SGBehaviorEngine{
public:
	SGOBJECTDEF(PHWaterContactEngine);

	UTRef<PHWater>	water;			///水
	///水フレームのワールドフレームに対するposture
	Affinef			water_posture, water_posture_inv;
	PHWSolids		solids;			///水に接触する剛体

	///
	PHWaterContactEngine();

	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	ロード完了時の処理
	virtual void Loaded(SGScene* scene);
	///	初期化処理、Loadedが呼ぶ
	virtual void Init(SGScene* scene);
	///
	int GetPriority() const { return SGBP_WATERCONTACTENGINE; }
	///	時間を dt 進める．
	virtual void Step(SGScene* s){}
	///	
	virtual void Clear(SGScene* s);
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};


}
#endif

