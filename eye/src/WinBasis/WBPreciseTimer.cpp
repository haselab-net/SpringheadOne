#include "WinBasis.h"
#pragma hdrstop
#include "WBPreciseTimer.h"
#include <MMSystem.h>

namespace Spr{;
DWORD WBPreciseTimer::freq;

//��������1�b�x�݁C���g�����J�E���g����
WBPreciseTimer::WBPreciseTimer(): stopWatch(0), startFlag(false){
	cycles2.QuadPart = 0;
	if (freq == 0){
#ifndef __BORLANDC__
		Init(50);
		Init();
#else
		freq = 100 * 1000;
        freq *= 1000*1000;
#endif
	}
}

void WBPreciseTimer::Init(int preiod){
	LARGE_INTEGER cycles;
	DWORD time = timeGetTime();
    DWORD lowPart, highPart;
	_asm{
		CPUID
		RDTSC								;// �N���b�N�J�E���^��ǂ�
		MOV		lowPart,	EAX				;// �J�E���^��ۑ�
		MOV		highPart,	EDX				;// �J�E���^��ۑ�
	}
    cycles.LowPart = lowPart;
    cycles.HighPart = highPart;
	cycles2.QuadPart = cycles.QuadPart;
	//	1�b�҂�
	int deltaTime;
	while(1){;
		deltaTime = timeGetTime() - time;
		if (deltaTime > preiod) break;
	}
	_asm{
		CPUID
		RDTSC								;// �N���b�N�J�E���^��ǂ�
		MOV		lowPart,	EAX				;// �J�E���^��ۑ�
		MOV		highPart,	EDX				;// �J�E���^��ۑ�
	}
    cycles.LowPart = lowPart;
    cycles.HighPart = highPart;
	freq = DWORD(cycles.QuadPart - cycles2.QuadPart);
	freq = DWORD(freq * (1000.0 / deltaTime));
#ifdef _DEBUG
	DSTR << "WBPreciseTimer CPU freq:" << freq;
#endif
}

void WBPreciseTimer::WaitUS(int time){
#ifndef __BORLANDC__
    DWORD lowPart, highPart;
	LARGE_INTEGER cycles;
	_asm{
		CPUID;
		RDTSC								;// �N���b�N�J�E���^��ǂ�
		MOV     lowPart, EAX                ;// �J�E���^��ۑ�
		MOV     highPart, EDX               ;// �J�E���^��ۑ�
	}
	cycles.LowPart = lowPart;
	cycles.HighPart = highPart;
	cycles2.QuadPart = cycles.QuadPart + (__int64)time*freq/1000000;
	do{
		_asm{
			CPUID;
			RDTSC							;// �N���b�N�J�E���^��ǂ�
			MOV     lowPart, EAX			;// �J�E���^��ۑ�
			MOV     highPart, EDX			;// �J�E���^��ۑ�
		}
        cycles.LowPart = lowPart;
        cycles.HighPart = highPart;
	}while(cycles2.QuadPart>cycles.QuadPart);
#endif
}


int  WBPreciseTimer::CountUS()
{
	int retval=1;
#ifndef __BORLANDC__
	LARGE_INTEGER cycles;
    DWORD lowPart, highPart;
	_asm{
		CPUID;
		RDTSC								;// �N���b�N�J�E���^��ǂ�
		MOV     lowPart, EAX				;// �J�E���^��ۑ�
		MOV     highPart, EDX				;// �J�E���^��ۑ�
	}
	cycles.LowPart = lowPart;
	cycles.HighPart = highPart;
	retval =  (int)((cycles.QuadPart-cycles2.QuadPart)*1000000 / freq);
	cycles2.QuadPart = cycles.QuadPart;
#endif
	return retval;
}


void WBPreciseTimer::CountAndWaitUS(int time)
{
	int elapsedtime;
	elapsedtime = CountUS();
	WaitUS(time - elapsedtime);
}

DWORD WBPreciseTimer::Start(){
	CountUS();
	startFlag = true;
	return stopWatch;
}
DWORD WBPreciseTimer::Stop(){
	if (startFlag){
		stopWatch += CountUS();
		startFlag = false;
	}
	return stopWatch;
}
DWORD WBPreciseTimer::Clear(){
	DWORD rv = stopWatch;
	stopWatch = 0;
	return rv;
}


}
