#include <Device/DRTokin3D.h>
#include <Device/DRAdapio.h>
#include <HIS/HITurnTable.h>
#include <HIS/HIStepWalk.h>
#include <WinBasis/WBWin.h>
#include <iostream> 
#include <conio.h> 
 
using namespace Spr;


int main(){
	DVDeviceManager dev;
	dev.RPool().Register(new DRTokin3D);
	dev.RPool().Register(new DRAdapio(10));
	dev.Init();
	
	HITurnTable	turn;
	HIStepWalk	walk;
	turn.Init(dev);
	walk.Init(dev);
	
	while(1){
		if (kbhit()){
			int k = getch();
			if (k == 'r' || k == 'R'){	//"r"を押すと、その角度をstart_angleに設定
				turn.Reset();
			}else if(k == 'q' || k == 'Q' || k == 0x1b){
				break;
			}
		}
//		turn.Step();
//		walk.Step();
		turn.Step(0);
		walk.Step(0);
		std::cout << "Angle:" << turn.GetAngle();
//		std::cout << "  Vel:" << walk.GetVelocity() << std::endl;
		std::cout << "  Vel:";
		walk.Print(std::cout);
		std::cout << std::endl;
		Sleep(20);
	}
	return 0;
}


