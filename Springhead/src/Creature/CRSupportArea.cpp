// CRSupportArea.cpp: CRSaftyArea クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "CRSupportArea.h"
#include "CRHuman.h"
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>

namespace Spr{;

void CRContactDetector::AddSubject(SGFrame* s){
	subjects.push_back(s);
}
void CRContactDetector::AddExcept(SGFrame* s){
	excepts.insert(s);
}
void CRContactDetector::Loaded(SGScene* sc){
	contactPairs.clear();
	//	衝突エンジンを見つける．
	PHContactEngine* pce;
	sc->GetBehaviors().Find(pce);
	pce->Loaded(sc);
	collision = pce->GetCollisionEngine();
	//	自分がリスナとして登録されていなければ，登録する．
	int pos;
	for(pos=0; pos<collision->NListener(); ++pos){
		if (this == collision->GetListener(pos)){
			collision->ClearActive(pos);
			collision->ClearInactive(pos);
			break;
		}
	}
	if (pos == collision->NListener()){
		pos = collision->AddListener(this);
	}
	//	アクティブペアを設定する．対象フレームと，
	for(SGFrames::iterator sit = subjects.begin(); sit != subjects.end(); ++sit){
		for(int i=0; i<collision->NFrame(); ++i){	//	全のフレームの
			SGFrame* opp = collision->GetFrame(i);
			std::set<SGFrame*>::iterator f = excepts.find(opp);
			//	除外フレームではないフレームについて
			if (f == excepts.end()){
				//	アクティブに設定
				collision->AddActive(*sit, opp, pos);
				//	接触力計算のため，PHFramePairRecord を contactPairsに追加
				PHContactEngine::FramePairRecord* fpr;
				float sign = 1.0f;
				fpr = pce->GetFramePairRecord(*sit, opp);
				if (!fpr){
					fpr = pce->GetFramePairRecord(*sit, opp);
					sign = -1.0f;
				}
				contactPairs.push_back(CRContactPair(fpr, sign));
			}
		}
	}
}

void CRContactDetector::Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
			  CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	for(CDQHPlane<CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//双対変換をしている。
		//it->deletedがtrueになっていると使わなくなった面を示す。
		if(it->dist != 0 && !it->deleted){
			vtxs.push_back(it->normal / it->dist + conv->commonPoint);
		}
	}
}
void CRContactDetector::CalcContactForce(){
	contactForce.clear();
	contactTorque.clear();
	for(std::vector<CRContactPair>::iterator it = contactPairs.begin(); it != contactPairs.end(); ++it){
		contactForce += it->sign * (it->fpr->GetReflexForce() + it->fpr->GetFrictionForce());
		contactTorque += it->sign * (it->fpr->GetReflexTorque() + it->fpr->GetFrictionTorque());
	}
}


Vec3f CRSupportArea::CRSupportVtxs::ClosestPoint(Vec3f p){
	Vec3f rv;
	if (size() > 1){	//	頂点と辺で1周できる場合
		bool bInside = true;
		bool bFirst = true;
		push_back(at(0));
		float lastCross = 0;
		float minDist2 = FLT_MAX;
		for(int i=0; i<size()-1; ++i){
			Vec3f dir = at(i+1) - at(i);
			Vec3f vec = p - at(i);
			Vec2f dir2(dir.x, dir.z);
			Vec2f vec2(vec.x, vec.z);
			float cross = vec2%dir2;
			if (cross*lastCross < 0) bInside = false;
			lastCross = cross;
			float len = dir.norm();
			dir /= len;
			float proj = vec * dir;
			Vec3f base;							//	線分上の点で p にもっとも近い点
			if (proj < 0) base = at(i);
			else if (proj > len) base = at(i+1);
			else base = at(i) + proj*dir;
			float dist2 = Square(p.x-base.x) + Square(p.z-base.z);
			if (dist2 < minDist2){
				rv = base;
				minDist2 = dist2;
			}
		}
		pop_back();
		if (bInside) rv = p;
	}else if (size()){
		rv = at(0);
	}else{
		rv = p;
	}
	return rv;
}

CRSupportArea::CRSupportArea()
{
}

CRSupportArea::~CRSupportArea()
{
}

void CRSupportArea::DrawCollisionVetxs(GRRender* render){
	if(render){
		render->SetModelMatrix(Spr::Affinef());
		render->SetLineWidth(6);
		render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,0,1)));
		
		//接触点を描画
		for(int i = 0; i < contacts.size(); ++i){
			for(int j = 0; j < contacts[i]->vtxs.size(); ++j){
				render->DrawDirect(GRRender::POINTS,&contacts[i]->vtxs[j],&contacts[i]->vtxs[j]+1);
			}
		}
		
		//　安定領域の描画
		if(supportVtxs.size() > 0){
			supportVtxs.push_back(supportVtxs[0]);
			for(int i = 0; i < supportVtxs.size()-1; ++i){
				render->DrawDirect(GRRender::LINES, &supportVtxs[i], &supportVtxs[i+2]);
			}
			supportVtxs.pop_back();
		}
	}
}

struct CRQHVtx: public Vec3f{
	Vec2f GetPos(){ return Vec2f(X(), Z()); }
};
void CRSupportArea::CalcSupportArea()
{
	supportVtxs.clear();
	static CDQHLines<CRQHVtx> qhull(100);
	static std::vector<CRQHVtx*> vtxs;
	vtxs.clear();
	for(CREnumContacts::iterator it = contacts.begin(); it != contacts.end(); ++it){
		for(int i=0; i<(*it)->vtxs.size(); ++i){
			Vec3f& vtx = (*it)->vtxs[i];
			vtxs.push_back((CRQHVtx*)&vtx);
		}
	}
	qhull.Clear();
	qhull.epsilon = 1e-3f;
	qhull.CreateConvexHull(&*vtxs.begin(), &*vtxs.end());
	if (qhull.nLines > 0){
		CDQHLine<CRQHVtx>* cur = qhull.end-1;
		do{
			supportVtxs.push_back(*cur->vtx[1]);
			cur = cur->neighbor[1];
		} while(cur != qhull.end-1);
	}
}
void CRSupportArea::CalcForce(){
	contactForce.clear();
	contactTorque.clear();
	for(CREnumContacts::iterator it = contacts.begin(); it != contacts.end(); ++it){
		contactForce += (*it)->GetContactForce();
		contactTorque += (*it)->GetContactTorque();
	}
}
Vec3f CRSupportArea::GetApplicationPoint(float y){
	/*	ap: 作用点とすると
		torque = ap ^ force,
		ap * force = 0 とすると ap ⊥ torque,  force ⊥ torque, ap ⊥ force 
		となり，3つのベクトルは直行する．
		そのため，
		|torque| = |ap|*|force| となる．

		ap = ((force^torque) / (|force|*|torque|)) * (|torque|/|force|)
		= (force^torque) / force^2
		
		(ap + k*normal) * normal = interior * normal
		k = (interior-ap) * normal 
	*/
	Vec3f ap = (contactForce^contactTorque) / contactForce.square();
	ap -= ((ap.Y() - y) / contactForce.Y()) * contactForce;
	return ap;
}

void CRSupportArea::SetContacts(CRContactDetector* contact){
	contacts.push_back(contact);
}


}//	end namespace Spr

