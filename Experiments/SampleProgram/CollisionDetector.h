// CollisionDetector.h: CollisionDetector クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <vector>

class CollisionDetector : public Spr::CDCollisionListener 
{
	Spr::GRRender* render;					//	レンダラー
	Spr::CDCollisionEngine* collision;		//	衝突判定エンジン
public:
	CollisionDetector();
	virtual ~CollisionDetector();
	void ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* mainrender);
	//接触している頂点を保存
	void Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
		Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer);
	//CDCollisionListenerの登録
	//void AddListener(Spr::SGScene* scene,Spr::CDCollisionEngine* collision);
	void AddListener();
	//接触頂点を描画
	void DrawCollisionVetxs();
	//保存されている接触頂点を解放
	void ClearCoVetxs();
	
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}
	std::vector<Spr::Vec3f> coVetxs;
	bool bAdd;
};

#endif
