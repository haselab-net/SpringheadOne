#ifndef _FIDOCONVERTER
#define _FIDOCONVERTER
#include "FIDocNode.h"
#include "FITypeDesc.h"
#include <set>

/**	@file FIDocConverter.h ドキュメントを別のドキュメントに変換する	*/
namespace Spr{;

class FIDocConverter;

/**	変換のテンプレート	*/
class FIDocConvBase: public UTRefCount{
public:
	//@name ノード・フィールドのマッチング．
	//@{
	///	ノードの型名
	virtual UTString GetType() const = 0;
	//@}

	///	前順序変換動作
	virtual void Pre(FIDocConverter* ctx){}
	///	後順序変換動作
	virtual void Post(FIDocConverter* ctx){}
};
inline bool operator < (const FIDocConvBase& d1, const FIDocConvBase& d2){
	return d1.GetType().compare(d2.GetType()) < 0;
}

/**	属性の移動	*/
class FIDocConvMove: public FIDocConvBase{
public:
	///	移動先のフィールド名
	FIString fieldTo;
	///	前順序変換動作
	virtual void Pre(FIDocConverter* ctx);
};

/**	ドキュメントのコンバータ	*/
class FIDocConverter{
protected:
	UTRef<FIDocNodeBase> from, to, root;
	typedef std::multiset< UTRef<FIDocConvBase> , UTContentsLess< UTRef<FIDocConvBase> > > Convs;
	Convs convs;
	UTRef<FITypeDescDb> typeDb;
public:
	///	タイプDBの取得
	FITypeDescDb* GetTypeDb(){ return typeDb; }
	///	タイプDBの設定	
	void SetTypeDb(FITypeDescDb* t) { typeDb = t; }
	///	変換元のルートノードを渡し，変換を行う．
	void Convert(FIDocNodeBase* r);
	///	変換フィルタの追加
	void Register(FIDocConvBase* f);
public:
	FIDocNodeBase* GetRoot(){ return root; }
	///	現在変換中の変換元ノード
	FIDocNodeBase* From();
	///	現在変換中の変換先ノード
	FIDocNodeBase* To();
	///	現在変換中の変換先ノードの移動
	void ChangeTo(FIString t);
	///	現在変換中の変換先ノードの移動
	void UpTo();
	///	ノードの作成
	virtual FIDocNodeBase* CreateNode(FIString t);
};






}	//	namespace Spr
#endif
