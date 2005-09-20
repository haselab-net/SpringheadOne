#ifndef WINCMP_MISC_CCRITICALSECTION_H
#define WINCMP_MISC_CCRITICALSECTION_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

namespace LVRM {

class WBCriticalSection{
	CRITICAL_SECTION cr;
public:
    WBCriticalSection(){
        InitializeCriticalSection(&cr);
	}
	~WBCriticalSection(){
        DeleteCriticalSection(&cr);
	}
    void Enter(){
		EnterCriticalSection(&cr);
	}
    void Leave(){
		LeaveCriticalSection(&cr);
	}
};

struct WBAutoLock{
	WBCriticalSection& cr;
	WBAutoLock(WBCriticalSection& c):cr(c){
		cr.Enter();
	}
	~WBAutoLock(){
		cr.Leave();
	}
};

}	//	namespace LVRM

#endif