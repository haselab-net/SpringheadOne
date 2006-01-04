#include "HIS/HISpidar4AngleQuad.h"
/// USB20を使わないときには下の「#define _USB20_」をコメントアウト
//#define _USB20_

#ifdef _USB20_
#include <Device/DRUsb20Simple.h>
#else
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#endif //_USB20_

#include <conio.h>
#include <iostream>
//追加
//#include<glut.h>
const float K = 2000;
using namespace Spr;



HISpidar4anglequad spidar;

void main(){

	DVDeviceManager devMan;


#ifdef _USB20_
	devMan.RPool().Register(new DRUsb20Simple(0));
#else
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
#endif //_USB20_
	devMan.Init();

	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	//　手元位置の最初の位置、姿勢
	//  (x+x_1,y+y_1,z+z_1),(x-x_1,y-y_1,z-z_1),(x+x_2,y+y_2,z+z_2),(x-x_2,y-y_2,z-+z_2),(x,y,z)
	//  の順番で書いている
	Vec3f initPos[5] = {
		Vec3f(-0.045f,0.0f,0.04f), Vec3f(0.045f,0.0f,-0.04f), Vec3f(-0.045f,0.0f,-0.04f), Vec3f(0.045f,0.0f,0.04f), Vec3f(0.0f,0.0f,0.0f)
	};

	
	spidar.Init(devMan, motorPos[0], initPos, 0.2f/*0.3776f*/, 2.924062107079e-5f, 1.570796326795e-3f, 1.0f, 20.0f);
	spidar.Calib();

	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			case  0x36:
				spidar.Calib();
				break;
			}
		}
		spidar.Update();
	
//		std::cout << "/****************長さと角度*****************/" << std::endl;
/*		std::cout << "l1   =   " <<  spidar.l1 << std::endl;
		std::cout << "l2   =   " <<  spidar.l2 << std::endl;
		std::cout << "l3   =   " <<  spidar.l3 << std::endl;
		std::cout << "l4   =   " <<  spidar.l4 << std::endl;
		std::cout << "a1   =   " <<  spidar.a1 << std::endl;
		std::cout << "a2   =   " <<  spidar.a2 << std::endl;
		std::cout << "a3   =   " <<  spidar.a3 << std::endl;
		std::cout << "a4   =   " <<  spidar.a4 << std::endl;
*/
		std::cout << "/****************位置*****************/" << std::endl;
		std::cout << "X    =   " <<  spidar.Xnew << std::endl;
		std::cout << "Y    =   " <<  spidar.Ynew << std::endl;
		std::cout << "Z    =   " <<  spidar.Znew << std::endl;
/*		std::cout << "X1   =   " <<  spidar.X1new << std::endl;
		std::cout << "Y1   =   " <<  spidar.Y1new << std::endl;
		std::cout << "Z1   =   " <<  spidar.Z1new << std::endl;
		std::cout << "X2   =   " <<  spidar.X2new << std::endl;
		std::cout << "Y2   =   " <<  spidar.Y2new << std::endl;
		std::cout << "Z2   =   " <<  spidar.Z2new << std::endl;
*/

/*
		float rad = 0.25;
		Vec3f colde;
		Vec3f force; 

		Affinef graphic = Affinef(0, 0, -0.5);

		colde = graphic.Pos() - spidar.GetPos(4); 
						
		spidar.Update();
		if (rad - colde.norm() > 0){
			force = -K * (rad - colde.norm()) * (graphic.Pos() - spidar.GetPos(4)).unit();
			}
			spidar.SetForce(force, 0.1f);
*/
	}


end:;
}


