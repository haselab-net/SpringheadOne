#ifndef SGSCENE_H
#define SGSCENE_H

#include "SGFrame.h"
#include "SGBehaviorEngine.h"
#include "SGRenderEngine.h"

namespace Spr {;


class SGScene;

class SGObjectNamesLess{
public:
	bool operator () (SGObject* o1, SGObject* o2) const {
		return strcmp(o1->GetName(), o2->GetName()) < 0;
	}
};
/**	名前とシーングラフのオブジェクトの対応表
	名前をキーにしたセットを用意し，名前の一意性の保証とオブジェクトの
	検索を実現する．	*/
class SGObjectNames:public std::set<UTRef<SGObject>, SGObjectNamesLess >{
public:
	typedef std::map<UTString, UTString> TNameMap;
	TNameMap nameMap;
	void Print(std::ostream& os) const;
	///	名前からオブジェクトを取得
	SGObject* Find(UTString name) const {
		SGObject key;
		key.name = name;
		key.AddRef();
		const_iterator it = find(&key);
		key.DelRef();
		if (it != end()) return *it;
		return NULL;
	}
	std::pair<iterator, iterator> Range(UTString name){
		std::pair<iterator, iterator> rv;
		SGObject key;
		key.name = name;
		key.AddRef();
		rv.first = find(&key);
		key.DelRef();
		if (rv.first == end()){
			rv.second = end();
		}else{
			rv.second = rv.first;
			++rv.second;
			for(; rv.second!=end(); ++rv.second){
				if (rv.second == end()) break;
				if (name.compare(0, name.length(), (*rv.second)->GetName())!=0) break;
			}
		}
		return rv;
	}
	/**	オブジェクトの追加，
		名前のないオブジェクトは追加できない．この場合 false を返す．
		追加に成功すると true． すでに登録されていた場合は false を返す．
		名前がぶつかる場合は，追加するオブジェクトの名前が変更される．	*/
	bool Add(SGObject* obj);
	///	オブジェクトの削除
	bool Del(SGObject* key){
		iterator it = find(key);
		if (it==end()) return false;
		erase(it);
		return true;
	}
	///	オブジェクトの削除
	bool Del(UTString name){
		SGObject key;
		key.name = name;
		key.AddRef();
		iterator it = find(&key);
		key.DelRef();
		if (it==end()) return false;
		erase(it);
		return true;
	}
};
inline std::ostream& operator << (std::ostream& os, const SGObjectNames& ns){
	ns.Print(os); return os;
}

/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
	OpenGLのシーングラフをD3Dに変換するためには，一度Documentに
	セーブして，D3D形式でロードしなければならない．	*/
class SPR_DLL SGScene:public SGObject{
	SGOBJECTDEF(SGScene);
protected:
	///	名前とオブジェクトの対応表	
	SGObjectNames names;
	///	ビヘイビアエンジン
	SGBehaviorEngines behaviors;
	///	レンダリングエンジン
	SGRenderEngines renderers;
	///	トップフレーム
	UTRef<SGFrame> world;
	/// 積分ステップ
	double timeStep;
	/// 積分した回数
	unsigned int count;
	///	1ステップあたりの速度の減衰率．粘性のように働く．大きくすると粘性が増し安定になる．
	double velocityLossPerStep;
public:

	///	コンストラクタ
	SGScene();
	///	デストラクタ
	~SGScene(){Clear();}
	///	ビヘイビアエンジンの vector を返す．
	SGBehaviorEngines& GetBehaviors(){ return behaviors; }
	const SGBehaviorEngines& GetBehaviors() const { return behaviors; }
	///	レンダリングエンジンの vector を返す．
	SGEngines& GetRenderers(){
		return renderers;
	}

	///	トップフレームを返す．
	SGFrame* GetWorld(){ return world; }
	///	名前からオブジェクトを取得
	SGObject* FindObject(UTString name){ return names.Find(name); }
	///	型と名前からオブジェクトを取得
	template <class T> void FindObject(UTRef<T>& t, UTString name){
		T* p;
		FindObject(p, name);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name){
		t = DCAST(T, FindObject(name));
	}
	typedef SGObjectNames::iterator SetIt;
	typedef std::pair<SetIt, SetIt> SetRange;
	SetRange RangeObject(UTString n){ return names.Range(n); }
	
	SGObjectNames::TNameMap& GetNameMap(){ return names.nameMap; }

	/// 積分ステップを返す
	double GetTimeStep()const{return timeStep;}
	/// 積分ステップを設定する
	void SetTimeStep(double dt);
	///	1ステップあたりの減衰率の取得
	double GetVelocityLossPerStep(){ return velocityLossPerStep; }
	///	減衰率の取得
	double GetVelocityLoss();
	///	減衰率の設定
	void SetVelocityLoss(double dt);
	/// カウント数を返す
	unsigned GetCount()const{return count;}
	/// カウント数を設定する
	void SetCount(unsigned c){count = c;}
	///	シーンの時刻を進める ClearForce(); GenerateForce(); Integrate(); と同じ
	void Step();
	///	シーンの時刻を進める（力のクリア）
	void ClearForce();
	///	シーンの時刻を進める（力の生成）
	void GenerateForce();
	///	シーンの時刻を進める（力と速度を積分して，速度と位置を更新）
	void Integrate();
	///	レンダリング
	void Render();
	///	ロード後，参照の解決が終わってから呼び出す関数．各オブジェクトのLoadedを呼び出す．
	void Loaded(SGScene* scene);
	///	シーンを空にする．
	void Clear();
	///	名前表から，参照されていないオブジェクトを削除する．
	void ClearName();
	///	デバッグ用
	void Print(std::ostream& os) const { names.Print(os); }
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates();

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
protected:
//	friend class SGFrame;
	friend class SGObject;
};

}
#endif
