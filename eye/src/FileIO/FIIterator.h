#ifndef FIITERATOR_H
#define FIITERATOR_H
#include <Base/BaseDebug.h>
#include <string>
#include <vector>

namespace Spr{;

///	FileIOで使う文字列型
typedef std::string FIString;
class FIDocNodeBase;
/**	抽象ツリー(FIDocNodeBase)のためのイタレータの実装の基本クラス	*/
class FIIteratorImpBase{
public:
	///	複製の作成
	virtual FIIteratorImpBase* Clone() const=0;
	///	イタレータを進める．
	virtual void Next()=0;
	///	比較
	virtual bool Equal(const FIIteratorImpBase* imp) const =0;

	//@{	中身へのアクセス
	///	idを取得
	virtual FIString GetId() const = 0;
	///	データ属性の型を取得
	virtual FIString GetType() const =0;
	///	データ属性の1要素のサイズを取得
	virtual size_t GetElementSize() const =0;
	///	データ属性の要素数を取得
	virtual size_t GetNElement() const =0;
	///	データ属性を設定する．szには，実際にセットしたサイズが返る．
	virtual bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type)=0;
	///	データ属性を取得する．szには，実際に読み出したサイズが返る．
	virtual bool GetDataImp(void* data, size_t& sz)=0;
	///	ノードを設定する
	virtual bool SetNode(FIDocNodeBase* node)=0;
	///	ノードを取得する
	virtual FIDocNodeBase* GetNode()=0;
	//@}
};

/**	抽象ツリー(FIDocNodeBase)のためのイタレータ
	ノードは，複数の属性を持つ．属性はキーと値からなる．
	値はデータかノードの配列を持つ．
	イタレータは配列を１要素ずつ進む．
*/
class FIIterator{
public:
	///	実装
	FIIteratorImpBase* imp;
	///
	FIIterator(FIIteratorImpBase* i){ imp=i; }
	FIIterator(){ imp=NULL; }
	~FIIterator(){ delete imp; }
	FIIterator(const FIIterator& i){ imp=i.imp->Clone(); }
	FIIterator& operator =(const FIIterator& i){
		delete imp;
		imp=i.imp->Clone();
		return *this;
	}
	///@name ポインタ演算
	//@{
	///
	FIIterator operator ++(){
		FIIterator rv(imp->Clone());
		imp->Next();
		return rv;
	}
	///
	FIIterator& operator ++(int){
		imp->Next();
		return *this;
	}
	///
	FIIterator operator +(int n){
		assert(n >= 0);
		FIIterator rv(imp->Clone());
		for(int i=0; i<n; ++i) imp->Next();
		return rv;
	}
	///
	bool operator == (const FIIterator& it) const {
		return imp->Equal(it.imp);
	}
	///
	bool operator != (const FIIterator& it) const {
		return !imp->Equal(it.imp);
	}
	//@}

	//@{	中身へのアクセス
	///	属性のidを取得
	FIString GetId() const { return imp->GetId(); }
	///	データ属性の型を取得
	FIString GetType() const { return imp->GetType(); }
	///	データ属性の1要素のサイズを取得
	size_t GetElementSize() const { return imp->GetElementSize(); }
	///	データ属性の要素数を取得
	size_t GetNElement() const { return imp->GetNElement(); }
	///	データ属性を設定する．szには，実際にセットしたサイズが返る．
	bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type){ return imp->SetDataImp(data, sz, esz, id, type); }
	bool SetDataImp(const void* data, size_t& sz){ return imp->SetDataImp(data, sz, 0, "", ""); }
	template <class T>
	bool SetData(const T& t, size_t esz, FIString id, FIString type){ return imp->SetDataImp(&t, sizeof(T), esz, id, type); }
	template <class T>
	bool SetData(const T& t){ return imp->SetDataImp(&t, sizeof(T), 0, "", ""); }
	///	データ属性を取得する．szには，実際に読み出したサイズが返る．
	bool GetDataImp(void* data, size_t& sz) const { return imp->GetDataImp(data, sz); }
	template <class T>
	bool GetData(T& t){
		unsigned sz = sizeof(T);
		bool rv = imp->GetDataImp(&t, sz);
		assert(sz == sizeof(T));
		return rv;
	}
	///	ノードを設定する
	bool SetNode(FIDocNodeBase* node){ return imp->SetNode(node); }
	///	ノードを取得する
	FIDocNodeBase* GetNode(){ return imp->GetNode(); }
	const FIDocNodeBase* GetNode() const{ return ((FIIteratorImpBase*)imp)->GetNode(); }
};


#if 0
/**	抽象ツリーのためのイタレータ	
	実装をコピーせずに参照する．	*/
struct FIRefIterator: public FIIterator{
	FIRefIterator(FIIteratorImpBase* i){ imp=i; }
	~FIRefIterator(){ imp=NULL; }
	FIRefIterator(const FIIterator& i){ imp=i.imp; }
	FIRefIterator& operator =(const FIIterator& i){
		imp=i.imp;
		return *this;
	}
};

/**	ノードの属性イタレータ	*/
template <class NODE>
class FIAttrIteratorImp:public FIIteratorImpBase{
public:
	typedef NODE::FIAttrIterator pos;
	void Next(){ ++pos; }
};

/**	値をもつ属性を巡回するイタレータの実装	*/
template <class NODE>
class FIValueIteratorImp:public FIIteratorImpBase{
public:
	struct FIStackFrame{
		NODE* node;
		NODE::FIAttrIterator pos;
		FIStackFrame(NODE* n):node(n){ pos = node->Begin(); }
	};
	typedef std::vector<FIStackFrame> FINodeStack;
protected:
	FINodeStack nodeStack;
public:
	virtual void Next(){
		++ nodeStack.back().pos;
		while(1){
			if (nodeStack.back().pos == nodeStack.back().node->End()){
				if (nodeStack->size()==0) return;	//	End() を指す．
				nodeStack->pop_back();
				++ nodeStack.back().pos;
			}
			NODE* attrNode = nodeStack.back().node->GetAttrNode(FIRefIterator(this));
			if (attrNode){
				nodeStack.push_back(FIStackFrame(attrNode));
			}else{
				return;	//	値を持つ属性を指す
			}
		}
	}
};
#endif

}	//	namespace Spr
#endif
