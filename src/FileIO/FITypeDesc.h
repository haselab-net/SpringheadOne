#ifndef FITYPEDESC_H
#define FITYPEDESC_H

#include "FIIterator.h"
#include <Base/BaseUtility.h>
#include <WinBasis/WBUtility.h>
#include <set>
#include <map>
#include <algorithm>

/**	@page FITypeDesc ドキュメントオブジェクトと型記述
	C++の構造体からデータを書き出す場合など，構造体やデータには，
	変数名や変数の型と言った情報はない．
	そこで，型記述型オブジェクト(FITypeDesc)を使って記述しておく．

	データを書き出す際には，バイナリデータから
	ドキュメントオブジェクトのプロパティを作る．
	この際，型記述を見てプロパティ名やプロパティ型を記入する．

	データを読み出す際には，型記述を見ながら，バイナリデータから
	構造体を構築する．

	これを実現するためのインタフェースとして，
	- 読み出しコンテキストの作成		FIContext* ReadContext();
	- 書き込みコンテキストの作成		FIContext* WriteContext();
	- コンテキストの開放				void ReleaseContext(FIContext*);
	- 次のフィールドを読み出す関数		bool Read(void* ctx, void* ptr, int sz)
	- 次のフィールドのデータを書き込む	void Write(void* ctx, void* ptr, int sz, FIString fn)
	- フィールドのグループに入る		void EnterGroup(FIString fn);
	- グループから抜ける．				void ExitGroup();
	を用意する．
*/

namespace Spr {;

/**	FIVVectorの基本クラス．
	ベクタへのロードとセーブのサポートに必要な仮想関数を用意	*/
class FIVVectorBase{
public:
	virtual size_t VItemSize() = 0;
	virtual size_t VSize() = 0;
	virtual void VResize(size_t s) = 0;
	virtual char* VBegin() = 0;
	virtual char* VEnd() = 0;
	virtual void VInsert(int pos, void* v) = 0;
	virtual void VPushBack(void* v) = 0;
	virtual void VPushBack() = 0;
	virtual void VPopBack() = 0;
};
/**	FITypeDesc が理解できるstd::vector
	ベクタへのロードとセーブのサポートに必要な仮想関数が実装される．
	FITypeDesc は普通のvector を含むクラスはサポートしない．*/
template <class T>
class FIVVector: public FIVVectorBase, public std::vector<T>{
	virtual size_t VItemSize(){ return sizeof(T); }
	virtual size_t VSize(){ return size(); }
	virtual void VResize(size_t s){ resize(s); }
	virtual char* VBegin(){ return (char*)&*begin(); }
	virtual char* VEnd(){ return (char*) &*end(); }
	virtual void VInsert(int pos, void* v){ insert(begin()+pos, *(T*)v); }
	virtual void VPushBack(void* v){ push_back(*(T*)v); }
	virtual void VPushBack(){ push_back(T()); }
	virtual void VPopBack(){ pop_back(); }
public:
	FIVVector(){}
	FIVVector(const std::vector<T>& v):std::vector<T>(v){}
	FIVVector& operator=(const std::vector<T>& v){ *(std::vector<T>*)this = v; return *this; }
};

class FIDocNodeBase;
class FIDocNodePos;

class FITypeDescDb;
///	型を表す
class SPR_DLL FITypeDesc:public UTRefCount{
public:
	///	レコードのフィールドを表す
	class SPR_DLL Field{
	public:
		Field(): length(1), lengthFieldOffset(0), lengthFieldSize(0), bReference(false), bVector(false){}
		~Field();
		///	メンバ名
		UTString name;
		///	型
		UTRef<FITypeDesc> type;
		///	配列の要素数．
		int length;
		///	要素数を別のフィールドからとる場合のフィールド名
		UTString lengthFieldName;
		///	要素数を別のフィールドからとる場合のオフセット
		int lengthFieldOffset;
		///	要素数を別のフィールドからとる場合サイズ
		int lengthFieldSize;
		///	vectorかどうか
		int bVector;
		///	参照かどうか
		bool bReference;
		///	データのサイズ
		size_t Size();
		///
		void Print(std::ostream& os) const;
		///	ノードから，構造体のフィールドを読み出す
		bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
		///	ノードに構造体のフィールドを書き出す．
		void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from);
	};
	///	組み立て型をあらわす場合に使う
	class SPR_DLL Composit: public std::vector<Field>{
	public:
		///	データのサイズを返す
		int Size(){ return Size(""); }
		///	データのサイズを返す
		int Size(FIString id);
		///
		void Print(std::ostream& os) const;
		///	ノードから，構造体を読み出す
		bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
		///	ノードに構造体を書き出す．
		void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from);
	};
protected:
	///	型名
	UTString typeName;
	///	データの長さ
	size_t size;
	///	組み立て型の中身の記述．単純型の場合は，size() == 0
	Composit composit;

	friend class FITypeDescDb;
public:
	///	型のGUID
	WBGuid guid;
	///	コンストラクタ
	FITypeDesc():size(0){}
	///	コンストラクタ
	FITypeDesc(UTString tn, int sz=0): typeName(tn), size(sz){}
	///	
	~FITypeDesc(){}
	///	C++言語のクラス定義部の文字列から，その型を表すオブジェクトを構築する．
	bool Init(const char* clsDef, FITypeDescDb& db);
	///
	void Print(std::ostream& os) const;
	///	ノードから，構造体を読み出す
	int Read(void* to, FIDocNodeBase* node, FIString id);
	int Read(void* to, FIDocNodeBase* node){ return Read(to, node, ""); }
	///	属性から構造体を読み出す
	bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
	///	ノードへ，構造体を書き出す
	int Write(FIDocNodeBase* node, const void* from);
	///	属性へ，構造体を書き出す
	void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from, UTString id);
	///	型名
	UTString GetTypeName() const { return typeName; }
	///	GUID
	const GUID& GetGuid() const { return guid; };
	///	型のサイズ
	size_t Size() { return size; }
	///	組み立て型の要素
	Composit& GetComposit(){ return composit; }
};
inline bool operator < (const FITypeDesc& d1, const FITypeDesc& d2){
	return d1.GetTypeName().compare(d2.GetTypeName()) < 0;
}
///	型のデータベース
class SPR_DLL FITypeDescDb: public UTRefCount{
public:
	///	コンテナの型
	typedef std::set< UTRef<FITypeDesc>, UTContentsLess< UTRef<FITypeDesc> > > Db;
	struct ProtoDesc{
		FIString fileType;
		UTRef<FITypeDesc> desc;
	};
	typedef std::vector<ProtoDesc> ProtoDescs;
protected:
	FIString prefix;			///<	名前のうちプレフィックスの部分
	Db db;						///<	FITypeDesc を入れておくコンテナ
	static ProtoDescs protoDescs;
public:
	///	
	~FITypeDescDb();
	static const ProtoDescs& GetProtoDescs(){ return protoDescs; }
	/**	型情報をデータベースに登録．	*/
	void RegisterDesc(FITypeDesc* n){
		if (prefix.length() && n->typeName.compare(0, prefix.length(), prefix) == 0){
			n->typeName = n->typeName.substr(prefix.length());
		}
		db.insert(n);
	}
	/**	型情報をプロトタイプリストに登録	*/
	void RegisterProto(FITypeDesc* n);
	/**	REG_RECORDマクロが使用する関数．型名と構造体の定義部分の文字列
		から型情報を生成し，データベースに登録．	*/
	void Register(const char* type, const char* def, bool bProto=false){
		UTRef<FITypeDesc> desc = new FITypeDesc(type);
		desc->Init(def, *this);
		RegisterDesc(desc);
		if(bProto) RegisterProto(desc);
	}
	/**	型名のPrefix を設定．
		型名をFindで検索する際に，検索キーにPrefixをつけたキーでも型名を検索する．	*/
	void SetPrefix(FIString p);
	/**	型情報を名前から検索する．
		@param tn	型名．prefix は省略してよい．	*/
	FITypeDesc* Find(UTString tn);
	///	DB内の型情報の表示
	void Print(std::ostream& os) const ;
};

///	FIVVectorのサイズ指定．x にはフィールド名(メンバ変数名)を指定する．
#define VSIZE(x)

///	単純型を登録する．
#define REG_FIELD(type)	RegisterDesc( new FITypeDesc(#type, sizeof(type)) )
/**	ドキュメントからロード可能なクラスの定義．
	クラスが基本クラスを持つ場合	*/
#define DEF_RECORDBASE(Type, Base, Def)					\
	struct Type: public Base Def;						\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	ドキュメントからロード可能なクラスの定義．
	基本クラスが無い場合	*/
#define DEF_RECORD(Type, Def)							\
	struct Type Def;									\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	DER_RECORD で定義した型を登録する．*/
#define REG_RECORD(Type)	\
	Register(classNameOf##Type, classDefOf##Type)

/**	DER_RECORD で定義した型をプロトタイプとしても登録する．*/
#define REG_RECORD_PROTO(Type)	\
	Register(classNameOf##Type, classDefOf##Type, true)

/**	@page TypeDesc 型説明クラス
C++の構造体宣言を文字列として受け取り，型を解析する．
使い方：ヘッダファイルで，定義
DEF_RECORD( Test, {
	int x;
	char y;
}
)

を書いて，クラスの定義を行い，
.cppファイルの何かの関数の中で
	REG_RECORD(Test);
を書いて登録します．
*/
}
#endif
