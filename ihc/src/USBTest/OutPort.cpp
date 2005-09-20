#include <math.h>
#include "iodefine.h"

void SetOutPortUp(int port){
	int status = (int)pow(2, 8 + (port-1));
	PORT.PDTRA.WORD &= ~status;
}

void SetOutPortDown(int port){
	int status = (int)pow(2, 8 + (port-1));
	PORT.PDTRA.WORD |= status;
}

void SetOutPortAllUp(){
	PORT.PDTRA.WORD = 0x0000;
}

void SetOutPortAllDown(){
	PORT.PDTRA.WORD = 0xFF00;
}

void SetOutPortInit(){
	PORT.PCTRA.LONG = 0x55550000;
	PORT.PDTRA.WORD = 0xFFFF;
}

