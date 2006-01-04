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
	Arg arg;					//	タイマー関数に渡す引数，
								//	ポインタひとつしか渡せないので，たくさん渡したければ
								//	クラスか構造体を使う．

	mmt.Resolution(1);			//	1ms精度で
	mmt.Interval(2);			//	2msごとに
	mmt.Set(TimerFunc, &arg);	//	TimerFuncを arg を引数に呼び出す．
#if 1	//	1ならタイマー
	mmt.Create();				//	タイマー割り込み開始
#else
	mmt.Thread();				//	スレッドで割り込み開始
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
	mmt.Release();				//	タイマー(スレッド)割り込み終了
}

