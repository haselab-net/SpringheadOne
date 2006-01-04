#ifndef FIDOCNODE_H
#define FIDOCNODE_H

#include "FIIterator.h"
#include "FITypeDesc.h"
#include <Base/BaseUtility.h>
#include <list>
#include <algorithm>

namespace Spr {;

///	文字列の型
typedef std::string FIString;
///	長さ0の文字列
extern FIString zeroLengthString;
///	文字列の比較
struct FIStringLess{
	bool operator() (const FIString& s1, const FIString& s2) const { return s1.compare(s2) < 0;}
};
///	文字列の表示
inline std::ostream& operator << (std::ostream& os, FIString s){
	os << s.c_str();
	return os;
}




/*	DocNodeの設計について
	D3Dの場合，データファイル．VRMLはスクリプト．
	たとえば，VRMLの場合，orientation が 何度も出てくるような Transform ノード
	がありえる．
	Xの場合，順番が固定で，データだけが書いてある．

	XのDocNodeは，同じ名前のフィールドを1つだけ持つので，C++のクラスと1対1に対応する．
	VRMLのDocNodeは複数ありえて，処理が割り当たる．

	同じIFにするのはよいのか？
	→よい


	イタレータは，ノードを巡回するけれど，組み立て型はどうするか？
	Xは属性が組み立て型になることがある．
	VMRLは単純型だけ．
	Xだと
		MeshFace{ int n; int vtx[]; }
		Mesh{ MeshFace face; MeshMaterialList{} }
	があり．
	これをDocNodeで書くには？
		Mesh{
			face -> {
				int n;
				int vtx[];
			}
			children -> MeshMatrialList{
			}
		}


	組み立て型の中を巡回するなら属性の値として，ノード型をありにしなければならない．
	属性→ノード名
	ノード→Childrenに入れておく
	とする？
*/

class FITypeDesc;
/**	ファイル入出力ノード．
	シーングラフをファイルからロードしたり，ファイルにセーブする際に，
	ファイルを直接扱わずに，ドキュメントオブジェクトを介してセーブ・
	ロードすると，ファイルの扱いをドキュメントオブジェクトに任せる
	ことができるので，セーブ・ロードのコードを減らすことができる．
	このクラスは，ドキュメントオブジェクトを構成するノードの基本クラス.

	ドキュメントノードでは，属性値として，ノードを取ることが出来るようにしている．
	子ノードと属性値の両方にノードが来ることがあり得る．*/
class SPR_DLL FIDocNodeBase:public UTRefCount{
public:
	friend class FISaveScene;
	///	仮想デストラクタを用意
	virtual ~FIDocNodeBase(){}

	//@name	ノードの基本属性
	//@{
	///	このノードの名前の取得
	virtual UTString GetName() const =0;
	///	このノードの名前の設定
	virtual void SetName(UTString s)=0;
	/**	このノードの型名の取得.
		リファレンスについて：
		このノードが他のノードへのリファレンスだった場合，
		GetType() は "REF" を返し，GetName()は参照先のノード名を返す．*/
	virtual UTString GetType() const =0;
	/**	このノードの型名の設定.
		リファレンスの場合は， "REF"を設定する．*/
	virtual void SetType(UTString t){ assert(0); }
	///	型記述子の設定
	virtual void SetTypeDesc(FITypeDesc* desc){ assert(0); }
	bool IsReference() const { return GetType().compare("REF") == 0; }
	//@}
	
	//@name	木構造の操作
	//@{
	///	親ノード
	virtual FIDocNodeBase* GetParent()=0;
	///	子ノードの数
	virtual int NChildren() const =0;
	///	子ノード
	virtual FIDocNodeBase* GetChild(int i){ return Child(i); }
	///	子ノード(Const版)
	virtual const FIDocNodeBase* GetChild(int i) const { return ((FIDocNodeBase*)this)->Child(i); }
	///	子ノードの追加
	virtual void AddChild(FIDocNodeBase* n){ n->SetParent(this); }
	///	親ノードの設定
	virtual void SetParent(FIDocNodeBase*)=0;
	///	子ノードをすべて削除
	virtual void ClearChildren()=0;
	//@}

	//@name	属性の操作
	//@{
	/**	全属性を設定 */
	template <class T> bool SetWholeData(const T& t){ return SetWholeData(&t, sizeof(t)); }
	virtual bool SetWholeData(const void* data, size_t sz);
	///	属性全体を取得．
	template <class T> bool GetWholeData(T& t){ return GetWholeData(&t, sizeof(t)); }
	virtual bool GetWholeData(void* data, size_t sz);	
	
	///	指定のデータ属性の設定
	template <class T> bool SetData(const T& t, FIString id, FIString type){ return FindAttr(id).SetData(t, type); }
	///	指定のデータ属性の設定(配列の一括設定可)
	bool SetData(const void* data, size_t& dataSize, size_t elementSize, FIString id, FIString type){ return FindAttr(id).SetDataImp(data, dataSize, elementSize, id, type); }
	///	指定のデータ属性の取得
	template <class T> bool GetData(T& t, FIString id){ return FindAttr(id).GetData(t); }
	///	指定のデータ属性の取得
	bool GetData(void* data, size_t sz, FIString id){ return FindAttr(id).GetDataImp(data, sz); }
	//@}

	//@name	属性操作の実装
	//@{
	///	現在の属性に新しいノードをセットする．
	virtual FIDocNodeBase* SetNewNode(FIString id, FIString type)=0;
	
	///	先頭の属性値を指すイタレータを取得
	virtual FIIterator FirstAttr() const =0;
	///	最後の次の属性値を指すイタレータを取得
	virtual FIIterator LastAttr() const =0;
	///	イタレータが最後に来たら true
	virtual bool IsLast(const FIIterator& it) const { return it == LastAttr(); }
	///	指定のノードを指すイタレータを取得
	virtual FIIterator FindAttr(FIString id, size_t pos=0) const =0;
	//@}

	///	ノードのツリーを表示
	virtual void Print(std::ostream& os, int indent=0) const;
	///	属性を表示
	virtual void PrintAttr(std::ostream& os, int indent=0) const;
	///	子ノードのツリーを表示
	virtual void PrintChildren(std::ostream& os, int indent=0) const;
protected:
	///	子ノードを返す関数の実装
	virtual FIDocNodeBase* Child(int i) = 0;
};

///	ドキュメントノードの基本クラスのvector
class SPR_DLL FIDocNodes:public UTStack< UTRef<FIDocNodeBase> >{
public:
	virtual void Print(std::ostream& os, int indent=0) const;
};

///	ドキュメントノード
class SPR_DLL FIDocNode:public FIDocNodeBase, public UTTreeNode<FIDocNode>{
public:
	struct TData{
		typedef std::vector<char> container;
		FIString type;							///<	値の型
		size_t elementSize;						///<	要素のサイズ
		container data;							///<	バッファ
		TData(){ elementSize=0; }
		///	データを最後に追加
		void PushBack(const void* d){
			data.resize(data.size()+elementSize);
			container::iterator it = data.end()-elementSize;
			memcpy(&*it, d, elementSize);
		}
		///	最後のデータを削除
		void PopBack(){
			data.resize(data.size() - elementSize);
		}
		///	1要素のサイズ
		size_t ElementSize() const { return elementSize; }
		void ElementSize(size_t s){ elementSize = s; }
		///	データ数
		void Size(size_t n){ data.resize(n*elementSize); }
		size_t Size() const { return elementSize ? data.size()/elementSize : 0; }
		///	全データのバイト数
		size_t DataSize(){ return data.size(); }
		///	n番目のデータへのポインタ
		char* Data(size_t i){ return &*data.begin() + i*elementSize; }
		///	最初のデータへのポインタ
		char* Begin(){ return &*data.begin(); }
		///	最初のデータへのポインタ
		const char* Begin() const { return &*data.begin(); }
		///	最初のデータの次へのポインタ
		char* End(){ return &*data.end(); }
		///	最初のデータの次へのポインタ
		const char* End() const { return &*data.end(); }
		///	サイズの設定
		void Resize(size_t s){ data.resize(s*elementSize); }
	};
	///	ノードかバイナリデータの配列
	struct TValue{
		///	ノード値(バイナリ値がある場合は空)
		std::vector< UTRef<FIDocNode> > nodes;
		///	バイナリデータ
		TData data;
		///	ノードまたはデータの数．
		size_t Size() const {
			return nodes.size() ? nodes.size() : data.Size();
		}
	};
	struct TAttr: public TValue{
		UTString id;				///<	属性名
		TAttr(){}
		TAttr(FIString i):id(i){}
		bool operator < (const TAttr& b) const { return id.compare(b.id) < 0; }
		bool operator == (const TAttr& b) const { return id.compare(b.id)==0; }
	};
	class TAttrs:public std::vector<TAttr>{
	public:
		typedef std::vector<TAttr> base;
		TAttrs(){}
		TAttrs(size_t sz, const TAttr a):base(sz,a){}
		iterator Find(FIString id) const;
	};
	class TIteratorImp:public FIIteratorImpBase{
	public:
		TAttrs::iterator it;	//	属性へのポインタ
		size_t pos;				//	属性内での位置
		FIDocNode* node;

		TIteratorImp(FIDocNode* n, TAttrs::const_iterator i, size_t p):node(n), it((TAttrs::iterator&)i), pos(p){}
		virtual FIIteratorImpBase* Clone() const { return new TIteratorImp(*this); }
		virtual void Next(){
			if (pos < it->Size()) { ++pos; }
			else {
				if (Validate()) ++pos;
			}
		}
		bool Equal(const FIIteratorImpBase* bBase) const {
			const TIteratorImp* a = this;
			const TIteratorImp* b = (TIteratorImp*) bBase;
			return a->it == b->it && a->node == b->node && a->pos == b->pos;			
		}
		bool Validate() const;
//		bool ValidateNode();
//		bool ValidateData();
		//@{	中身へのアクセス
		///	idを取得
		virtual FIString GetId() const;
		///	データ属性の型を取得
		virtual FIString GetType() const;
		///	データ属性の1要素のサイズを取得
		virtual size_t GetElementSize() const;
		///	データ属性の要素数を取得
		virtual size_t GetNElement() const;
		///	データ属性を設定する．szには，実際にセットしたサイズが返る．
		virtual bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type);
		///	データ属性を取得する．szには，実際に読み出したサイズが返る．
		virtual bool GetDataImp(void* data, size_t& sz);
		///	ノードを設定する
		virtual bool SetNode(FIDocNodeBase* node);
		///	ノードを取得する
		virtual FIDocNodeBase* GetNode();
		//@}
	};

	///	基本クラス
	typedef UTTreeNode<FIDocNode> FITree;

public:
	TAttrs attrs;
	FIString name;
	FIString type;
public:
	///
	FIDocNode(){}
	///
	~FIDocNode(){}
	//@name	ノードの属性
	//@{
	///	このノードの名前の取得
	virtual UTString GetName() const { return name; }
	///	このノードの名前の設定
	virtual void SetName(UTString s){ name = s; }
	///	このノードの型名のせってい
	virtual void SetType(FIString t);
	///	このノードの型名の取得
	virtual FIString GetType() const;
	//@}
	
	//@name	木構造の操作
	//@{
	///	親ノード
	virtual FIDocNodeBase* GetParent(){ return FITree::GetParent(); }
	///	子ノードの数
	virtual int NChildren() const { return (int)FITree::Children().size(); }
	///	親ノードの設定
	virtual void SetParent(FIDocNodeBase* n){ FITree::SetParent((FIDocNode*)n); }
	///	子ノードをすべて削除
	virtual void ClearChildren(){ FITree::ClearChildren(); }
	///	子ノードを返す
	virtual FIDocNodeBase* Child(int i){ return FITree::Children()[i]; }
	//@}
	
	//@name	属性の操作
	//@{	
	virtual FIDocNodeBase* SetNewNode(FIString id, FIString type);
	virtual FIIterator FirstAttr() const;
	virtual FIIterator LastAttr() const;
	virtual FIIterator FindAttr(FIString id, size_t pos=0) const;
	virtual bool IsLast(const FIIterator& it) const;
	//@}
};
}
#endif
