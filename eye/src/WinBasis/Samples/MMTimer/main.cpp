#include <WinBasis/WBMMTimer.h>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace Spr;

WBMMTimer mmt;
struct Arg{
	int counter;
	Arg():counter(0){}
};
void TimerFunc(void* a){
	Arg* arg = (Arg*) a;
	arg->counter ++;
}
void main(int argc, char** argv){
	Arg arg;					//	�^�C�}�[�֐��ɓn�������C
								//	�|�C���^�ЂƂ����n���Ȃ��̂ŁC��������n���������
								//	�N���X���\���̂��g���D

	mmt.Resolution(1);			//	1ms���x��
	mmt.Interval(2);			//	2ms���Ƃ�
	mmt.Set(TimerFunc, &arg);	//	TimerFunc�� arg �������ɌĂяo���D
#if 1	//	1�Ȃ�^�C�}�[
	mmt.Create();				//	�^�C�}�[���荞�݊J�n
#else
	mmt.Thread();				//	�X���b�h�Ŋ��荞�݊J�n
#endif
	DWORD start = timeGetTime();
	while(1){
		if (kbhit()){
			getch();
			break;
		}
		std::cout << timeGetTime() - start << "\t";
		std::cout << arg.counter << std::endl;
		Sleep(100);
	}
	mmt.Release();				//	�^�C�}�[(�X���b�h)���荞�ݏI��
}

