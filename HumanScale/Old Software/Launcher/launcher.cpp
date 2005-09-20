#include <WinBasis/WBSockStr.h>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace LVRM;

void main(int argc, char** argv){
	WBSockStream sock;
	char ch = 0;
	std::string buffer;
	char* address=NULL;
	if (argc > 1) address = argv[1];
	while(1){
		if (!sock.IsConnected()){
			if (address){
				sock.Connect(address, 20000);
			}else{
				sock.Listen(20000);
				sock.Accept();
			}
		}else{
			if (address){
				char ch = cin.get();
				if (ch == '\r' || ch == '\n'){
					if(buffer.length()){
						sock.write(buffer.c_str(), buffer.length());
						sock << "\n";
						sock.flush();
						sock.Close();
						break;
					}
				}else{
					buffer+=ch;
				}
			}else{
				if (sock.avail() > 0){
					char ch = sock.get();
					if (ch == '\r' || ch == '\n'){
						if(buffer.length()){
							system(buffer.c_str());
							sock.Close();
						}
						buffer="";
					}else{
						buffer+=ch;
					}
				}
			}
		}
	}
}
