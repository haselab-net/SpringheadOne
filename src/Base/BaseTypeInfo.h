#ifndef BASETYPEINFO_H
#define BASETYPEINFO_H

namespace Spr{;

class UTTypeInfo {
public:
	const char* className;
	UTTypeInfo* base;

	UTTypeInfo(const char* cn, UTTypeInfo* b): className(cn), base(b){}
	virtual bool Inherit(const UTTypeInfo* key) const ;
	virtual const char* ClassName() const = 0;
	virtual void* CreateInstance() const = 0;
};
template <class T>
class UTTypeInfoImp: public UTTypeInfo{
public:
	UTTypeInfoImp(const char* cn, UTTypeInfo* b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return new T; }
	virtual const char* ClassName() const { return className; }
};

template <class T>
class UTTypeInfoImpAbst: public UTTypeInfo{
public:
	UTTypeInfoImpAbst(const char* cn, UTTypeInfo* b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return 0; }
	virtual const char* ClassName() const { return className; }
};

#define DEF_UTTYPEINFO(cls)			\
UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, NULL);

#define DEF_UTTYPEINFO1(cls, base)		\
UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, &base::typeInfo);

#define DEF_UTTYPEINFOABST(cls)			\
UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, NULL);

#define DEF_UTTYPEINFOABST1(cls, base)		\
UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, &base::typeInfo);

#define DCAST(T, p)	UTDcastImp<T>(p)
template <class T, class P> T* UTDcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	return NULL;
}
#define ACAST(T, p)	UTAcastImp<T>(p)
template <class T, class P> T* UTAcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
char* UTAcastError(const char* );

}
#endif
