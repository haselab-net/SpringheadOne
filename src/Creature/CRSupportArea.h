// CRSupportArea.h: CRSaftyArea クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_SUPPORT_AREA_H
#define CR_SUPPORT_AREA_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <vector>
#include <fstream>

//#include "CRHuman.h"

namespace Spr{;

class CRHuman;

class CRSupportArea : public CDCollisionListener
{
public:
	CRSupportArea();
	virtual ~CRSupportArea();

	CDCollisionEngine* collision;		//	衝突判定エンジン

private:
	std::vector<Vec3f> coVetxs;			// 接触点
	std::vector<Vec3f> supportVetxs;	// 安定領域の点
	bool bAdd;							// AddListener出来たか否か
	bool bClear;						// coVetxsをクリアするタイミング
	int listenerPos;					// CDContactEngineの中でのリスナの位置
	

public:
	void Step();
	void Load(SGScene* scene,CRHuman* crHuman);

	//	CollisionEngineを取得し、CDCollisionListenerの登録 
	void AddListener(SGScene* scene);

	//接触している頂点を保存
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
		CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	
	void DrawCollisionVetxs(GRRender* render);				// 接触頂点を描画
	void ClearCoVetxs();									// 保存されている接触頂点を解放
	void SelectCDFrame(CRHuman* crHuman);					// 衝突判定するFrameの決定
	bool HasFrame(SGFrame* f,CRHuman* crHuman);				// FrameがVHかどうか確認
	void CalcSupportArea();									// 安定領域の計算
	std::vector<Vec3f> GetSupportArea(){ return coVetxs;}	// 安定領域を取得

	// CalcSupportAreaで使う変数
	typedef struct
	{
		double			x;
		double			y;
		int				n;
	} M_point;

	int point_number;		// マウスの点の個数 
	int search_number;		// 求める点の個数 
	M_point M_vec_draw;		// マウスの位置
	std::vector<M_point> M_vec;	// マウスの点の履歴　多分入力データになる?
	std::vector<M_point> S_vec;	// 外形となる点の座標
	Vec2f coCog;				//安定領域の中心（重心）


};		

}		//end namespace Spr
#endif // !defined(CR_SUPPORT_AREA_H)
