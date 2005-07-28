#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include <Base/BaseUtility.h>
#include <SceneGraph/SGScene.h>
#include <vector>

namespace Spr{;

//この辺の機能は出来る限りCollisionとマージしたいところ...
//PHWサフィックスはPHWater内部クラスの意

class CDConvex;
class CDMesh;
//typedef std::vector<UTRef<CDConvex> > CDConveces;

//ジオメトリが持つ凸多面体をリストアップしたもの
class PHWaterRegistanceMap;
class PHWaterContactEngine;
class PHWGeometry : public UTRefCount{
public:
	PHWGeometry();
	UTRef<SGFrame>	frame;			//このジオメトリが属する子フレーム
	CDMesh*			mesh;
	PHWaterRegistanceMap* frm;		///
	CDGeometries	conveces;		//このジオメトリを構成する凸多面体
	void Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

class PHSolid;
/** FRMクラス．

 */
class PHWForceTexture{		//wsのTfluidForceTex
public:
	int ndata;	//prs, friのデータ長
	int rate;	//流体計測時タイムステップの逆数
	float v0;	//基本流速
	std::vector<Vec3f>	prs, fri;	//valarrayの方がいいかな？
};
class PHWaterRegistanceMap;
class PHWHapticSource{		//wsのThapticSourceとTforceSet（1対1と思われるので融合)
public:
	float dthe, dphi;
	int	  nthe, nphi;
	int   ntex;
	float v0;
	Vec3f pos, normal;
	std::vector<PHWForceTexture> ftex;
	float pressure;
	PHWaterRegistanceMap* frm;

	//	流速の設定	ここで，pressureを計算する．
	//	実際は，ほぼ，prs だけが値を持ち， prs * normal が圧力補正値なので，これだけを計算する．
	void SetVelocity(float theta, float phi, Vec3f v, float t);
	//	
	float GetPressure();
	//	Mesh座標系での Haptic Soruce の位置
	Vec3f GetPos();//{ return (pos * 25) + Vec3f(-0.5, -0.28, 0); }
};
class PHWaterRegistanceMap : public SGObject{	//wsのThapticObjみたいなもの
public:
	SGOBJECTDEF(PHWaterRegistanceMap);
	PHWaterRegistanceMap();
	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	virtual void Loaded(SGScene* scene);
	void InitMesh();
	void InitFrmMap();
	void SetVelocity(Vec3f vel, float t);
	
	UTRef<SGFrame>	frame;
	UTRef<CDMesh>	mesh;
	UTString		filename;
	float			pressureGain;	//haptic sourceが計算した圧力に掛け合わせる定数．
	Affinef			posture;		//meshに対する相対変換．
	TVec3<int>		sym;			//シンメトリーフラグ
	std::vector<PHWHapticSource> hsrc;
	std::vector<PHWHapticSource*> vtxHsrcMap;
	std::vector<PHWHapticSource*> dirHsrcMap;

	//	ある座標の最近傍のhsrcを見つけるためのマップ
	enum{
		NTHETA=20,
		NPHI=40,
	} DirMapSize;
	float dTheta, dPhi;	//	マップの間隔
	//	最近棒のHsrcを見つける
	PHWHapticSource* FindHsrc(Vec3f pos);
};
class  PHWaterRegistanceMaps: public std::vector<UTRef<PHWaterRegistanceMap> >{
};

class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//剛体
	UTRef<SGFrame>		frame;		//剛体フレーム
	//UTRef<PHWRegistanceMap> frm;
//	Affinef				posture;	//剛体フレームのワールドフレームに対するposture
	PHWGeometries		geometries;	//剛体のフレームの形状データ
	void Init(PHWaterContactEngine* e);
	void EnumGeometries(SGFrame*, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWSolid> >		PHWSolids;

/**	剛体と流体との接触力計算エンジン.
	現状ではPHWater 1 対 PHSolid 多
	もちろんPHSolid同士の接触は別途PHContactEngineでやってください．
 */
class PHWater;
class PHWaterContactEngine : public SGBehaviorEngine{
public:
	SGOBJECTDEF(PHWaterContactEngine);
	bool bUseFrm;

	UTRef<PHWater>	water;			///水
	PHWSolids		solids;			///水に接触する剛体
	PHWaterRegistanceMaps	frms;	///

	std::vector<Vec3f> tris;		///<	デバッグ用:水面下の3角形
	std::vector<Vec3f> points;		///<	デバッグ用:物体と重なっている水

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
	virtual void Step(SGScene* s);
	///	
	virtual void Clear(SGScene* s);
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///
	virtual void Render(GRRender* r, SGScene* s);
};


}
#endif

