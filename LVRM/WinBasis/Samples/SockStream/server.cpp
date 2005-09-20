#include <WinBasis/WBSockStr.h>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace LVRM;

void main(){
	WBSockStream sock;
	char ch = 0;
	while(1){
		if (!sock.IsConnected()){
			sock.Listen(20000);
			sock.Accept();
			cout << "a";
		}else{
			if (!sock.good()) break;
			if (sock.avail() > 0){
				ch = sock.get();
				cout << ch;
			}
			if(kbhit()){
				char in = getch();
				sock << in;
				sock.flush();
			}
		}
	}
}
