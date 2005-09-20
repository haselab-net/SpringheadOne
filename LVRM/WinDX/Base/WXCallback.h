#ifndef LVRM_MISC_CALLBACK_H
#define LVRM_MISC_CALLBACK_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>
#include <WinBasis/WBWin.h>
#include <Base/Debug.h>

namespace LVRM {

template <class TCaller>
struct WXCallbackBase{
	virtual void Call(TCaller* caller)=0;
	virtual size_t size()const=0;
	bool operator ==(const WXCallbackBase& c) const{
		if (size() != c.size()) return false;
		return memcmp(this, &c, size()) == 0;
	}
};

template <class TCaller, class TCallee>
class WXCallback:public WXCallbackBase<TCaller>{
public:
	typedef void (TCallee::*Func)(TCaller*);
protected:
	TCallee* callee;
	Func func;
public:
	WXCallback(TCallee* c, Func f){ callee = c; func = f;}
	void Call(TCaller* caller){ (callee->*func)(caller); }
	size_t size() const { return sizeof(*this); }
};

template <class TCaller>
class WXCallbacks:public std::vector<WXCallbackBase<TCaller>*>{
public:
	typedef std::vector<WXCallbackBase<TCaller>*> superclass;
	typedef std::vector<WXCallbackBase<TCaller>*> container;
	~WXCallbacks(){
		for(iterator it=begin(); it != end(); ++it) delete *it;
	}
	void Call(TCaller* caller){
		container copy(*this);
		for(iterator it=copy.begin(); it != copy.end(); ++it) {
			if (std::find(begin(), end(), *it) != end()) (*it)->Call(caller);
		}
	}
	superclass::iterator find(const WXCallbackBase<TCaller>& cb){
		iterator it;
		for(it = begin(); it != end(); ++it) if (**it == cb) break;
		return it;
	}
	bool Add(WXCallbackBase<TCaller>* cb){
		iterator itf = find(*cb);
		if (itf != end()){
			delete cb;
			return false;
		}
		push_back(cb);
		return true;
	}
	bool Del(const WXCallbackBase<TCaller>& cb){
		iterator itf = find(cb);
		if (itf != end()){
			delete *itf;
			erase(itf);
			return true;
		}
		DPF("Fail to delete.\n");
		return false;
	}
};


}	//	namespace LVRM

#endif