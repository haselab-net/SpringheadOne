#ifndef FIDOCUMENT_H
#define FIDOCUMENT_H

#include "FITypeDesc.h"
#include <Base/BaseUtility.h>
#include <set>
#include <map>
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

/**	ドキュメントノードの読み出し・書き込み位置を保持するコンテキスト	*/
class FIDocNodePos{
public:
	virtual ~FIDocNodePos(){}
};
/**	ファイル入出力ノード．
	シーングラフをファイルからロードしたり，ファイルにセーブする際に，
	ファイルを直接扱わずに，ドキュメントオブジェクトを介してセーブ・
	ロードすると，ファイルの扱いをドキュメントオブジェクトに任せる
	ことができるので，セーブ・ロードのコードを減らすことができる．
	このクラスは，ドキュメントオブジェクトを構成するノードの基本クラス.
*/
class SPR_DLL FIDocNodeBase:public UTRefCount{
public:
	friend class FISaveScene;
	///	仮想デストラクタを用意
	virtual ~FIDocNodeBase(){}
	//@name	ノードの属性
	//@{
	///	このノードの名前の取得
	virtual UTString GetName()=0;
	///	このノードの名前の設定
	virtual void SetName(UTString s)=0;
	///	このノードの型名の取得
	virtual UTString GetType()=0;
	///	このノードの型名の設定
	virtual void SetType(UTString t){}
	///	このノードの型データの取得
	virtual FITypeDesc* GetTypeDesc()=0;
	///	このノードがシーングラフ内の他のノードへの参照かどうか
	virtual bool IsReference() = 0;
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
	///	子ノードを型名で検索
	virtual FIDocNodeBase* FindChild(UTString type)=0;
	//@}

	///	データを設定
	virtual void SetDataImp(const void* data, size_t sz);
	///	データを取得
	virtual void GetDataImp(void* data, int sz, FIString id);
	virtual void GetDataImp(void* data, int sz){ GetDataImp(data, sz,""); }
	///	データを設定(テンプレート版)
	template <class T> void SetData(const T& t){ SetDataImp(&t, sizeof(t)); }
	///	データを取得(テンプレート版)
	template <class T> void GetData(T& t){ GetDataImp(&t, sizeof(t), ""); }
	template <class T> void GetData(T& t, UTString id){ GetDataImp(&t, sizeof(t), id); }
	///	読み出しコンテキストの作成
protected:
	virtual FIDocNodePos* ReadContextImp(FIString id)=0;
public:
	FIDocNodePos* ReadContext(){ return ReadContextImp(""); }
	FIDocNodePos* ReadContext(FIString id){ return ReadContextImp(id); }
	///	書き込みコンテキストの作成
protected:
	virtual FIDocNodePos* WriteContextImp(FIString id){ return ReadContext(id); }
public:
	FIDocNodePos* WriteContext(FIString id){ return WriteContextImp(id); }
	FIDocNodePos* WriteContext(){ return WriteContextImp(""); }
	///	コンテキストの開放
	virtual void ReleaseContext(FIDocNodePos* c){ delete c; }
	///	次のフィールドを読み出す関数
	virtual bool Read(FIDocNodePos* ctx, char* ptr, int sz)=0;
	///	次のフィールドのデータを書き込む	
	virtual void Write(FIDocNodePos* ctx, const char* ptr, int sz, FIString fn, FIString type)=0;
	///	フィールドのグループに入る		
	virtual void EnterGroup(FIString fn){}
	///	グループから抜ける．				
	virtual void ExitGroup(){}
	
	///	ノードのツリーを表示
	virtual void Print(std::ostream& os, int indent=0) const;
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
	UTString name;
	UTRef<FITypeDesc> typeDesc;
	bool bReference;
	std::vector<char> data;
public:
	typedef UTTreeNode<FIDocNode> Tree;
	///
	FIDocNode(): bReference(false){}
	///
	~FIDocNode(){}
	///	型の設定
	void SetType(FITypeDesc* t);
	void SetType(FIString s){ FIDocNodeBase::SetType(s); }
	//@name	ノードの属性
	//@{
	///	このノードの名前の取得
	virtual UTString GetName(){ return name; }
	///	このノードの名前の設定
	virtual void SetName(UTString s){ name = s; }
	///	このノードの型名の取得
	virtual UTString GetType();
	///	このノードの型データの取得
	virtual FITypeDesc* GetTypeDesc(){ return typeDesc; }
	///	このノードがシーングラフ内の他のノードへの参照かどうか
	virtual bool IsReference(){ return bReference; }
	//@}
	
	//@name	木構造の操作
	//@{
	///	親ノード
	virtual FIDocNodeBase* GetParent(){ return Tree::GetParent(); }
	///	子ノードの数
	virtual int NChildren() const { return (int)Tree::Children().size(); }
	///	親ノードの設定
	virtual void SetParent(FIDocNodeBase* n){ Tree::SetParent((FIDocNode*)n); }
	///	子ノードをすべて削除
	virtual void ClearChildren(){ Tree::ClearChildren(); }
	///	子ノードを型名で検索
	virtual FIDocNodeBase* FindChild(UTString type);
	//@}

	///	読み出しコンテキストの作成
	virtual FIDocNodePos* ReadContextImp(FIString id);
	///	次のフィールドを読み出す関数
	virtual bool Read(FIDocNodePos* ctx, char* ptr, int sz);
	///	次のフィールドのデータを書き込む	
	virtual void Write(FIDocNodePos* ctxBase, const char* ptr, int sz, FIString fn, FIString type);

protected:
	///	子ノードを返す関数の実装
	virtual FIDocNodeBase* Child(int i){ return Tree::Children()[i]; }
};

/**	ドキュメントとファイルの変換のためのコンテキスト．	*/
class SPR_DLL FIFileDoc:public UTRefCount{
protected:
	///	ファイル名
	UTString fileName;
	///	ドキュメントノードのスタック
	FIDocNodes docs;
public:
	///	ファイルからドキュメントオブジェクトを作成
	virtual bool Load(UTString filename)=0;
	///	ドキュメントオブジェクトに保存
	virtual bool Save(UTString filename, FIDocNodeBase* doc)=0;
	///	ドキュメントのルートノード
	virtual FIDocNodes& GetRoots(){ return docs; }
	///	ファイル名
	UTString GetFileName(){ return fileName; }
};

}
#endif
