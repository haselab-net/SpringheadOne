#ifndef SGENGINE_H
#define SGENGINE_H

#include "SGFrame.h"

namespace Spr {;

///	動作エンジンの基本クラス．
class SGEngine:public SGObject{
	SGOBJECTDEF(SGEngine);
public:
	///	シーンをクリアするときの処理
	virtual void Clear(SGScene* s){}
	///	プライオリティ	小さいほど優先順位が高い
	virtual int GetPriority() const { return 0; }
	///	オブジェクトをセットする
	virtual void Set(SGObject* obj){}
};
inline bool operator < (const SGEngine& e1, const SGEngine& e2){
	return e1.GetPriority() < e2.GetPriority();
}

class FISaveScene;

///	SGEngineの配列
class SGEngines: std DOUBLECOLON vector< UTRef<SGEngine> >{
protected:
	SGObjects objs;
	iterator begin2() { return base::begin(); }
	iterator end2() { return base::end(); }
public:
	typedef std::vector< UTRef<SGEngine> > base;
	typedef base::iterator iterator;
	typedef base::const_iterator const_iterator;
	const_iterator begin() const { return base::begin(); }
	const_iterator end() const { return base::end(); }
	size_t size(){ return base::size(); }
	UTRef<SGEngine>& operator [] (int id){ return base::begin()[id]; }
	const UTRef<SGEngine>& operator [] (int id) const { return base::begin()[id]; }
	virtual void Add(SGEngine* e);
	virtual bool Del(SGEngine* e);
	///
	SGEngines();
	///	セットされたオブジェクトたち
	SGObjects& GetObjects(){ return objs; }
	///	オブジェクトをセットする
	virtual void Set(SGObject* obj){
		for(const_iterator it = begin(); it != end(); ++it){
			(*it)->Set(obj);
		}
		objs.push_back(obj);
	}
	///	ロード後の処理
	virtual void Loaded(SGScene* scene);
	///	シーンをクリアするときの処理
	virtual void Clear(SGScene* s);
	///	指定した型を継承したエンジンを見つける (最初に見つかったものを返す)
	SGEngine* FindInherit(const UTTypeInfo& tinfo, UTString name) const;
	SGEngine* FindInherit(const UTTypeInfo& tinfo) const;
	///	指定した型かその派生クラスのエンジンを見つける (最初に見つかったものを返す)
	template <class T> bool Find(T*& t) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic());
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic()));
		return t != NULL;
	}
	///	指定した型かその派生クラスのエンジンで名前が一致するものを見つける (最初に見つかったものを返す)
	template <class T> bool Find(T*& t, UTString name) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic(), name);
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t, UTString name) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic(), name));
		return t != NULL;
	}
	void Save(FISaveScene* ctx);
};

}
#endif
