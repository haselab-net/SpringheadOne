#ifndef BASETYPEINFO_H
#define BASETYPEINFO_H
/**	実行時型情報テンプレートライブラリ
	コンパイラのrttiを使わずマクロとテンプレートで実装してある．
*/
namespace Spr{;

///	実行時型情報
class UTTypeInfo {
public:
	const char* className;
	UTTypeInfo* base;

	UTTypeInfo(const char* cn, UTTypeInfo* b): className(cn), base(b){}
	virtual bool Inherit(const UTTypeInfo* key) const ;
	virtual bool Inherit(const char* str) const ;
	virtual const char* ClassName() const = 0;
	virtual void* CreateInstance() const = 0;
};

///	実行時型情報クラスの実装
template <class T>
class UTTypeInfoImp: public UTTypeInfo{
public:
	UTTypeInfoImp(const char* cn, UTTypeInfo* b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return new T; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報クラスの実装．抽象クラス版
template <class T>
class UTTypeInfoImpAbst: public UTTypeInfo{
public:
	UTTypeInfoImpAbst(const char* cn, UTTypeInfo* b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return 0; }
	virtual const char* ClassName() const { return className; }
};

///	実行時型情報を持つクラスの基本クラス
class UTTypeInfoBase{
public:
	virtual const UTTypeInfo* GetTypeInfo() const =0;
};

///	実行時型情報を持つクラスが持つべきメンバの宣言部
#define DEF_UTTYPEINFODEF(cls)							\
public:													\
	static UTTypeInfoImp<cls> typeInfo;					\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\

///	実行時型情報を持つクラスが持つべきメンバの宣言部．抽象クラス版
#define DEF_UTTYPEINFOABSTDEF(cls)						\
public:													\
	static UTTypeInfoImpAbst<cls> typeInfo;				\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\

///	実行時型情報を持つクラスが持つべきメンバの実装．
#define DEF_UTTYPEINFO(cls)			\
UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, NULL);

///	実行時型情報を持つクラスが持つべきメンバの実装．継承する場合
#define DEF_UTTYPEINFO1(cls, base)		\
UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, &base::typeInfo);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版
#define DEF_UTTYPEINFOABST(cls)			\
UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, NULL);

///	実行時型情報を持つクラスが持つべきメンバの実装．抽象クラス版．継承する場合
#define DEF_UTTYPEINFOABST1(cls, base)		\
UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, &base::typeInfo);

#define GETCLASSNAME(p)		(p->GetTypeInfo()->className)
#define GETCLASSNAMES(T)	(T::GetTypeInfoStatic()->className)

///	ダイナミックキャスト
#define DCAST(T, p)	UTDcastImp<T>(p)
template <class T, class P> T* UTDcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	return NULL;
}

///	キャストに失敗するとassertするダイナミックキャスト
#define ACAST(T, p)	UTAcastImp<T>(p)
template <class T, class P> T* UTAcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	ダイナミックキャスト．型名にポインタ型を使うバージョン
#define DCASTP(TP, p)	UTDcastpImp<TP>(p)
template <class TP, class P> TP UTDcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	return NULL;
}
///	キャストに失敗するとassertするダイナミックキャスト．型名にポインタ型を使うバージョン
#define ACASTP(TP, p)	UTAcastpImp<TP>(p)
template <class TP, class P> TP UTAcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	キャストに失敗した場合のassert関数
char* UTAcastError(const char* );

}
#endif
