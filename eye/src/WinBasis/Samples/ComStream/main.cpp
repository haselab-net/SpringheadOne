#include <conio.h>
#include <WinBasis/WBComStream.h>

using namespace Spr;

void main(){
	WBComStream com;
	WBComStatus comSt;
	comSt.boudRate = 38400;
	comSt.flowControl = WBComStatus::FC_HARDWARE;
	com.init(comSt);
	while(1){
		if (com.avail() > 0){
			char ch = com.get();
			std::cout << ch;
		}
		if (kbhit()){
			getch();
			break;
		}
	}
}
