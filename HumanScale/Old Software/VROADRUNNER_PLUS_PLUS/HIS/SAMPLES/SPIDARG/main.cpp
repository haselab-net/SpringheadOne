#include "HIS/HISpidarG.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#include <conio.h>
#include <iostream>

const float K = 2000;
using namespace LVRM;

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	HISpidarG spidarG;
	std::cout << devMan;
	spidarG.Init(devMan);

	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				return;
			}
		}
		spidarG.Update();
		Vec3f p = spidarG.GetPos();
		std::cout << p << std::endl;
	} 
}
