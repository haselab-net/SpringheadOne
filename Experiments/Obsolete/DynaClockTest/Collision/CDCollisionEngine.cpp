#include "Collision.h"
#pragma hdrstop

namespace Spr{;

void CDCollisionListeners::Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){
	for(unsigned  i=0; i<size(); ++i){
		if (fr->IsActive(i)) begin()[i]->Before(scene, dt, count, fr);
	}
}
void CDCollisionListeners::Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	for(unsigned  i=0; i<size(); ++i){
		if (fr->IsActive(i)) begin()[i]->Analyzed(scene, dt, count, fr, geom, conv, analyzer);
	}
}
void CDCollisionListeners::After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){
	for(unsigned  i=0; i<size(); ++i){
		if (fr->IsActive(i)) begin()[i]->After(scene, dt, count, fr);
	}
}
	
SGOBJECTIMP(CDCollisionEngine, SGBehaviorEngine);
CDCollisionEngine::CDCollisionEngine():nFrameRecords(0), nFramePairRecords(0), nConvexPairRecords(0){
}

int CDCollisionEngine::ReserveFrameRecord(){
	nFrameRecords ++;
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		(*it)->records.resize(nFrameRecords);
	}
	return nFrameRecords-1;
}
int CDCollisionEngine::ReserveConvexPairRecord(){
	nConvexPairRecords ++;
	for(CDFramePairWithRecords::iterator itf = pairs.begin(); itf != pairs.end(); ++itf){
		for(CDGeometryPairs::iterator itg = itf->geometryPairs.begin();
			itg != itf->geometryPairs.end(); ++itg){
			for(CDConvexPairs::iterator itc = (*itg)->convexPairs.begin(); itc != (*itg)->convexPairs.end(); ++itc){
				CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*)&**itc;
				cp->records.resize(nConvexPairRecords);
			}
		}
	}
	return nConvexPairRecords-1;
}
int CDCollisionEngine::ReserveFramePairRecord(){
	nFramePairRecords ++;
	for(CDFramePairWithRecords::iterator itf = pairs.begin(); itf != pairs.end(); ++itf){
		itf->records.resize(nFramePairRecords);
	}
	return nFramePairRecords-1;
}
void CDCollisionEngine::AddFrame(SGFrame* frame, CDUserRecord* rec, int pos){
	CDFrame* cf=NULL;
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		if ((*it)->frame == frame){
			cf = *it;
		}
	}
	if (!cf){
		frames.push_back(new CDFrame);
		cf = frames.back();
		cf->frame = frame;
		cf->records.resize(nFrameRecords);
		frameMap[frame] = frames.size()-1;
	}
	if (pos>=0) cf->records[pos] = rec;
}
bool CDCollisionEngine::AddInactive(int f1, int f2, int pos){
	if (f1<0 || f1>NFrame() || f2<0 || f2>NFrame()) return false;
	if (f1 < pairs.height() && f2 < pairs.width()){
		//	すでに初期化済みで，フレームペアがある場合．
		if (f1 > f2) std::swap(f1, f2);
		pairs.item(f1, f2).isActive[pos] = false;
		pairs.item(f1, f2).isActive.Update();
	}
	//	未初期化なので，フレームペアがないので，記録だけしておく
	inactiveList.push_back(ActivePair());
	inactiveList.back().frame[0] = f1;
	inactiveList.back().frame[1] = f2;
	inactiveList.back().pos = pos;
	return true;
}
bool CDCollisionEngine::AddActive(int f1, int f2, int pos){
	if (f1<0 || f1>NFrame() || f2<0 || f2>NFrame()) return false;
	if (f1 < pairs.height() && f2 < pairs.width()){
		//	すでに初期化済みで，フレームペアがある場合．
		if (f1 > f2) std::swap(f1, f2);
		pairs.item(f1, f2).isActive[pos] = true;
		pairs.item(f1, f2).isActive.Update();
	}
	activeList.push_back(ActivePair());
	activeList.back().frame[0] = f1;
	activeList.back().frame[1] = f2;
	activeList.back().pos = pos;
	return true;
}
/**	Frameの端の位置をもち，ソートされるもの	*/
struct CDFrameEdge{
	float edge;				///<	端の位置(グローバル系)
	int frame;				///<	元の frameの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const CDFrameEdge& s) const { return edge < s.edge; }
};

void CDCollisionEngine::Clear(SGScene* s){
	for(CDFramePairWithRecords::iterator it = pairs.begin(); it != pairs.end(); ++it){
		it->Clear();
	}
	pairs.clear();
	frames.clear();
	frameMap.clear();
	nFrameRecords = -1;
	nConvexPairRecords = -1;
}
void CDCollisionEngine::Init(){
	//	デフォルトで判定をするか，しないかを調べる．
	std::vector<bool> defaults;
	defaults.resize(listeners.size(), true);
	for(unsigned i=0; i<activeList.size(); ++i){
		defaults[activeList[i].pos] = false;
	}
	//	pairsのサイズを対象フレームの数にあわせて広げる
	unsigned oldSz = pairs.height();
	pairs.resize(frames.size(), frames.size());
	//	広げた部分にフレームを設定する．
	for(unsigned i=0; i<frames.size(); ++i){
		for(unsigned j=i+1; j<frames.size(); ++j){
			CDFramePairWithRecord& pair = pairs.item(i,j);
			if (i>=oldSz || j>=oldSz){
				pair.Set(frames[i], frames[j]);
				for(unsigned k=0; k < pair.isActive.size(); ++k) pair.isActive[k] = false;
			}
			//	除外フラグのサイズをリスナの数にあわせる
			unsigned isActiveSize = pair.isActive.size();
			pair.isActive.resize(listeners.size()+1);
			for(unsigned i=isActiveSize-1; i<pair.isActive.size()-1; ++i){
				pair.isActive[i] = defaults[i];
			}
			//	ユーザレコードの数を合わせる
			pair.records.resize(nFramePairRecords);
		}
	}
	//	判定対象のフレームのペアに判定フラグを設定
	for(unsigned i=0; i<activeList.size(); ++i){
		ActivePair& a = activeList[i];
		if (a.frame[0] > a.frame[1]) std::swap(a.frame[0] , a.frame[1]);
		pairs.item(a.frame[0], a.frame[1]).isActive[a.pos] = true;
	}
	//	判定除外リストのフレームに除外フラグを設定
	for(unsigned i=0; i<inactiveList.size(); ++i){
		ActivePair& ia = inactiveList[i];
		if (ia.frame[0] > ia.frame[1]) std::swap(ia.frame[0] , ia.frame[1]);
		pairs.item(ia.frame[0], ia.frame[1]).isActive[ia.pos] = false;
	}
	//	除外フラグの設定，フラグの最後の要素は，フラグ全体のorが入る
	for(unsigned int i=0; i<frames.size(); ++i){
		for(unsigned int j=i+1; j<frames.size(); ++j){
			CDFramePairWithRecord& pair = pairs.item(i,j);
			pair.isActive.Update();
		}
	}

/*	
	for(unsigned i=0; i<frames.size(); ++i){
		for(unsigned j=i+1; j<frames.size(); ++j){
			CDFramePairWithRecord& pair = pairs.item(i,j);
			DSTR << pair.frame[0]->frame->GetName() << " - " << pair.frame[1]->frame->GetName() << std::endl;
			for(int k=0; k<pair.isActive.size(); ++k){
				DSTR << " " << pair.isActive[k] << std::endl;
			}
		}
	}
	DSTR << std::endl;
*/

	//	凸形状のごとのユーザレコードの数を設定
	for(CDFramePairWithRecords::iterator itf = pairs.begin(); itf != pairs.end(); ++itf){
		for(CDGeometryPairs::iterator itg = itf->geometryPairs.begin();
			itg != itf->geometryPairs.end(); ++itg){
			for(CDConvexPairs::iterator itc = (*itg)->convexPairs.begin(); itc != (*itg)->convexPairs.end(); ++itc){
				CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*)&**itc;
				cp->records.resize(nConvexPairRecords);
			}
		}
	}
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		(*it)->records.resize(nFrameRecords);
	}
}
void CDCollisionEngine::Step(SGScene* scene, float dt, unsigned int count){
#if 1
	//	両方の端を並べたリストを作り，ソートする．
	Vec3f dir(1,0,0);
	typedef std::vector<CDFrameEdge> Edges;
	Edges edges;
	edges.resize(frames.size()*2);
	Edges::iterator eit = edges.begin();
	for(unsigned int i = 0; i < frames.size(); ++i){
		frames[i]->frame->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		if (!_finite(eit[0].edge) || !_finite(eit[1].edge)){
			DSTR << frames[i]->frame->GetName() << " min:" << eit[0].edge << " max:" << eit[1].edge << std::endl;
			frames[i]->frame->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		}
		eit[0].frame = i; eit[0].bMin = true;
		eit[1].frame = i; eit[1].bMin = false;
		eit += 2;
	}
	if (edges.size() > 200){
		DSTR << "strange edges" << std::endl;
		DSTR << (DWORD)edges.begin() << (DWORD)edges.end();
	}
	std::sort(edges.begin(), edges.end());
	//	端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> FrameSet;
	FrameSet cur;							//	現在のSolidのセット
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
//			DSTR << "Col " << it->solid << " - ";
			for(FrameSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->frame;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				CDFramePairWithRecord& pair =  pairs[f1*frames.size() + f2];
				//	フレームとフレームの衝突判定
				timerGjk.Start();
				if (pair.IsActive() && pair.Detect(count)){	//	再帰的に衝突判定(GJKを使用)
					timerGjk.Stop();
					listeners.Before(scene, dt, count, &pair);
					for(CDIntersections::iterator it = pair.intersections.begin(); it != pair.intersections.end(); ++it){
						timerQhull.Start();
						analyzer.FindIntersection(*it->convexPair, it->geometryPair->posture);	//	接触形状の解析
						timerQhull.Stop();
						timerNormal.Start();
						analyzer.CalcNormal(*(CDConvexPairWithNormal*)it->convexPair, it->geometryPair->posture);		//	法線ベクトルの計算
						timerNormal.Stop();
						listeners.Analyzed(scene, dt, count, &pair, it->geometryPair, (CDConvexPairWithRecord*) it->convexPair, &analyzer);
					}
					listeners.After(scene, dt, count, &pair);
				}
				timerGjk.Stop();
//				DSTR << " " << *itf;
			}
//			DSTR << std::endl;
			cur.insert(it->frame);
		}else{
			cur.erase(it->frame);			//	終端なので当該フレームを削除．
		}
	}
#else
	for(int i=0; i<pairs.height(); ++i){
		for(int j=i+1; j<pairs.width(); ++j){
			CDFramePairWithRecord& pair =  pairs.item(i, j);
			//	フレームとフレームの衝突判定
			timerGjk.Start();
			if (pair.IsActive() && pair.Detect(count)){	//	再帰的に衝突判定(GJKを使用)
				timerGjk.Stop();
				listeners.Before(scene, dt, count, &pair);
				for(CDIntersections::iterator it = pair.intersections.begin(); it != pair.intersections.end(); ++it){
					//	DSTR << pair.frame[0]->frame->GetName() << "-" << pair.frame[1]->frame->GetName() << "  ";
					timerQhull.Start();
					analyzer.FindIntersection(*it->convexPair, it->geometryPair->posture);	//	接触形状の解析
					timerQhull.Stop();
					timerNormal.Start();
					analyzer.CalcNormal(*(CDConvexPairWithNormal*)it->convexPair, it->geometryPair->posture);		//	法線ベクトルの計算
					timerNormal.Stop();
					listeners.Analyzed(scene, dt, count, &pair, it->geometryPair, (CDConvexPairWithRecord*) it->convexPair, &analyzer);
				}
				listeners.After(scene, dt, count, &pair);
			}
			timerGjk.Stop();
		}
	}
#endif
}

}
