#ifndef SG_OBJECT_H
#define SG_OBJECT_H

#include <Base/BaseUtility.h>
#include <Base/BaseTypeInfo.h>
#include <Base/Affine.h>
#include <FileIO/FIDocNode.h>
#include <WinBasis/WBWin.h>


namespace Spr {;

///	SGObject を継承したクラスはメンバとしてこのマクロを持つ．
#define SGOBJECTDEF(cls)				DEF_UTTYPEINFODEF(cls)
#define SGOBJECTDEFABST(cls)			DEF_UTTYPEINFOABSTDEF(cls)
#define SGOBJECTIMPBASE(cls)			DEF_UTTYPEINFO(cls)
#define SGOBJECTIMPBASEABST(cls)		DEF_UTTYPEINFOABST(cls)
#define SGOBJECTIMP(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define SGOBJECTIMP2(cls, b1, b2)		DEF_UTTYPEINFO2(cls, b1, b2)
#define SGOBJECTIMP3(cls, b1, b2, b3)	DEF_UTTYPEINFO3(cls, b1, b2, b3)
#define SGOBJECTIMPABST(cls, base)		DEF_UTTYPEINFOABST1(cls, base)
#define SGOBJECTIMPABST2(cls, b1, b2)	DEF_UTTYPEINFOABST2(cls, b1,b2)
#define SGOBJECTIMPABST3(cls, b1, b2, b3)	DEF_UTTYPEINFOABST3(cls, b1,b2,b3)

class SGScene;

/**	シーングラフのオブジェクト型．
	クラス名を返す関数を持つ．	*/
class SPR_DLL SGObject:public UTRefCount, UTTypeInfoBase{
	UTString name;			///<	名前
	UTString nameSpace;		///<	名前空間＝ファイル名
protected:
	friend class SGObjectNames;
public:
	///	クラス名の取得などの基本機能の実装
	SGOBJECTDEF(SGObject);
	///	このオブジェクトの元となったドキュメント
	UTRef<FIDocNodeBase> doc;
	///	ユーザが自由に使えるポインタ
	void* userData;

	///
	SGObject(): userData(NULL) {}
	///	デストラクタをvirtualに
	virtual ~SGObject();

	///	名前の取得
	const char* GetName() const { return name.c_str(); }
	///	名前空間の取得
	const char* GetNameSpace() const { return nameSpace.c_str(); }
	///	名前の設定
	void SetName(const char* n, SGScene* s);
	///	関連付けられたドキュメントノードを開放
	virtual void ReleaseDoc(){ doc = NULL; }

	///	ドキュメントからのロード時，参照が解決したあとで呼び出される．
	virtual void Loaded(SGScene* scene){}
	///	デバッグ用の表示
	virtual void Print(std::ostream& os) const;

	///	所有しているオブジェクトの数
	virtual size_t NChildObjects(){ return 0; }
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i){ return NULL; }
	///	参照しているオブジェクトの数
	virtual size_t NReferenceObjects(){ return 0; }
	///	参照しているオブジェクト
	virtual SGObject* ReferenceObject(size_t i){ return NULL; };
	///	子オブジェクトの追加(所有・参照を問わない)
	virtual bool AddChildObject(SGObject* o, SGScene* s){ return false; }
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s){ return false; }
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates();
};

///	SGObjectのポインタの配列
class SGObjects:public UTStack< UTRef<SGObject> >{
public:
	///	Tの派生クラスをスタックのTop側から探す．
	template <class T> bool Find(T*& t){
		for(iterator it = end(); it != begin();){
			--it;
			T* tmp = DCAST(T, *it);
			if (tmp){
				t = tmp;
				return true;
			}
		}
		return false;
	}
};

}
#endif SG_OBJECT_H
