#include "Device.h"
#pragma hdrstop
#include "DRUsbH8SimpleDef.h"

namespace Spr {


DRUsbH8Simple::VirtualDeviceDa::VirtualDeviceDa(DRUsbH8Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A Ch %d", realDevice->Name(), ch);
}
DRUsbH8Simple::VirtualDeviceCounter::VirtualDeviceCounter(DRUsbH8Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s counter Ch %d", realDevice->Name(), ch);
}
DRUsbH8Simple::DRUsbH8Simple(int ch){
	channel = ch;
	sprintf(name, "Cyberse USB H8 Simple #%d", channel);
	hSpidar = NULL;
	int si[] = {-1, -1, 1, 1, 1, 1, -1, -1};
	for(int i=0; i<8; i++){
		sign[i] = si[i];
		daOut[i] = 0;
		count[i] = 0;
		countOffset[i] = 0;
	}
}
bool DRUsbH8Simple::Init(){
	hSpidar = CreateFile(
					"\\\\.\\Spidar0",
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL, 
					OPEN_EXISTING, 
					0, 
					NULL);
	if (hSpidar == INVALID_HANDLE_VALUE ) return false;
	ULONG length;
	unsigned char SetData[8];
	SetData[0] = 1;
	for(int i = 1; i < 8; i++) SetData[i] = 0;
	DeviceIoControl(hSpidar, IOCTL_SPIDAR_SET_DATA, SetData, 8, NULL, 0, &length, NULL);
	return true;
}
void DRUsbH8Simple::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDeviceDa(this, i));
		vpool.Register(new VirtualDeviceCounter(this, i));
	}
}

//	int table[8] = {6,8,1,3, 7,5,4,2};
const int table[8] = {5,7,0,2, 6,4,3,1};
void DRUsbH8Simple::Voltage(int ch, float v){
	int value = (int) (0x800 + sign[ch] * v * 102.4f);
	daOut[table[ch]] = value;
}
void DRUsbH8Simple::Digit(int ch, int v){
	daOut[table[ch]] = v;
}
void DRUsbH8Simple::Count(int ch, long c){
	countOffset[table[ch]] = - (count[table[ch]] - c); 
}
long DRUsbH8Simple::Count(int ch){
	return count[table[ch]] + countOffset[table[ch]];
}

void DRUsbH8Simple::Update(){
	ULONG length;
	unsigned char PutData[64];
	unsigned char GetData[64];
	for(int i = 0; i < 64; i++) PutData[i] = 0;
	for(int i = 0; i < 64; i++) GetData[i] = 0;
	PutData[16] = 5;
	//	D/A�o�̓f�[�^�̍쐬
	for(int i = 0; i < 8; i++){
		int value = daOut[i];
		PutData[i*2] = value >> 8;
		PutData[i*2+1] = value & 0xFF;
	}
	//	D/A�o��
	DeviceIoControl(hSpidar, IOCTL_SPIDAR_PUT_DATA, PutData, 64, NULL, 0, &length, NULL);
	//	Counter����
	DeviceIoControl(hSpidar, IOCTL_SPIDAR_GET_DATA, NULL, 0, GetData, 64, &length, NULL);
	//	Counter�f�[�^�̎擾
	for(int i = 0; i < 8; i++){
		int value = GetData[i*4] * 256 + GetData[i*4+1] + GetData[i*4+3] * 65536;
		if(GetData[i*4+3] & 0x80){
			value |= 0xFF000000;
		}
		count[i] = (value * sign[i]);
	}
}

}	//	namespace Spr
