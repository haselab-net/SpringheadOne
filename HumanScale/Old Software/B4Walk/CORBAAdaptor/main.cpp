#include "../communication.h"
#include <Base/Affine.h>
#include <conio.h>
using namespace LVRM;
using namespace std;

float angle;
float angleOffset;
Affinef afBody;
volatile bool sendFlag;
float NormalizeAngle(float rad){
	while (rad > Rad(180)) rad -= Rad(360);
	while (rad < -Rad(180)) rad += Rad(360);
	return rad;
}

class CmdEye:public Command{
public:
	CmdEye(CmdSocket* s):Command(s){}
	int Id()const{ return 1; }
	int Receive(char* data){
		Affinef& af = *(Affinef*)data;
		afBody = af;
		angle = -atan2(afBody.Ex().Z(), afBody.Ex().X());
		std::cout << "Recv:" << af.Pos() << Deg(angle) << std::endl;
		return sizeof(af);
	}
	void Send(){
		SendId();
		afBody.Rot() = Affinef(angle, 'y').Rot();
		SendData(&afBody, sizeof(afBody));
		cmdSocket->Flush();
	}
};

volatile bool bQuit = false;

DWORD WINAPI CommThread(void*){
	CmdSocket cmdSocket;
	cmdSocket.Register(new CmdEye(&cmdSocket));
	cmdSocket.Init(10000, 10100);
	while(1){
		if (sendFlag){
			CmdEye cmdEye(&cmdSocket);
			cmdEye.Send();
			sendFlag = false;
		}
		while(cmdSocket.Receive());
		if (kbhit()){
			int ch = getch();
			if (ch == 75){
				afBody = afBody * Affinef(Rad(5), 'Y');
				sendFlag = true;
			}
			if (ch == 77){
				afBody = afBody * Affinef(Rad(-5), 'Y');
				sendFlag = true;
			}
			if (ch == 0x1b){
				bQuit = true;
				break;
			}
		}
		Sleep(30);
	}
	return 0;
}

void StartComm(){
	DWORD tid=0;
	HANDLE h = CreateThread(NULL, 0, CommThread, NULL,0, &tid);
}

void main(){
	StartComm();
	while(!bQuit){
		Sleep(100);
	}
}
