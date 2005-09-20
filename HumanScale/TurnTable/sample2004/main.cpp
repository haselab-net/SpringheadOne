#include <stdio.h>
#include <process.h>
#include <conio.h>

#include <Device/DRAdapio.h>

#include "main.h"

namespace LVRM{;

void Step(void *hoge){
	bool bInitSuccess = InitTurnTable();

	if (bInitSuccess){
		while(bRun){
			//	ターンテーブル制御処理
			float inner = hiStep.GetInner();
			float turnVel = hiTurn.GetTurnVel();
			int statusOfUser = hiStep.GetStatusOfUser();

			hiStep.Step(turnVel);

			if(statusOfUser < 4){
				hiTurn.Step(inner);
			}else{
				hiTurn.Step(0);
			}

			inner = inner/ 3.141592 * 180;
			printf("StatusOfUser = %d\n", statusOfUser);
			printf("UserAngle = %3.3f\n\n", inner);
		}
	}
	bStep = false;
	_endthread();
}

void KeyEvent(void *hoge){
	while(bRun){
		if(kbhit()){
			char ch = getch();
			switch(ch){
			case 'c': // start calibration
				hiStep.pSenCal = true;
				//hiStep.beforeCalib();
				break;
			case 'e': // end calibration
				hiStep.pSenCal = false;
				hiStep.PSenRange();
				hiStep.SetStatusOfUser(0);
				//hiStep.ValueOutput();
				break;
			case 's':
				bRun = false;
				break;
			}
		}
	}
	bKeyEvent = false;
	_endthread();
}

bool InitTurnTable(){
	//	ターンテーブルの初期化
	bool bInitSuccess = false;
	dev.RPool().Register(new DRAdapio(10));
	dev.Init();
	std::cout << dev;
	bInitSuccess = hiTurn.Init(dev);
	bInitSuccess = bInitSuccess && hiStep.Init(dev);
	
	return bInitSuccess;
}


};
void main()
{
	bRun = true;
	bStep = true;
	bKeyEvent = true;
	_beginthread(Step, 0, NULL);
	_beginthread(KeyEvent, 0, NULL);

	while(1){}

	return;
}
