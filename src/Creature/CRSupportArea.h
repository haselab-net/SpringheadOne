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
#include <Physics/PHContactEngine.h>

namespace Spr{;

class CRHuman;

class CRContactDetector : public CDCollisionListener, public UTRefCount{
public:
	struct CRContactPair{
		CRContactPair(PHContactEngine::FramePairRecord* f, float s): fpr(f), sign(s){}
		PHContactEngine::FramePairRecord* fpr;
		float sign;
	};
protected:
	SGFrames subjects;						///<	足の裏など支えるものの剛体のリスト
	std::set<SGFrame*> excepts;				///<	体など，除外すべき剛体のリスト
	CDCollisionEngine* collision;			///<	衝突判定エンジン
	std::vector<CRContactPair> contactPairs;///<	接触力計算のための接触ペア
	Vec3f contactForce, contactTorque;		///<	接触力，接触トルク
public:
	CRContactDetector(){}
	virtual ~CRContactDetector(){}
	virtual void AddSubject(SGFrame* f);///<	
	virtual void AddExcept(SGFrame* f);	///<	
	virtual void Loaded(SGScene* s);	///<	ロード後の処理=初期化処理
	std::vector<Vec3f> vtxs;			///<	衝突点
	virtual void BeforeAll(SGScene* s){ vtxs.clear(); }
	virtual void AfterAll(SGScene* s){ CalcContactForce(); }
	//	接触している頂点をvtxsに保存
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
		CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	//	接触力の計算
	void CalcContactForce();
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetApplicationPoint(float y=0);						//	作用点
};


class CRSupportArea : public CDCollisionListener{
public:
	CRSupportArea();
	virtual ~CRSupportArea();
	typedef std::vector< UTRef<CRContactDetector> > CREnumContacts;
	class CRSupportVtxs:public std::vector<Vec3f>{
	public:
		Vec3f ClosestPoint(Vec3f p);
	};
protected:
	CREnumContacts contacts;
	CRSupportVtxs supportVtxs;	// 安定領域の点
	Vec3f contactForce, contactTorque;
public:
	void Step(){
		CalcSupportArea();
		CalcForce();
	}
	void SetContacts(CRContactDetector* contact);				//	安定領域になる接触物
	void DrawCollisionVetxs(GRRender* render);					//	安定領域と接触頂点を描画
	CRSupportVtxs& GetSupportArea(){ return supportVtxs;}	// 安定領域を取得
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetApplicationPoint(float y=0);						//	作用点
protected:
	void CalcForce();
	void CalcSupportArea();										//	安定領域の計算
};		

}		//end namespace Spr
#endif // !defined(CR_SUPPORT_AREA_H)
