#include <Device/DRTokin3D.h>
#include <Device/DRAdapio.h>
#include <Haptic/HITurnTable.h>
#include <Haptic/HIStepWalk.h>
#include <WinBasis/WBWin.h>
#include <iostream> 
#include <conio.h> 
 
using namespace LVRM;


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
			if (k == 'r' || k == 'R'){	//"r"‚ğ‰Ÿ‚·‚ÆA‚»‚ÌŠp“x‚ğstart_angle‚Éİ’è
				turn.Reset();
			}else if(k == 'q' || k == 'Q' || k == 0x1b){
				break;
			}
		}
		turn.Step();
		walk.Step();
		std::cout << "Angle:" << turn.GetAngle();
//		std::cout << "  Vel:" << walk.GetVelocity() << std::endl;
		std::cout << "  Vel:";
		walk.Print(std::cout);
		std::cout << std::endl;
		Sleep(20);
	}
	return 0;
}


