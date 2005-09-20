#include "iodefine.h"

void PhysicsThread(){
	PORT.PDTRA.WORD &= ~0x0200; //Port2 Up
	for(int i = 0; i < 10; i++){}
	PORT.PDTRA.WORD |= 0x0200;  //Port2 Down

	return;
}

