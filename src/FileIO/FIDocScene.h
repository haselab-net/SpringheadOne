#ifndef FIDOCSCENE_H
#define FIDOCSCENE_H
#include "FIDocNode.h"
#include "FIFileDoc.h"
#include "FITypeDesc.h"
#include <SceneGraph/SGScene.h>

/**	@file FIDocScene.h ドキュメントからシーングラフをロードする仕組み	*/
namespace Spr{;

/**	ある型のドキュメントノードのローダー	*/
class SPR_DLL FIBaseLoader:public UTRefCount{
public:
	///	ローダが担当する型名
	virtual UTString GetNodeType() const =0;
	///	ロード関数．子ノードがロードされる前に呼ばれる．
	virtual void Load(class FILoadScene* context)=0;
	///	ロード後処理関数．子ノードがロードされた後呼ばれる．
	virtual void Loaded(class FILoadScene* context){}
};
template <class T, bool bAdd=true, bool bCont=true> class FIObjectLoader: public FIBaseLoader{
public:
	typedef T TLoaded;
	void Create(class FILoadScene* context, UTRef<TLoaded>& t){
		t = new TLoaded;
		t->SetName(context->docs.Top()->GetName().c_str(), context->scene);
	}
	///	ローダが担当するノードの型名．C++クラスの型名から接頭辞2文字をなくしたもの．
	virtual UTString GetNodeType() const{
		return TLoaded::GetTypeInfoStatic()->ClassName()+2;
	}
	///	ロード関数．子ノードがロードされる前に呼ばれる．
	virtual void Load(class FILoadScene* context){
		UTRef<TLoaded> t=NULL;
		Create(context, t);
		t->doc = context->docs.Top();
		t->SetName(t->doc->GetName().c_str(), context->scene);
		if (bAdd) context->AddObject(t);
		if (bCont) context->AddContainer(t);
		if (!LoadData(context, t)){
			if (bAdd){
				context->CancelAddObject();
			}
		}
	}
	///	ロードの実装
	virtual bool LoadData(class FILoadScene* context, TLoaded* t)=0;
};

template <class P=SGFrame> class FIOtherLoader: public FIBaseLoader{
public:
	///	ロード関数．子ノードがロードされる前に呼ばれる．
	virtual void Load(class FILoadScene* context){
		P* p=NULL;
		context->objects.Find(p);
		Load(context, p);
	}
	virtual void Load(class FILoadScene* context, P* p)=0;
	///	ロード後処理関数．子ノードがロードされた後呼ばれる．
};

inline bool operator < (const FIBaseLoader& t1, const FIBaseLoader& t2){
	return t1.GetNodeType().compare(t2.GetNodeType()) < 0;
}
///	各ノードのローダーのSet
class SPR_DLL FILoders:public std::multiset<FIBaseLoader*, UTContentsLess<FIBaseLoader*> >{
public:
	class FITypeLoaderKey:public FIBaseLoader{
		UTString* key;
	public:
		FITypeLoaderKey(UTString& k):key(&k){}
		virtual UTString GetNodeType() const{ return *key; }
		virtual void Load(FILoadScene* context){}
	};
	iterator LowerBound(UTString s){
		return lower_bound(&FITypeLoaderKey(s));
	}
	iterator UpperBound(UTString s){
		return upper_bound(&FITypeLoaderKey(s));
	}
	std::pair<iterator, iterator> EqualRange(UTString s){
		return equal_range(&FITypeLoaderKey(s));
	}
	void Erase(UTString s){
		std::pair<iterator, iterator> r = EqualRange(s);
		erase(r.first, r.second);
	}
};

class SPR_DLL FIReferenceBase:public UTRefCount{
protected:
	SGObject* Find(SGScene* s);
	///	リファレンス先の名前
	UTString name;
public:
	virtual ~FIReferenceBase(){}
	///	参照の解決
	virtual void SolveReference(SGScene* s)=0;
};
class SPR_DLL FIReferences:public std::vector< UTRef<FIReferenceBase> >{
};
/**	リファレンス保持クラス	*/
template <class T>
class SPR_DLL FIReference:public FIReferenceBase{
	///	リファレンス元ポインタのアドレス．ここを書き換える．
	UTRef<T>* reference;
	///	
public:
	FIReference(UTRef<T>& t, UTString n): reference(&t) { name=n; }
	///	参照の解決
	virtual void SolveReference(SGScene* s){
		T* t = (T*)Find(s);
		*reference = t;
//		reference->ForceOverWrite(t);
//		t->AddRef();
	}
};
template <class CT>
class SPR_DLL FIReferenceVector:public FIReferenceBase{
	///	リファレンス元ポインタのアドレス．ここを書き換える．
	CT* container;
	typedef TYPENAME CT::value_type T;
	int pos;
public:
	FIReferenceVector(CT& c, int p, UTString n): container(&c), pos(p) { name=n; }
	///	参照の解決
	virtual void SolveReference(SGScene* s){
		T t = T(UTRef<SGObject>(Find(s)));
		(*container)[pos] = t;
	}
};
template <class T>
FIReference<T>* createFIReference(UTRef<T>& t, UTString n){
	return new FIReference<T>(t, n);
}
template <class CT>
FIReferenceVector<CT>* createFIReference(CT& ct, int p, UTString n){
	return new FIReferenceVector<CT>(ct, p, n);
}

class SPR_DLL FILink: public SGObject{
public:
	SGOBJECTDEF(FILink);
	UTString linkName;
};
class SGObject;
class SPR_DLL FIObjectAdder{
public:
	///	追加先のオブジェクト
	SGObjects containers;
	///	追加するオブジェクト
	SGObjects objects;
	///	参照を解決
	void Link(SGScene* scene);
	void Add(SGObject* obj);
	void Add(UTString name);
};
class SPR_DLL FIObjectAdders:public UTStack<FIObjectAdder>{
public:
};

/**	ドキュメントノードのツリーを読んでシーングラフを作る．
	コンテキストも保持する．
	ドキュメントノードには，型があり型ごとにローダが用意できる．
	型ごとのローダーはFIBaseLoaderを派生させて作る．
*/
class SPR_DLL FILoadScene:public UTRefCount{
	///	型ごとのローダー
	FILoders loaders;
	/**	オブジェクトの追加リスト（参照解決後に追加する）
		追加すべきコンテナが途中で増えることがあるので，
		まずadderStackに入れておき，それをこちらに移す．*/
	FIObjectAdders adders;
	/**	オブジェクトの追加リストのスタック．ノードごとにPush,Popする．
		ひとつのノードが複数のローダを持つ場合，ロード中にコンテナが増えることがある．
		そこで，ロード中の追加リストと，ロード先のコンテナをひとまずここに入れておく．
		Popするときに，追加リストをadders に移す．*/
	FIObjectAdders adderStack;
	///	子ノードのためのコンテナリスト
	SGObjects containers;
public:
	///	@name 各ノードのローダが使うメンバ
	//@{
	///	もとのファイル名
	UTString fileName;
	///	ロード元のドキュメントのスタック
	FIDocNodes docs;
	///	ロード先シーン
	UTRef<SGScene> scene;
	///	ロード中に使うオブジェクトのスタック
	SGObjects objects;
	///	ロードしたオブジェクトを現在のコンテナに追加
	void AddObject(SGObject* obj);
	///	ロードしたリファレンスを現在のコンテナに追加
	void AddObject(UTString name);
	///	最後の追加をキャンセル
	void CancelAddObject();
	///	コンテナを追加
	void AddContainer(SGObject* cont){
		containers.Push(cont);
	}
	///	コンテナスタックをPush．成功するとtrueを返す．
	bool PushContainer();
	///	コンテナスタックをPop
	void PopContainer();
	//@}

	//@name	外部から呼び出す関数．
	//@{
	///	ドキュメントオブジェクトからシーングラフを作成
	virtual void Load(SGScene* s, SGObject* obj, FIFileDoc* fileDoc);
	virtual void Load(SGScene* s, FIFileDoc* fileDoc);
	virtual void Load(SGScene* s, SGObject* obj, FIDocNodes& docs);
	virtual void Load(SGScene* s, FIDocNodes& docs);
	///	ローダーを登録
	void Register(FIBaseLoader* l){ loaders.insert(l); }
	///	ローダーを削除
	void Erase(UTString k){ loaders.Erase(k); }
	///	ドキュメントオブジェクトからシーングラフを作成
	virtual void LoadRecursive();
	//@}


protected:
	///	参照の解決．
	void SolveReferences();
	///	オブジェクトの追加．
	void AddObjects();
};

///	ある型(SGFrame/Visual/..) を保存するセーバー
class FIBaseSaver:public UTRefCount{
public:
	virtual UTString GetType() const =0;
	virtual UTString GetNodeType() const { return GetType().substr(2); }
	virtual void Save(class FISaveScene* ctx, SGObject* target){}
};
inline bool operator < (const FIBaseSaver& t1, const FIBaseSaver& t2){
	return t1.GetType().compare(t2.GetType()) < 0;
}
///	SGObjectのためのセーバー
template <class T, bool bAdd=true, bool bCont=true> class FIObjectSaver: public FIBaseSaver{
public:
	typedef T TSaved;
	///	型名
	virtual UTString GetType() const {
		return TSaved::GetTypeInfoStatic()->ClassName();
	}
	///	データのセーブ．オーバーライドしてください．
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, TSaved* target)=0;
	///	セーブ
	virtual void Save(class FISaveScene* ctx, SGObject* target){
		TSaved* t = (TSaved*) target;
		UTRef<FIDocNodeBase> doc = t->doc;
		if (!doc) doc = ctx->CreateDocNode(GetNodeType(), t);
        doc->SetName(target->GetName());
		doc->ClearChildren();
		SaveData(ctx, doc, t);
		ctx->AddSavedObject(target);
		if (bAdd) ctx->docs.Top()->AddChild(doc);
		if (bCont){
			ctx->docs.Push(doc);
			for(unsigned i=0; i<t->NChildObjects(); ++i){
				if(ctx->IsSavedObject(t->ChildObject(i))){
					ctx->SaveReference(t->ChildObject(i));
				}else{
					ctx->SaveRecursive(t->ChildObject(i));
				}
			}
			for(unsigned i=0; i<t->NReferenceObjects(); ++i){
				ctx->SaveReference(t->ReferenceObject(i));
			}
			ctx->docs.Pop();
		}
	}
};

///	ある型(SGFrame/Visual/..) を保存するセーバーのSet
class FISavers:public std::set<FIBaseSaver*, UTContentsLess<FIBaseSaver*> >{
public:
	class FITypeSaverKey:public FIBaseSaver{
		UTString* key;
	public:
		FITypeSaverKey(UTString& k):key(&k){}
		virtual UTString GetType() const{ return *key; }
		virtual void Save(FISaveScene* context, SGObject* target){}
	};
	iterator Find(UTString s){
		return find(&FITypeSaverKey(s));
	}
};
/**	シーングラフをドキュメントにセーブするセーバー
*/
class SPR_DLL FISaveScene:public UTRefCount{
protected:
	///	型ごとのセーバー
	FISavers savers;
	///	ドキュメントノードの生成
	virtual UTRef<FIDocNodeBase> CreateDocNodeImp(UTString type)=0;
public:
	///	セーブするシーン
	UTRef<SGScene> scene;
	///	ドキュメントノードのスタック
	FIDocNodes docs;
	///	セーブ済みオブジェクトの記録
	std::set<SGObject*> savedObjects;
	///	ドキュメントノードの作成
	virtual UTRef<FIDocNodeBase> CreateDocNode(UTString type, SGObject* o=NULL){
		UTRef<FIDocNodeBase> doc = CreateDocNodeImp(type);
		if (o){
			doc->SetName(o->GetName());
			if (type.compare("REF")!=0) o->doc = doc;
		}
		return doc;
	}
	///	シーングラフの特定のノード以下をセーブ
	virtual void Save(FIDocNodeBase* doc, SGScene* s, SGObject* obj=NULL);
	///	Saveの再帰部
	virtual void SaveRecursive(SGObject* obj);
	///	参照をセーブ
	virtual void SaveReference(SGObject* obj);
	
	///	特定の型のセーバーを登録
	void Register(FIBaseSaver* s){ savers.insert(s); }
	///	セーブ済みオブジェクトの登録
	void AddSavedObject(SGObject * o){
		if (!savedObjects.insert(o).second){
			DSTR << "Fail to insert " << o->GetTypeInfo()->ClassName() << ":" << o->GetName() << std::endl;
		}
	}
	///	セーブ済みオブジェクトかどうか判定
	bool IsSavedObject(SGObject * o){
		return savedObjects.find(o) != savedObjects.end();
	}
};

#define DEF_REGISTER_LOADER(cls)				\
void RegisterLoaderFor##cls(FILoadScene* l){	\
	static cls##Loader load_##cls;				\
	l->Register(&load_##cls);					\
}

#define DEF_REGISTER_SAVER(cls)				\
void RegisterSaverFor##cls(FISaveScene* s){	\
	static cls##Saver save_##cls;			\
	s->Register(&save_##cls);				\
}

#define DEF_REGISTER_BOTH(cls)	DEF_REGISTER_SAVER(cls)	DEF_REGISTER_LOADER(cls)

#define REGISTER_LOADER(l, cls)									\
	extern void RegisterLoaderFor##cls(FILoadScene* ld);		\
	RegisterLoaderFor##cls(l);									\

#define REGISTER_SAVER(s, cls)									\
	extern void RegisterSaverFor##cls(FISaveScene* sv);			\
	RegisterSaverFor##cls(s);									\

}
#endif
