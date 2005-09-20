#include "WBMMTimer.h"
#include <Base/Debug.h>
#include <mmsystem.h>

namespace LVRM {

using namespace std;
int WBMMTimer::count;
UINT WBMMTimer::resolution=1;
WBMMTimer::WBMMTimer()
	{
	func = NULL;
	arg = NULL;
	timerID = 0;
	interval = resolution;
	if (interval==0) interval = 1;
    bRun = false;
	bCreated = false;
	bThread = false;
	hThread = NULL;
	heavy = 0;
	}
WBMMTimer::~WBMMTimer()
	{
	Release();
	}
bool WBMMTimer::Create()
	{
	heavy = 0;
	tick = GetTickCount();
	if (bCreated) return true;
	if (bThread) Release();
	if (count == 0) BeginPeriod();
	count ++;
	timerID = timeSetEvent(interval, resolution, TimerCallback, (DWORD)this, TIME_PERIODIC);
	bCreated = (timerID != 0);
	return bCreated;
	}
bool WBMMTimer::Thread()
	{
	heavy = 0;
	if (bThread) return true;
	if (bCreated) Release();
	DWORD id=0;
	bThread = true;
	hThread = CreateThread(NULL, 0x1000, ThreadCallback, this, 0, &id);
	if (hThread){
		bRun = true;
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else bThread = false;
	return bThread;
	}
void WBMMTimer::Release()
	{
	if (bThread)
		{
		bThread = false;
		for(int t=0; t<100 && bRun; t++) Sleep(20);
		CloseHandle(hThread);
		hThread = NULL;
		}
	if (bCreated)
		{
		timeKillEvent(timerID);
		for(int i=0; i<100; i++)
			{
			if (!bRun) break;
			Sleep(10); 
			}
		count --;
		if (count == 0)	EndPeriod();
		bCreated = false;
		}
	}
void WBMMTimer::BeginPeriod()
	{
	TIMECAPS tc;
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		{
		OutputDebugString("WBMMTimer::Resolution()  Fail to get resolution.\n");
		VRASSERT(0);
		}
	resolution = min(max(tc.wPeriodMin, resolution), tc.wPeriodMax);
	timeBeginPeriod(resolution);
	}
void WBMMTimer::EndPeriod()
	{
	timeEndPeriod(resolution);
	}
void CALLBACK WBMMTimer::TimerCallback(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
	{
	WBMMTimer& mmtimer = *(WBMMTimer*)dwUser;
#if 0
    int tick = GetTickCount();
	int delta = tick - mmtimer.tick;
	if (delta > mmtimer.interval+1) mmtimer.heavy ++;
	mmtimer.tick = tick;
#endif
	mmtimer.bRun = true;
	mmtimer.func(mmtimer.arg);
    mmtimer.bRun = false;
	}
DWORD WINAPI WBMMTimer::ThreadCallback(void* arg){
	WBMMTimer& mmtimer = *(WBMMTimer*)arg;

	DWORD lastCall = GetTickCount();
	while(mmtimer.bThread){
		DWORD now = GetTickCount();
		DWORD nextCall = lastCall + mmtimer.interval;
		if (int(nextCall - now) > 0){
			Sleep(nextCall - now);
		}
 		lastCall = now;
		
		mmtimer.func(mmtimer.arg);
	}
	mmtimer.bRun = false;
	return 0;
}
UINT WBMMTimer::Resolution()
	{
	return resolution;
	}
void WBMMTimer::Resolution(UINT res)
	{
	if (resolution == res) return;
	if (count)
		{
		EndPeriod();
		BeginPeriod();
		}
	}
UINT WBMMTimer::Interval()
	{
	return interval;
	}
void WBMMTimer::Interval(UINT i)
	{
	if (i == interval) return;
	interval = i;
	if (bCreated)
		{
		Release();
		Create();
		}
	}
void WBMMTimer::Set(MMTimerFunc* f, void* a)
	{
	func = f;
	arg = a;
	}

}	//	namespace LVRM
