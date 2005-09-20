#ifndef HULL_H
#define HULL_H

#include <Base/Affine.h>
#include <vector>
#include <float.h>

#define HULL_EPSILON 1e-6

//#define HULL_DEBUG		//	デバッグ出力
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif
namespace LVRM{;


///	作られる面
template <class TVtx>
class TPlane{
public:
	Vec3d normal;			///<	面の法線
	float dist;				///<	面の原点からの距離
	
	TVtx* vtx[3];			///<	面を構成する頂点
	TPlane* neighbor[3];	///<	隣の面 vtx[0]-vtx[1] の隣が neighbor[0]
	bool deleted;			///<	削除された面はtrue
	void Clear(){			///<	メモリクリア．使う前に呼ぶ．
		deleted = false;
	}
	///	頂点 v から表側が見えるかどうか
	bool Visible(const Vec3d& v){
		return normal * (v - vtx[0]->Vtx()) > 0;
	}
	///	vの頂点番号を返す(0..2を返す)．見つからなければ3を返す．
	int GetVtxID(TVtx* v){
		int i;
		for(i=0; i<3; ++i) if(v == vtx[i]) break;
		return i;
	}
	///	法線ベクトルと距離を計算する．
	void CalcNormal(){
		normal = (vtx[2]->Vtx() - vtx[0]->Vtx()) ^ (vtx[1]->Vtx() - vtx[0]->Vtx());
		normal.unitize();
		dist = normal * vtx[0]->Vtx();
	}
	///	表裏をひっくり返す．
	void Reverse(){	
		normal = -normal;
		dist = -dist;
		std::swap(vtx[0], vtx[2]);
	}
	///	デバッグ用表示
	void Print(std::ostream& os) const {
		os << "Plane:";
		for(int i=0; i<3; ++i){
			os << " " << *vtx[i];
		}
		os << " n:" << normal;
		if (deleted) os << "del";
		os << std::endl;
	}
};
template <class TVtx>
std::ostream& operator << (std::ostream& os, const TPlane<TVtx>& pl){
	pl.Print(os);
	return os;
}

///	面のバッファ
template <class TVtx>
class TPlanes{
public:
	///	頂点のVector
	class TVtxs: public std::vector<TVtx*>{
	public:
		void Print(std::ostream& os) const {
			for(TVtxs::const_iterator it = begin(); it != end(); ++it){
				(*it)->Print(os);
			}
		}
	};

	typedef TPlane<TVtx> TPlane;

	TPlane* buffer;		///<	バッファへのポインタ new する．
	int len;			///<	バッファの長さ
	TPlane* begin;		///<	最初の面
	TPlane* end;		///<	最後の面の次
	TVtx** vtxBegin;	///<	残っている頂点の先頭
	TVtx** vtxEnd;		///<	残っている頂点の最後の次
	TPlanes(int l):len(l){
		buffer = new TPlane[len];
		begin = buffer;
		end = begin;
	}
	~TPlanes(){ delete buffer; }
	void CreateConvexHull(TVtx** b, TVtx** e){
		vtxBegin = b;
		vtxEnd = e;
		//	最初の面を作る
		CreateFirstConvex();
		HULL_DEBUG_EVAL(std::cout << "First:" << e[-3]->id_ << " " << e[-2]->id_ << " " << e[-1]->id_ << std::endl;)
		for(TPlane* cur = begin; cur != end; ++cur){
			if (cur->deleted) continue;
			TreatPlane(cur);
			assert(end < buffer+len);
		}
	}
	
	void Print(std::ostream& os) const {
		int num=0;
		for(const TPlane* it = begin; it != end; ++it){
			if (!it->deleted) num ++;
		}
		os << num << " planes." << std::endl;
		for(const TPlane* it = begin; it != end; ++it){
			it->Print(os);
		}
	}

private:
	void CreateFirstConvex(){
		TPlanes& planes = *this;
		float xMin, xMax;
		TVtxs::iterator it, xMinVtx, xMaxVtx;
		xMin = xMax = (*vtxBegin)->Vtx().X();
		xMinVtx = xMaxVtx = vtxBegin;
		//	最大と最小を見つける
		for(it = vtxBegin+1; it != vtxEnd; ++it){
			float x = (*it)->Vtx().X();
			if (x < xMin){
				xMin = x;
				xMinVtx = it;
			}
			if (x > xMax){
				xMax = x;
				xMaxVtx = it;
			}
		}
		//	最大を最後，最小を最後から2番目に置く
		std::swap(*xMaxVtx, vtxEnd[-1]);		//	最後と最大を入れ替え
		if (xMinVtx == vtxEnd-1){				//	最後が最小だったら
			std::swap(*xMaxVtx, vtxEnd[-2]);	//	最大だった場所=最後が入っている場所が最小
		}else{
			std::swap(*xMinVtx, vtxEnd[-2]);	//	最小を最後から2番目と入れ替え
		}
		
		//	2つの頂点が作る辺から一番遠い点を見つける
		Vec3d dir = vtxEnd[-2]->Vtx() - vtxEnd[-1]->Vtx();
		dir.unitize();
		Vec3d pos = vtxEnd[-1]->Vtx();
		float maxDist = -1;
		TVtxs::iterator third;
		for(it = vtxBegin; it != vtxEnd-2; ++it){
			Vec3d v = (*it)->Vtx() - pos;
			float dist2 = v.square() - Square(v*dir);
			assert(dist2>=0);
			if (dist2 > maxDist){
				maxDist = dist2;
				third = it;
			}
		}
		std::swap(*third, vtxEnd[-3]);
		planes.end->Clear();
		planes.end->vtx[0] = vtxEnd[-3];
		planes.end->vtx[1] = vtxEnd[-2];
		planes.end->vtx[2] = vtxEnd[-1];
		planes.end->CalcNormal();
		planes.end++;
		
		//	裏表を作って最初の凸多面体にする．
		*planes.end = *planes.begin;
		planes.end->Reverse();
		planes.begin->neighbor[0] = planes.end;
		planes.begin->neighbor[1] = planes.end;
		planes.begin->neighbor[2] = planes.end;
		planes.end->neighbor[0] = planes.begin;
		planes.end->neighbor[1] = planes.begin;
		planes.end->neighbor[2] = planes.begin;
		planes.end++;
		//	使用した頂点を頂点リストからはずす．
		vtxEnd -= 3;
	}

	/**	horizon を作る． cur が穴をあける面，vtx が新しい頂点．
	rv にhorizonを辺に持つ3角形を1つ返す．*/
	void FindHorizon(TVtx*& rv, TPlane* cur, TVtx* vtx){
		//	curの削除．隣の面からの参照も消す．
		for(int i=0; i<3; ++i){
			TPlane* next = cur->neighbor[i];
			if (!next) continue;
			for(int j=0; j<3; ++j){
				if (next->neighbor[j] == cur){
					next->neighbor[j] = NULL;
				break;
				}
			}
		}
		cur->deleted = true;
		//	隣の面について，裏表を判定して処理
		for(int i=0; i<3; ++i){
			TPlane* next = cur->neighbor[i];
			if (!next) continue;
			if (next->Visible(vtx->Vtx())){	//	見える面には穴をあける．
				FindHorizon(rv, next, vtx);
			}else{
				//	地平線を作る頂点にこの面を登録する．
				cur->vtx[i]->horizon = next;
				rv = cur->vtx[i];
			}
		}
	}
	/**	頂点とhorizonの間にコーンを作る．*/
	void CreateCone(TVtx* firstVtx, TVtx* top){
		TPlanes& planes = *this; 
		TVtx* curVtx = firstVtx;
		TPlane* curHorizon = firstVtx->horizon;
		HULL_DEBUG_EVAL( std::cout << "Horizon:" << *curVtx; )
		//	最初の面を張る
		int curVtxID = curHorizon->GetVtxID(curVtx);
		int prevVtxID = (curVtxID+2)%3;
		//	面の作成
		planes.end->Clear();
		planes.end->vtx[0] = curHorizon->vtx[curVtxID];
		planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
		planes.end->vtx[2] = top;
		planes.end->CalcNormal();
		//	curHorizonと接続
		planes.end->neighbor[0] = curHorizon;
		curHorizon->neighbor[prevVtxID] = planes.end;
		TPlane* firstPlane = planes.end;
		//	面の作成完了
		planes.end ++;
	
		//	curHorizon の隣りの3角形を見つけて，curHorizonを更新
		curVtx = curHorizon->vtx[prevVtxID];
		curHorizon = curVtx->horizon;
	 	
		//	2番目以降を張る
		while(1){
			HULL_DEBUG_EVAL(std::cout << *curVtx;)
			int curVtxID = curHorizon->GetVtxID(curVtx);
			int prevVtxID = (curVtxID+2)%3;
			//	面の作成
			planes.end->Clear();
			planes.end->vtx[0] = curHorizon->vtx[curVtxID];
			planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
			planes.end->vtx[2] = top;
			planes.end->CalcNormal();
			//	curHorizonと接続
			planes.end->neighbor[0] = curHorizon;
			curHorizon->neighbor[prevVtxID] = planes.end;
			//	1つ前に作った面と今作った面を接続
			planes.end[-1].neighbor[1] = planes.end;
			planes.end->neighbor[2] = planes.end-1;
	 		//	面の作成完了
			planes.end ++;
	
			//	curHorizon の隣りの3角形を見つけて，curHorizonを更新
			curVtx = curHorizon->vtx[prevVtxID];
			if (curVtx == firstVtx) break;
			curHorizon = curVtx->horizon;
		}
		HULL_DEBUG_EVAL( std::cout << std::endl;)
		//	最後の面とfirstPlaneを接続
		firstPlane->neighbor[2] = planes.end-1;
		planes.end[-1].neighbor[1] = firstPlane;
	}	
	/**	一番遠くの頂点を見つける．見つけたらそれを頂点リストからはずす	*/
	bool FindFarthest(const Vec3d& normal, float maxDist){
		TVtx** maxVtx=NULL;
		for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
			float dist = (*it)->Vtx() * normal;
			if (dist > maxDist + HULL_EPSILON){
				maxDist = dist; 
				maxVtx = it;
			}
		}
		if (maxVtx){
			std::swap(*vtxBegin, *maxVtx);
			vtxBegin++;
			return true;
		}
		return false;
	}
	/*	外側 内側 の順に並べる．
		外側の終わり＝内側の始まりが inner	*/
	TVtx** DivideByPlane(const Vec3d& normal, float dist, TVtx** start, TVtx** end){
		while(start != end){
			float d = (*start)->Vtx() * normal;
			if (d < dist){	//	内側の場合は後ろに移動
				-- end;
				std::swap(*end, *start);
			}else{
				++ start;
			}
		}
		return start;
	}
	/**	一つの面に対する処理を行う．一番遠くの頂点を見つけ，
		地平線を調べ，コーンを作り，内部の頂点をはずす．*/
	void TreatPlane(TPlane* cur){
		if (!FindFarthest(cur->normal, cur->dist)) return;
		HULL_DEBUG_EVAL(std::cout << "Farthest:" << *vtxBegin[-1] << std::endl;)
		//	地平線の調査
		TVtx* hor=NULL;
		FindHorizon(hor, cur, vtxBegin[-1]);
		//	コーンの作成
		TPlane* coneBegin = end;
		CreateCone(hor, vtxBegin[-1]);
		TPlane* coneEnd = end;
		//	コーンに閉じ込められる頂点をvtxEndの後ろに移動
		TVtx** inner = DivideByPlane(-cur->normal, -cur->dist+HULL_EPSILON, vtxBegin, vtxEnd);
		for(TPlane* it=coneBegin; it!=coneEnd; ++it){
			if (it->deleted) continue;
			HULL_DEBUG_EVAL(
				std::cout << "Inner:";
				for(TVtx** v = inner; v != vtxEnd; ++v){
					std::cout << **v;
				}
				std::cout << std::endl;
			);
			inner = DivideByPlane(it->normal, it->dist+HULL_EPSILON, inner, vtxEnd);
		}
		HULL_DEBUG_EVAL(
			std::cout << "InnerFinal:";
			for(TVtx** v = inner; v != vtxEnd; ++v){
				std::cout << **v;
			}
			std::cout << std::endl;
		);
		
		vtxEnd = inner;
	}
};
template <class TVtx>
std::ostream& operator << (std::ostream& os, const TPlanes<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class TVtx>
inline std::ostream& operator << (std::ostream& os, const TPlanes<TVtx>::TVtxs& f){
	f.Print(os); return os;
}


///	頂点クラスの例
class TVtx{
public:
	//	QuickHullが使用するメンバ．必須．
	///  頂点の座標
	const Vec3d Vtx() const { return vtx_; }

	/**	QuickHullアルゴリズム用ワークエリア．
		一番遠い頂点から見える面を削除したあと残った形状のエッジ部分
		を一周接続しておくためのポインタ．
		頂点→面の接続．	面→頂点は頂点の並び順から分かる．	*/
	TPlane<TVtx>* horizon;

	//	その他のメンバ
	Vec3d vtx_;	//	頂点のデータ
	int id_;

	///	デバッグ用表示
	template <class T>
	void Print(T& os) const {
//		os << Vtx();
		os << id_ << " ";
	}
};
inline std::ostream& operator << (std::ostream& os, const TVtx& f){
	f.Print(os); return os;
}

}

#endif
