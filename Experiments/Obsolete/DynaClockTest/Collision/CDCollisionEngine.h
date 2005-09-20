#ifndef CDCOLLISIONENGINE_H
#define CDCOLLISIONENGINE_H

#pragma once
#include <SceneGraph/SGBehaviorEngine.h>
#include <Collision/CDFramePair.h>
#include <Collision/CDDetectorImp.h>
#include <WinBasis/WBPreciseTimer.h>

namespace Spr{;


///	衝突時のリスナー(コールバック)クラス．
class CDCollisionListener{
public:
	///	衝突判定後，解析前に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){}
	///	解析直後，交差部の面や頂点が有効なときに，接触している凸形状対の数だけ呼び出されるコールバック．
	virtual void Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){}
	///	解析後に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){}
};
///	リスナのvector
class CDCollisionListeners:public std::vector<CDCollisionListener*>{
public:
	///	衝突判定後，解析前に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr);
	///	解析直後，交差部の面や頂点が有効なときに，接触している凸形状対の数だけ呼び出されるコールバック．
	virtual void Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	///	解析後に呼び出されるコールバック．接触しているフレーム対の数だけ呼び出される．
	virtual void After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr);
};

/**	衝突判定エンジン．
	衝突しているペアを見つけ，衝突判定を行い，コールバックを呼び出す．	*/
class CDCollisionEngine: public SGBehaviorEngine
{
protected:
	///	アクティブ／非アクティブなリスナの指定
	struct ActivePair{
		int pos;
		int frame[2];
	};
	typedef std::vector<ActivePair> ActiveList;
	ActiveList activeList;		///<	衝突判定を行うペアのリスト(デフォルトでは行わない)
	ActiveList inactiveList;	///<	衝突判定を行わないペアのリスト(デフォルトでは行う)
	std::vector<bool> defaults;	///<	デフォルトで判定を行うかどうか
	
	///	衝突判定の一覧．item(i,j) (i<j) でアクセスする．
	CDFramePairWithRecords pairs;
	///	接触解析エンジン．交差部の面や頂点の情報を持っている．
	CDContactAnalysis analyzer;
	///	フレームごとのユーザレコードの数
	int nFrameRecords;
	///	フレームペアごとのユーザレコードの数
	int nFramePairRecords;
	///	凸形状のペアごとのユーザレコードの数
	int nConvexPairRecords;
	///	判定対象フレームのコンテナ型
	typedef std::vector< UTRef<CDFrame> > Frames;
	///	衝突判定対象のフレーム
	Frames frames;
	///	衝突のリスナ
	CDCollisionListeners listeners;
	///	フレームとフレーム番号の対応表
	std::map<SGFrame*, int> frameMap;
public:
	WBPreciseTimer timerGjk, timerQhull, timerNormal;
	///
	SGOBJECTDEF(CDCollisionEngine);
	///
	CDCollisionEngine();

	///@name	BehaviorEngineをオーバーライド
	//{
	///
	int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	時間を dt 進める
	virtual void Step(SGScene* s, float dt, unsigned int count);
	///	判定対象のフレームをクリアする．
	virtual void Clear(SGScene* s);
	///	初期化(frames から pairsを作る)
	void Init();
	//}

	//@name	リスナや判定対象のフレームと関連付けられたデータのアクセス
	//@{
	///	リスナを返す．
	CDCollisionListener* GetListener(int pos){ return listeners[pos]; }
	///	リスナの数
	int NListener(){ return listeners.size(); }

	///	フレームの番号を返す．
	int GetFrameID(SGFrame* fr){ return frameMap[fr]; }
	///	フレーム対を返す．
	CDFramePairWithRecord& GetFramePair(int f1, int f2){
		if (f1 > f2) std::swap(f1, f2);
		return pairs.item(f1, f2);
	}
	///	フレーム対を返す．
	CDFramePairWithRecord& GetFramePair(SGFrame* f1, SGFrame*f2){ return GetFramePair(GetFrameID(f1), GetFrameID(f2)); }
	///	フレームIDからフレームを返す．
	SGFrame* GetFrame(int id){ return frames[id]->frame; }
	///	フレームIDとレコードの位置(ReserveFrameRecord()の返り値)からレコードを返す．
	CDUserRecord* GetFrameRecord(int id, int pos){ return frames[id]->records[pos]; }
	//}
	
	///@name	登録，追加系のメソッド
	//{
	///	判定対象フレームを追加．すべての判定対象フレームは同階層になければならない．
	void AddFrame(SGFrame* frame, CDUserRecord* rec=NULL, int pos=-1);
	///	フレームの数
	int	NFrame(){ return frames.size(); }
	///	リスナーの登録
	int AddListener(CDCollisionListener* l){
		listeners.push_back(l);
		return listeners.size()-1;
	}
	///	衝突判定をしないペアを登録（デフォルトでは衝突判定する）
	bool AddInactive(int f1, int f2, int pos);
	///	衝突判定をしないペアを登録（デフォルトでは衝突判定する）
	bool AddInactive(SGFrame* f1, SGFrame* f2, int pos){ return AddInactive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	衝突判定をするペアを登録（pos のリスナについては，デフォルトでは判定しなくなる．）
	bool AddActive(int f1, int f2, int pos);
	///	衝突判定をするペアを登録（pos のリスナについては，デフォルトでは判定しなくなる．）
	bool AddActive(SGFrame* f1, SGFrame* f2, int pos){ return AddActive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	フレーム(CDFrame) ごとのユーザレコード(records) の場所取り．
	int ReserveFrameRecord();
	///	フレームのペア(CDFramePairWithRecord) ごとのユーザレコード(records) の場所取り．
	int ReserveFramePairRecord();
	///	凸形状のペア(CDConvexPairWithRecord) ごとのユーザレコードの場所取り．
	int ReserveConvexPairRecord();
	//}
};

}

#endif

