#include <WinCmp/Stream/CSockStr.h>
#include <iostream>
#include <conio.h>

using namespace std;

void main(){
	WBSockStream sock;
	sock.Connect("localhost", 10000);
	char ch = 0;
	while(ch != ' ' && sock.good()){
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
