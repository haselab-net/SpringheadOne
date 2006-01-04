#ifndef WINCMP_MISC_CCRITICALSECTION_H
#define WINCMP_MISC_CCRITICALSECTION_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/BaseDebug.h>
#include <vector>
namespace Spr {

class SPR_DLL WBCriticalSection{
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

}	//	namespace Spr

#endif