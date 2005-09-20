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
	DVDeviceManager devMan;						//	�f�o�C�X�Ǘ��N���X�D
	devMan.RPool().Register(new DRAdapio(9));	//	ADPIO�̃��j�b�gID���w�肵�āC�f�o�C�X�Ǘ��N���X�ɓo�^

	devMan.RPool().Register(new DRContecIsaDa(0));
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.RPool().Register(new DRUsbH8Simple(0));


	
	devMan.Init();								//	�f�o�C�X�̏�����
	std::cout << devMan;						//	���ʂ̕\��
	
	//	�f�o�C�X�Ǘ��N���X����A/D���؂�Ă���
	std::vector<DVAdBase*> ads;
	for(DVAdBase* ad; devMan.VPool().Rent(ad);) ads.push_back(ad);
	//	�f�o�C�X�Ǘ��N���X����D/A���؂�Ă���
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
