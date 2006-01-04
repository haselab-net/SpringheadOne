#include <Device/DVDeviceManager.h>
#include <Device/DRADAPIO.h>
#include <Device/DRContecIsaDa.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsbH8Simple.h>
#include <conio.h>
#include <iostream>
#include <vector>

using namespace Spr;

void main(){
	DVDeviceManager devMan;						//	デバイス管理クラス．
	devMan.RPool().Register(new DRAdapio(9));	//	ADPIOのユニットIDを指定して，デバイス管理クラスに登録

	devMan.RPool().Register(new DRContecIsaDa(0));
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.RPool().Register(new DRUsbH8Simple(0));


	
	devMan.Init();								//	デバイスの初期化
	std::cout << devMan;						//	結果の表示
	
	//	デバイス管理クラスからA/Dを借りてくる
	std::vector<DVAdBase*> ads;
	for(DVAdBase* ad; devMan.VPool().Rent(ad);) ads.push_back(ad);
	//	デバイス管理クラスからD/Aを借りてくる
	std::vector<DVDaBase*> das;
	for(DVDaBase* da; devMan.VPool().Rent(da);) das.push_back(da);
	
	getch();
	while(!kbhit()){
		std::cout << "D/A:";
		for(std::vector<DVAdBase*>::iterator it = ads.begin(); it != ads.end(); ++it){
			std::cout << " " << (*it)->Voltage();
		}
		std::cout << std::endl;
	}
	getch();
}
