// DRUsb20Sh4.cpp: DRUsb20Sh4 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Device.h"
#pragma hdrstop
#include "DREzUSB.h"
#include <winioctl.h>


namespace Spr {;
DRUsb20Sh4::VirtualDeviceAd::VirtualDeviceAd(DRUsb20Sh4* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s A/D Ch %d", realDevice->Name(), ch);
}

DRUsb20Sh4::DRUsb20Sh4(int ch):DRUsb20Simple(ch){
	sprintf(name, "Cyberse USB2.0 SH4 #%d", channel);
	adVoltPerDigit = 24.0f/4096.0f;
}

void DRUsb20Sh4::Reset(){
	DWORD pipeNum = 2;
	if (hSpidar != INVALID_HANDLE_VALUE){
		OVERLAPPED ov;
		memset(&ov, 0, sizeof(ov));
		DeviceIoControl(hSpidar,
			IOCTL_Ezusb_ABORTPIPE,
			&pipeNum, sizeof(pipeNum), NULL, 0, NULL, &ov);
		DeviceIoControl(hSpidar,
			IOCTL_Ezusb_RESETPIPE,
			&pipeNum, sizeof(pipeNum), NULL, 0, NULL, &ov);
	}
}

void DRUsb20Sh4::Register(DVVirtualDevicePool& vpool){
	DRUsb20Simple::Register(vpool);
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDeviceAd(this, i));
	}
}
void DRUsb20Sh4::Update(){
	UsbUpdate();
}

//--------------------------------------------------------------------------------
//	USBでの通信
#define PACKET_SIZE			512
#define OP_ALL_DATA_UPDATE	0x01
#define OP_BASE_ANALYZER	0x0e


void DRUsb20Sh4::UsbUpdate(){
	UCHAR outBuffer[PACKET_SIZE];
	UCHAR inBuffer[PACKET_SIZE];

	int cur = 0;	//	パケット先頭からの位置
	outBuffer[cur++] = OP_ALL_DATA_UPDATE;
	outBuffer[cur++] = OP_BASE_ANALYZER;
	for(int i=0; i < 8; i++){
		outBuffer[cur++] = daOut[i] & 0xFF;
		outBuffer[cur++] = daOut[i] >> 8;
	}
	UsbSend(outBuffer);
	UsbRecv(inBuffer);

	cur = 16;	//	パケット先頭からの位置，D/Aの設定値は読み飛ばす．
	//	カウンタの値の読み出し
	for(int i=0; i<8; ++i){
		DWORD ctIn = inBuffer[cur++];
		ctIn |= (DWORD)(inBuffer[cur++]) << 8;
		//	カウンタは16bitしかないので，前回の値から32bitに拡張する．
		DWORD ct = -count[i];
		DWORD last = ct&0xFFFF;
		int diff = (int)ctIn - (int)last;
		if (diff > 0){
			if (diff > 0x8000){
				diff = int(ctIn-0x10000) - (int)last;
			}
		}else{
			if (diff < -0x8000){
				diff = (int)(ctIn+0x10000) - (int)last;
			}
		}
		ct += diff;
		count[i] = -ct;
	}
	//	A/Dの読み出し
	for(int i=0; i<8; ++i){
		WORD ad = inBuffer[cur++];
		ad |= (WORD)(inBuffer[cur++]) << 8;
		adIn[i] = ad;
	}
	//	PIOの読み出し
	UCHAR pio[2];
	pio[0] = inBuffer[cur++];
	pio[1] = inBuffer[cur++];
//	DSTR << "PIO:";
	for(int i=0; i<16; ++i){
		pioLevel[i] = (pio[i/8] & (0x01 << (i%8))) ? 1 : 0;
//		DSTR << pioLevel[i];
	}
//	DSTR << " " << RotarySwitch();
//	DSTR << std::endl;

	//	infoってなに？
	DWORD info;
	info = inBuffer[cur++];
	info |= (DWORD)(inBuffer[cur++]) << 8;
	info |= (DWORD)(inBuffer[cur++]) << 16;
	info |= (DWORD)(inBuffer[cur++]) << 24;
//	DSTR << "info:" << info << std::endl;
}
void DRUsb20Sh4::UsbSend(unsigned char* outBuffer){
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = PACKET_SIZE;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		outBuffer,
		outPacketSize,
		(unsigned long *)&nBytes, NULL);
}
void DRUsb20Sh4::UsbRecv(unsigned char* inBuffer){
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 2;
	WORD inPacketSize = PACKET_SIZE;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_READ,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		inBuffer,
		inPacketSize,
		(unsigned long *)&nBytes,
		NULL);
}
bool DRUsb20Sh4::FindDevice(int ch){
	for(int i=0; i < 0x100; ++i){
		hSpidar = UsbOpen(i);
		if (hSpidar == INVALID_HANDLE_VALUE) return false;
		if (UsbVidPid(hSpidar) != 0x0CEC0205){	//	SH版アンプのVIDとPID
			UsbClose(hSpidar);
			continue;
		}		
		Reset();
		Update();
		if (ch == RotarySwitch()) return true;
		UsbClose(hSpidar);
	}
	return INVALID_HANDLE_VALUE;
}

} //namespace Spr

