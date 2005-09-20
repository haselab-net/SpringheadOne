#include <WinBasis/WBComStream.h>
#include <conio.h>
using namespace LVRM;
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
