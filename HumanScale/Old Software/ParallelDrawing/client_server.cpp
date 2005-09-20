#include <WinBasis/WBSockStr.h>
#include <iostream>
#include <conio.h>
#include "communication.h"
using namespace LVRM;
using namespace std;

int nServer = 0; // serverÇÃêî

void main(int argc, char *argv[]){
	nServer = argc - 1;
	Communication csSelect;
	if(nServer){
		csSelect.mode = Communication::CLIENT;
	}else{
		csSelect.mode = Communication::SERVER;
	}
	while(1)
	{
		if(kbhit()){
			if (getch() == 27) break;
		}
		csSelect.Client(nServer, argv);
		csSelect.Server();
	}
}