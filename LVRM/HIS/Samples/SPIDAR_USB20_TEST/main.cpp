#include "HIS/HISpidar8MotorTest.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>

const float K = 2000;
using namespace LVRM;

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsb20Simple(0));
	//devMan.RPool().Register(new DRContecIsaDa(0x300));
	//devMan.RPool().Register(new DRContecIsaCounter(0x200));
	//devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	//HISpidar4quad spidar;
	//HISpidar4quad spidar2;
	HISpidar8MotorTest spidar;
	std::cout << devMan;

/*
	HISpidarMotor motor[4];
	motor[0].da = (DVDaBase*)devMan.VPool().Rent("D/A");
//	if(motor[0].da) motor[0].da->Voltage(f * voltPerNewton); 
	if(motor[0].da) motor[0].da->Voltage(1);
	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			}
		}
	}
*/

	///*
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[8] = {	//	モータの取り付け位置(中心を原点とする)
		Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ),
		Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)
	};

	spidar.Init(devMan, motorPos, 0.5776f, 2.924062107079e-5f, 1.0f, 20.0f);
	//spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);

	DWORD dwNow,dwDelta,dwPast = ::timeGetTime();
	int update_count=0;
	while(1){
		update_count++;
	//for(int j=0;j<10000;j++){
		spidar.Update();
		/*
		for(int i=0;i<8;i++){
			printf("%d ",spidar.GetCount(i));
		}
		//printf("\n");
		/*
		for(int i=0;i<8;i++){
			spidar.SetForce(i,5);
		}
		*/
		///*
		for(int i=0;i<8;i++){
			printf("%d ",spidar.GetCount(i));
		}
		printf("\n");
		if (kbhit()){
			int ch = getch();
			printf("%x\n",ch-0x30);
			switch(ch){
			case 0x1b:
				goto end;
				break;
			case 0x30:
				spidar.SetForce(0,2); //ch=0,F=2[N]
				break;
			case 0x31:
				spidar.SetForce(1,2);//ch=1,F=2[N]
				break;
			case 0x32:
				spidar.SetForce(2,2);//ch=2,F=2[N]
				break;
			case 0x33:
				spidar.SetForce(3,2);//ch=3,F=2[N]
				break;
			case 0x34:
				spidar.SetForce(4,2);//ch=4,F=2[N]
				break;
			case 0x35:
				spidar.SetForce(5,2);//ch=5,F=2[N]
				break;
			case 0x36:
				spidar.SetForce(6,2);//ch=6,F=2[N]
				break;
			case 0x37:
				spidar.SetForce(7,2);//ch=7,F=2[N]
				break;
			case 0x38:
				for(int i=0;i<8;i++){ // ch0 to ch7 F=2[N]
					spidar.SetForce(i,2);
				}
				break;
			case 0x39:
				for(int i=0;i<8;i++){ // ch0 to ch7 F=0[N] but minForce = 1[N]
					spidar.SetForce(i,0);
				}
				break;
			}
		}
	}

end:;
	dwNow = ::timeGetTime();
	dwDelta = dwNow - dwPast;
	double hz = (double)update_count/(double)dwDelta;
	printf("delta:%d:%lfkHz\n",dwDelta,hz);

}