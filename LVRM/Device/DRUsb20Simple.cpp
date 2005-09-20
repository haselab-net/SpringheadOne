// DRUsb20Simple.cpp: DRUsb20Simple クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRUsb20Simple.h"
#include "ezusbsys.h"
#include <winioctl.h>


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace LVRM {

DRUsb20Simple::VirtualDeviceDa::VirtualDeviceDa(DRUsb20Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A Ch %d", realDevice->Name(), ch);
}
DRUsb20Simple::VirtualDeviceCounter::VirtualDeviceCounter(DRUsb20Simple* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s counter Ch %d", realDevice->Name(), ch);
}

DRUsb20Simple::DRUsb20Simple(int ch)
{
	channel = ch;
	sprintf(name, "Cyberse USB2.0 Simple #%d", channel);
	hSpidar = NULL;
	int si[] = {-1, -1, 1, 1, 1, 1, -1, -1};
	for(int i=0; i<8; i++){
		sign[i] = si[i];
		voltage[i] = 0;
		count[i] = 0;
		countOffset[i] = 0;
	}

}

DRUsb20Simple::~DRUsb20Simple()
{

}
bool DRUsb20Simple::Init(){
	/*
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
	*/
	hSpidar = CreateFile("\\\\.\\Ezusb-0",
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
	if (hSpidar == INVALID_HANDLE_VALUE ){
		return false;
	}

	Reset();
	return true;
}
void DRUsb20Simple::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new VirtualDeviceDa(this, i));
		vpool.Register(new VirtualDeviceCounter(this, i));
	}
}

//	int table[8] = {6,8,1,3, 7,5,4,2};
//const int table[8] = {5,7,0,2, 6,4,3,1};
const int table[8] = {0,1,2,3,4,5,6,7};
//const int table[8] = {3,7,6,2,1,5,4,0};
void DRUsb20Simple::Voltage(int ch, float v)
{
	voltage[table[ch]] = v;
}
void DRUsb20Simple::Count(int ch, long c){
	countOffset[table[ch]] = - (count[table[ch]] - c); 
}
long DRUsb20Simple::Count(int ch){
	return count[table[ch]] + countOffset[table[ch]];
}

// 実数スカラー
typedef float Real;


//#define _KATSU_DEBUG
#ifdef _KATSU_DEBUG
int count__ = 0;
DWORD tnow,tstart;//=::timeGetTime();
#endif //_KATSU_DEBUG

void DRUsb20Simple::Update(){
	
#ifdef _KATSU_DEBUG
	if(count__ == 0){
		tstart = ::timeGetTime();
	}
	else if(count__ % 1000 == 0){
		tnow = ::timeGetTime();
		printf("%d - %d = %d\n",tnow, tstart,tnow-tstart);
		tstart = tnow;
	}
	count__++;
#endif //_KATSU_DEBUG

	///////////////////////////////////////////////////////////////////////////
	// D/A出力 TEST-->>
	int Analog[8];

	WORD  outPacketSize;
	UCHAR outBufferDA[32];
	int nBytes = 0;

	BULK_TRANSFER_CONTROL bulkControl; //バルク、インタラプト転送用コントロール構造体

	//float a = 1.0/0.0024; //4.0
	const double HexPerV[]={
		418.077,
		414.313,
		414.557,
		415.830,
		415.315,
		416.100,
		415.707,
		414.557
	};
	int i;

	for(int j=0;j<8;j++){
		Analog[j] = (int)(voltage[j] * HexPerV[j]);
	}
/*
	i = (int)(voltage[1] * HexPerV[1]);
	//i = (int)(voltage[1] * a);
	//if (i < iMinTorque[1]) i = iMinTorque[1];
	//else if (i > iMaxTorque[1]) i = iMaxTorque[1];
	Analog[1] = i;
	i = (int)(voltage[2] * HexPerV[2]);
	//i = (int)(voltage[2] * a);
	//if (i < iMinTorque[2]) i = iMinTorque[2];
	//else if (i > iMaxTorque[2]) i = iMaxTorque[2];
	Analog[2] = i;
	i = (int)(voltage[3] * HexPerV[3]);
	//i = (int)(voltage[3] * a);
	//if (i < iMinTorque[3]) i = iMinTorque[3];
	//else if (i > iMaxTorque[3]) i = iMaxTorque[3];
	Analog[3] = i;
	i = (int)(voltage[4] * HexPerV[0]);
	//i = (int)(voltage[4] * a);
	//if (i < iMinTorque[4]) i = iMinTorque[4];
	//else if (i > iMaxTorque[4]) i = iMaxTorque[4];
	Analog[4] = i;
	i = (int)(voltage[5] * HexPerV[0]);
	//i = (int)(voltage[5] * a);
	//if (i < iMinTorque[5]) i = iMinTorque[5];
	//else if (i > iMaxTorque[5]) i = iMaxTorque[5];
	Analog[5] = i;
	i = (int)(voltage[6] * HexPerV[0]);
	//i = (int)(voltage[6] * a);
	//if (i < iMinTorque[6]) i = iMinTorque[6];
	//else if (i > iMaxTorque[6]) i = iMaxTorque[6];
	Analog[6] = i;
	i = (int)(voltage[7] * HexPerV[0]);
	//i = (int)(voltage[7] * a);
	//if (i < iMinTorque[7]) i = iMinTorque[7];
	//else if (i > iMaxTorque[7]) i = iMaxTorque[7];
	Analog[7] = i;
*/
	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	outPacketSize = 32;

	// DAコンバータセット
	int k = 0;
	outBufferDA[k++] = 0x52;
	for(i = 0; i < 8; i++){
		outBufferDA[k++] = Analog[i] & 0xFF;
		outBufferDA[k++] = Analog[i] >> 8;
	}

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBufferDA[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
	// D/A出力 <<--TEST
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	//	Counter入力  TEST-->>
	WORD  inPacketSize;
	UCHAR inBuffer[32];
	UCHAR outBuffer[32];

	int piBuffer[8];

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	outPacketSize = 32;

	// エンコーダホールド
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
	inPacketSize = 32;

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
	for(i = 0; i < 8; i++){
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
	/*
	int iButton;
	iButton = (inBuffer[24] & 0x7);
	printf("Button:%x\n",iButton);
	*/

	for(i=0;i<8;i++){
		count[i] = (Real)piBuffer[i];
	}
		
	/*
	count[0] = (Real)piBuffer[0];		//7
	count[1] = (Real)piBuffer[1];		//4
	count[2] = (Real)piBuffer[2];		//3
	count[3] = (Real)piBuffer[3];		//0
	count[4] = (Real)piBuffer[4];		//6
	count[5] = (Real)piBuffer[5];		//5
	count[6] = (Real)piBuffer[6];		//2
	count[7] = (Real)piBuffer[7];		//1
	count[0] = (Real)piBuffer[0];		//7
	count[1] = (Real)piBuffer[1];		//4
	count[2] = (Real)piBuffer[2];		//3
	count[3] = (Real)piBuffer[3];		//0
	count[4] = (Real)piBuffer[4];		//6
	count[5] = (Real)piBuffer[5];		//5
	count[6] = (Real)piBuffer[6];		//2
	count[7] = (Real)piBuffer[7];		//1
	*/
	/**/

#ifdef _KATSU_DEBUG
	for(i=0;i<8;i++){
		printf("%d ",count[i]);
	}
	printf("\n");
#endif //_KATSU_DEBUG
	
	//printf("\n");
	//	Counter入力  <<-- TEST
	///////////////////////////////////////////////////////////////////////////
}

void DRUsb20Simple::Reset(){
	int Analog[8];
	UCHAR outBuffer[32];
	BULK_TRANSFER_CONTROL bulkControl;
	WORD  outPacketSize;
	int nBytes = 0;
	int i;
	int k;

	for(i = 0; i < 32; i++){
		outBuffer[i] = 0;
	}

	//EnterCriticalSection(&m_csLockParam);

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	outPacketSize = 32;

	// エンコーダ相順設定
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x1A;
//	outBuffer[2] = 0x5A;
	outBuffer[2] = 0xFF;	//	正転逆転の設定	サイバースはA5にしていた。

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	for(i = 0; i < 32; i++){
		outBuffer[i] = 0;
	}
	// D/Aコンバータデータセット
	k = 0;
	outBuffer[k++] = 0x52;
	for(i = 0; i < 8; i++){
		//Analog[i] = CGripDeviceTorque::iMinTorque[i];
		Analog[i] = 0x10;
		outBuffer[k++] = Analog[i] & 0xFF;
		outBuffer[k++] = Analog[i] >> 8;
	}

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	Sleep(100);

	// エンコーダカウンタクリア
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x19;
	outBuffer[2] = 0xFF;

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	//LeaveCriticalSection(&m_csLockParam);
}


} //namespace LVRM

