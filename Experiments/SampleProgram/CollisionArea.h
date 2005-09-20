// CollisionArea.h: CollisionArea クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef COLLISIONAREA_H
#define COLLISIONAREA_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>

#include <Physics/PHHuman.h>
#include <vector>
#include <fstream>


class CollisionArea : public Spr::CDCollisionListener 
{
	Spr::GRRender* render;					//	レンダラー
	Spr::CDCollisionEngine* collision;		//	衝突判定エンジン
	//THuman* human;

public:

	CollisionArea();
	virtual ~CollisionArea();

///////////////////////   関数   /////////////////////////////////

	void Step(Spr::SGScene* scene,Spr::GRRender* mainrender,Spr::PHHuman& phHuman);

	//CollisionEngineを取得し、CDCollisionListenerの登録のため、AddListenerを呼ぶ
	void ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* mainrender,Spr::PHHuman& phHuman);
	
	//接触している頂点を保存
	void Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
		Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer);
	
	//CDCollisionListenerの登録
	void AddListener();
	
	//接触頂点を描画
	void DrawCollisionVetxs();

	//保存されている接触頂点を解放
	void ClearCoVetxs();
	
	void ReLoad();

	//　衝突判定するFrameの決定
	void SelectCDFrame(Spr::PHHuman& phHuman);

	bool HasFrame(Spr::SGFrame* f,Spr::PHHuman& phHuman);

	//描画のための関数
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}



///////////////////////   変数   /////////////////////////////////
private:
	
public:
	std::vector<Spr::Vec3f> coVetxs;	//接触点
	bool bAdd;							//AddListener出来たか否か
	bool bClear;						//coVetxsをクリアするタイミング
	int listenerPos;					//CDContactEngineの中でのリスナの位置
	
};

#endif
