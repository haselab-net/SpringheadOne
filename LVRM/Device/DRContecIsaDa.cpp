#include "DRContecIsaDa.h"
#include <WinBasis/WBPortIO.h>
#include <stdio.h>

namespace LVRM {

DRContecIsaDa::VirtualDevice::VirtualDevice(DRContecIsaDa* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s Channel %d", realDevice->Name(), ch);
}
DRContecIsaDa::DRContecIsaDa(int a){
	address = a;
	sprintf(name, "Contec ISA D/A at 0x%03X", address);
	WBGetPortIO();
}
bool DRContecIsaDa::Init(){
	if (address == 0) return false;
	_outp(address+0x2,0x01);					//	range set mode �ɂ���
	_outp(address+0x0,0x00);					//	channel 0 to 3
	_outp(address+0x1,0x00);					//	range data set 0�`10[V]
	int in = _inp(address+0x1);
	if (in == 0xFF) return false;
	_outp(address+0x0,0x04);					//	channel 4 to 7
	_outp(address+0x1,0x00);					//	range data set 0�`10[V]
	_outp(address+0x2,0x00);					//	�ʏ탂�[�h
	return true;
}
void DRContecIsaDa::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDevice(this, i));
	}
}
void DRContecIsaDa::Voltage(int ch, float volt){
	if (address == 0) return;

	int value = volt * 409.6f;
	//	�ő�l�ŏ��l�͈̔͂𒴂��Ȃ��悤�ɂ���
	if (0xFFF < value) value = 0xFFF;
	//	�o��
	_outp(address+0x0,(unsigned char)(ch + ((value << 4) & 0x0f0) ));
														//	OUTPUT DATA (LOW)
	_outp(address+0x1,(unsigned char)(value >> 4));		//	            (HIGH)
}
}	//	namespace LVRM

