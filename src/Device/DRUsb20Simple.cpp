// DRUsb20Simple.cpp: DRUsb20Simple クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Device.h"
#pragma hdrstop
#include "DREzUSB.h"
#include <winioctl.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr {

DRUsb20Simple::VirtualDeviceDa::VirtualDeviceDa(DRUsb20Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A Ch %d", realDevice->Name(), ch);
}
DRUsb20Simple::VirtualDeviceCounter::VirtualDeviceCounter(DRUsb20Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s counter Ch %d", realDevice->Name(), ch);
}
DRUsb20Simple::VirtualDevicePio::VirtualDevicePio(DRUsb20Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s PIO Ch %d", realDevice->Name(), ch);
}

DRUsb20Simple::DRUsb20Simple(int ch)
{
	channel = ch;
	sprintf(name, "Cyberse USB2.0 Simple #%d", channel);
	hSpidar = NULL;
	for(int i=0; i<8; i++){
		daOut[i] = 0;
		adIn[i]=0;
		count[i] = 0;
		countOffset[i] = 0;
		pioLevel[i] = 0;
	}
}

DRUsb20Simple::~DRUsb20Simple()
{
}
bool DRUsb20Simple::Init(){
	return FindDevice(channel);
}
bool DRUsb20Simple::InitAny(){
	for(int i=0; i < 0x100; ++i){
		hSpidar = UsbOpen(i);
		if (hSpidar == INVALID_HANDLE_VALUE) return false;
		return true;
//		if (UsbVidPid(hSpidar) == 0x0CEC0205){	//	SH版アンプのVIDとPID
//			UsbClose(hSpidar);
//			return true;
//		}
	}
	return false;
}
bool DRUsb20Simple::FindDevice(int ch){
	for(int i=0; i < 0x100; ++i){
		hSpidar = UsbOpen(i);
		if (hSpidar == INVALID_HANDLE_VALUE) return false;
	//	if (UsbVidPid(hSpidar) != 0x0CEC0203){	//	USB2.0版アンプのVIDとPID
		if (UsbVidPid(hSpidar) == 0x0CEC0205){	//	SH版アンプのVIDとPID
			UsbClose(hSpidar);
			continue;
		}		
		Reset();
		Update();
		if (ch == RotarySwitch()) return true;
		UsbClose(hSpidar);
	}
	return false;
}

HANDLE DRUsb20Simple::UsbOpen(int id){
	std::ostringstream name;
	name << "\\\\.\\Ezusb-" << id;
	HANDLE rv = CreateFile(name.str().c_str(),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
	return rv;
}
bool DRUsb20Simple::UsbClose(HANDLE& h){
	bool rv = CloseHandle(h) != 0;
	h = NULL;
	return rv;
}

void DRUsb20Simple::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDeviceDa(this, i));
		vpool.Register(new VirtualDeviceCounter(this, i));
		vpool.Register(new VirtualDevicePio(this, i));
	}
}

void DRUsb20Simple::Voltage(int ch, float v){
	assert(0 <= ch && ch < 8);
	const float DigitPerVolt[]={	//	DA指令値/出力電圧 アンプごとに異なるので計測値をアンプに書いておき，読み出す必要がある．
		415.0f,
		415.0f,
		415.0f,
		415.0f,		
		415.0f,
		415.0f,
		415.0f,
		415.0f,
	};
	daOut[ch] = (int)(v * DigitPerVolt[ch]);
}
void DRUsb20Simple::Digit(int ch, int v){
	daOut[ch] = v;
}
void DRUsb20Simple::Count(int ch, long c){
	countOffset[ch] = - (count[ch] - c); 
}
long DRUsb20Simple::Count(int ch){
	return count[ch] + countOffset[ch];
}
void DRUsb20Simple::Pio(int ch, bool level){
	pioLevel[ch] = level ? 1 : 0;
}
bool DRUsb20Simple::Pio(int ch) {
	if(pioLevel[ch]) return true;
	else return false;
}
int DRUsb20Simple::RotarySwitch(){
	int sw=0;
	for(int i=0; i<4; ++i){
		sw |= pioLevel[i+4] << i;
	}
	return sw;
}

void DRUsb20Simple::Update(){
	UsbDaSet();
	UsbCounterGet();
}

void DRUsb20Simple::Reset(){
	UsbReset();
	UsbCounterClear();
}

void DRUsb20Simple::UsbReset(){
	BULK_TRANSFER_CONTROL bulkControl;
	int nBytes = 0;
	int i;
	UCHAR outBuffer[32];
	for(i = 0; i < 32; i++) outBuffer[i] = 0;

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;

	// エンコーダ相順設定
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x1A;
	outBuffer[2] = 0xFF;	//	各チャンネル(各ビットが対応)の正逆転の設定

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

}
void DRUsb20Simple::UsbCounterClear(){
	// エンコーダカウンタクリア
	UCHAR outBuffer[32];
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x19;
	outBuffer[2] = 0xFF;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
}
void DRUsb20Simple::UsbDaSet(){
	//----------------------------------------------------------------------
	// D/A出力
	WORD  outPacketSize;
	int nBytes = 0;

	BULK_TRANSFER_CONTROL bulkControl; //バルク、インタラプト転送用コントロール構造体

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	outPacketSize = 32;

	// DAコンバータセット
	int cur = 0;	//	パケット先頭からの位置
	UCHAR outBufferDA[32];
	memset(outBufferDA, 0, sizeof(outBufferDA));
	outBufferDA[cur++] = 0x52;	//	D/Aセット
	for(int i=0; i < 8; i++){
		outBufferDA[cur++] = daOut[i] & 0xFF;
		outBufferDA[cur++] = daOut[i] >> 8;
	}
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBufferDA[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
}
void DRUsb20Simple::UsbCounterGet(){
	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;
	int nBytes = 0;

	// エンコーダ値のホールドを指令
	UCHAR outBuffer[32];
	outBuffer[0] = 0x51;

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	// デバイス側からホストへ
	// PipeNum:2 type:blk Endpoint:6 IN MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 2;
	WORD inPacketSize = 32;
	UCHAR inBuffer[32];
	int piBuffer[8];
	
	//	エンコーダ値の読み出し
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_READ,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&inBuffer[0],
		inPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	UCHAR Data;
	bool flag;
	for(int i = 0; i < 8; i++){
		piBuffer[i] = 0;
		Data = inBuffer[i * 3 + 2];
		if(Data & 0x80)
			flag = true;
		else
			flag = false;
		piBuffer[i] += Data << 16;
		Data = inBuffer[i * 3 + 1];
		piBuffer[i] += Data << 8;
		Data = inBuffer[i * 3 + 0];
		piBuffer[i] += Data;
		if(flag)
			piBuffer[i] |= 0xFF000000;
	}

	for(int i=0;i<8;i++){
		pioLevel[i] = (inBuffer[24] & (1 << i)) ? 1 : 0;
	}

	for(int i=0;i<8;i++){
		count[i] = piBuffer[i];
	}
}

DWORD DRUsb20Simple::UsbVidPid(HANDLE h){
	USB_DEVICE_DESCRIPTOR desc;
	DWORD nBytes=0;
	if (DeviceIoControl(h,
	       IOCTL_Ezusb_GET_DEVICE_DESCRIPTOR,
	       NULL,
	       0,
	       &desc,
	       sizeof(desc),
	       &nBytes,
		   NULL)){
		return (desc.idVendor << 16) | desc.idProduct;
	}
	return 0;
}

} //namespace Spr

