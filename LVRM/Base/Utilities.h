#ifndef LVRM_BASE_UTILITIES_H
#define LVRM_BASE_UTILITIES_H
#include "Debug.h"
#include <new.h>
#include <iosfwd>
/**	@file	Utilities.h	その他のユーティリティークラス・関数．*/

namespace LVRM {

template <class T>
bool UTContentsEqual(const T& t1, const T& t2){
	return *t1 == *t2;
}

template <class T>
struct UTContentsLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1 < *t2;
	}
};

template <class T, class M, M T::* m>
struct UTMemberLess{
	bool operator ()(const T& t1, const T& t2) const{
		return *t1.*m < *t2.*m;
	}
};

class UTPadding{
public:
	int len;
	UTPadding(int i){len = i;}
};
std::ostream& operator << (std::ostream& os, UTPadding p);

struct UTEatWhite{
	UTEatWhite(){}
};
std::istream& operator >> (std::istream& is, const UTEatWhite& e);

//-----------------------------------------------------------------------------
//	参照カウントクラスのベース．カウントが0になっても勝手に消えはしない．
class UTRefCount{
	mutable int refCount;
public:
#ifndef _DEBUG
	UTRefCount(){refCount = 0;}
	UTRefCount(const UTRefCount&){refCount = 0;}
	~UTRefCount(){ assert(refCount==0); }
#else
	static int nObject;
	UTRefCount();
	UTRefCount(const UTRefCount& r);
	~UTRefCount();
#endif
	UTRefCount& operator = (const UTRefCount& r){ return *this; }

	int AddRef() const { return ++ refCount; }
	int DelRef() const {
		assert(refCount > 0);
		return -- refCount;
	}
	int RefCount() const { return refCount; }
};

template <class T>
class UTRef{
	T* rcObj;
public:
	UTRef(T* t = NULL){
		rcObj = t;
		if (rcObj) rcObj->AddRef();
	}
	template <class E>
	UTRef(const UTRef<E>& r){
		rcObj = (T*)(E*)r;
		if (rcObj) rcObj->AddRef();
	}
	UTRef(const UTRef<T>& r){
		rcObj = r.rcObj;
		if (rcObj) rcObj->AddRef();
	}
	~UTRef(){ if (rcObj && rcObj->DelRef() == 0) delete rcObj; }
	UTRef& operator =(T* t){
		if (rcObj) rcObj->DelRef();
		rcObj = t;
		if (rcObj) rcObj->AddRef();
		return *this;
	}
	template <class E>
	UTRef& operator =(const UTRef<E>& r){
		if (rcObj) rcObj->DelRef();
		rcObj = r;
		if (rcObj) rcObj->AddRef();
		return *this;
	}
	UTRef& operator =(const UTRef<T>& r){
		if (rcObj) rcObj->DelRef();
		rcObj = r;
		if (rcObj) rcObj->AddRef();
		return *this;
	}
	operator T*() const {return rcObj;}
	T* operator->() const {return rcObj;}
	bool operator <(const UTRef& r) const { return rcObj < r.rcObj; }
};
template <class T>
T& Singleton(){
	static T t;
	return t;
}


}	//	namespace LVRM

#endif
