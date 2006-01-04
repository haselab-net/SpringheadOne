#include "Device.h"
#pragma hdrstop


namespace Spr {

DRContecIsaCounter::VirtualDevice::VirtualDevice(DRContecIsaCounter* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s Channel %d", realDevice->Name(), ch);
}
DRContecIsaCounter::DRContecIsaCounter(int a){
	address = a;
	sprintf(name, "Contec ISA Counter at 0x%03X", address);
}
bool DRContecIsaCounter::Init(){
	if (address == 0) return true;
	WBGetPortIO();
	//	�{�[�h�����݂��邱�Ƃ��m�F
	_outp(address, 0x15);		//	���荞�݃}�X�N
	int data = _inp(address+0x1);
	if (data == 0xFF)			//	���荞�݃}�X�N��0xFF�ł͂Ȃ��͂�
		{
		DPF("Could not find Contec ISA counter borad at 0x%x.\n", address);
		return false;
		}
	DPF("Contec ISA counter borad was found at 0x%x.\n", address);
	//	�J�E���^�{�[�h�̏�����
	//	operation command �͉����P.3-4�Q��
	for(int channel=0; channel<4; channel++){
		_outp(address    , channel*5+1);			//	mode set
		_outp(address+0x1, 0x86);
		_outp(address    , channel*5+2);			//	z-pulse invalidity
		_outp(address+0x1, 0x2);					//	z-pulse ��p���Ȃ�
		_outp(address    , 0x16);				//	sense reset
		_outp(address+0x1, 0xff);
	}
	return true;
}
void DRContecIsaCounter::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<4; i++){
		vpool.Register(new VirtualDevice(this, i));
	}
}
void DRContecIsaCounter::Count(int ch, long n){
	if (address == 0) return;
	unsigned char low,middle,high;
	high   = (unsigned char)(n >>16);
	middle = (unsigned char)(n >> 8);
	low    = (unsigned char)n;

	_outp(address    , ch*5 );	//	���ۂɒl�������镔��
	_outp(address+0x1, low);
	_outp(address+0x1, middle);
	_outp(address+0x1, high);
}
long DRContecIsaCounter::Count(int ch){
	if (address == 0) return 0;
	unsigned long low,middle,high;
	_outp(address    , 0x14  );					 	    //	data latch
	_outp(address+0x1, 0xf   );
	_outp(address    , ch*5 );							//	read data

	low    = _inp(address+0x1) & 0xff;	//	0x000000??
  	middle = _inp(address+0x1) & 0xff;	//	0x0000??00
	high   = _inp(address+0x1) & 0xff;	//	0x00??0000
	if (high & 0x80) high |= 0xFF00;	//	0x??000000
	return (long) (low+(middle<<8)+(high<<16));
}

}	//	namespace Spr
