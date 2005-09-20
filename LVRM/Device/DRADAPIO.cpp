#include "DRAdapio.h"
#include <WinBasis/WBDllLoader.h>
#include <stdio.h>
#include <conio.h>
#include <iostream.h>

using namespace std;

namespace LVRM {
static WBDllLoader dllLoader("TUADAPIO.DLL");	//	�O���[�o���ϐ��Ń��[�_�[�����D
	
DRAdapio::DVDA::DVDA(DRAdapio* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s D/A ch %d", realDevice->Name(), ch);
}
DRAdapio::DVAD::DVAD(DRAdapio* r, int c): realDevice(r), ch(c) {
	sprintf(name, "%s A/D ch %d", realDevice->Name(), ch);
}
DRAdapio::DRAdapio(int i){
	id = i;
	sprintf(name, "ADAPIO %d", id);
}
DRAdapio::~DRAdapio(){
	CloseDevice();
}
/// ������
bool DRAdapio::Init(){
	return dllLoader.Load() && Adapio_Device_Open(id)==0;
}
void DRAdapio::Register(DVVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new DVAD(this, i));
	}
	for(int i=0; i<8; i++){
		vpool.Register(new DVDA(this, i));
	}
}
///	�d���o��
void DRAdapio::DAVoltage(int ch, float v){
	int dat = (v / 2.5f) * 0x100;
	if (dat > 0xFF) dat = 0xFF;
	if (dat < 0) dat = 0;
	Adapio_Dac_Out(id, ch, dat);
}
///	�d������
float DRAdapio::ADVoltage(int ch){
	short dat=0;
	Adapio_Adc_SingleSample(id, ch, &dat);
	return dat * (2.5f / 1024);
}
/// �I������
void DRAdapio::CloseDevice(){
	Adapio_Device_Close(id);
}


extern "C" {
//	short __stdcall Adapio_Device_Open(short id)
#define DLLFUNC_RTYPE	short					//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Device_Open		//	�֐���
#define DLLFUNC_STR		"Adapio_Device_Open"	//	�֐���
#define DLLFUNC_ARGDEF	(short id)				//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) void __stdcall Adapio_Device_Close(short id);
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Device_Close		//	�֐���
#define DLLFUNC_STR		"Adapio_Device_Close"	//	�֐���
#define DLLFUNC_ARGDEF	(short id)				//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowDirection(short id, unsigned char dir);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_LowDirection				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_LowDirection"			//	�֐���
#define DLLFUNC_ARGDEF	(short id, unsigned char dir)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dir)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighDirection(short id,unsigned char dir);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_HighDirection			//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_HighDirection"			//	�֐���
#define DLLFUNC_ARGDEF	(short id, unsigned char dir)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dir)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteWrite(short id,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_LowByteWrite				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_LowByteWrite"			//	�֐���
#define DLLFUNC_ARGDEF	(short id,unsigned char dat)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteWrite(short id,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_HighByteWrite			//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_HighByteWrite"			//	�֐���
#define DLLFUNC_ARGDEF	(short id,unsigned char dat)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteRead(short id,unsigned char *dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_LowByteRead				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_LowByteRead"			//	�֐���
#define DLLFUNC_ARGDEF	(short id, unsigned char* dat)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteRead(short id,unsigned char *dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_HighByteRead				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_HighByteRead"			//	�֐���
#define DLLFUNC_ARGDEF	(short id, unsigned char* dat)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_WordWrite(short id,int dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_WordWrite				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_WordWrite"				//	�֐���
#define DLLFUNC_ARGDEF	(short id, int dat)					//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_WordRead(short id,int *dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_WordRead					//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_WordRead"				//	�֐���
#define DLLFUNC_ARGDEF	(short id, int* dat)				//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, dat)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Out(short id,int count );
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_Clock_Out				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_Clock_Out"				//	�֐���
#define DLLFUNC_ARGDEF	(short id, int count)				//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, count)							//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Stop(short id);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Pio_Clock_Stop				//	�֐���
#define DLLFUNC_STR		"Adapio_Pio_Clock_Stop"				//	�֐���
#define DLLFUNC_ARGDEF	(short id)							//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id)								//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Dac_Out(short id,short ch,unsigned char dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Dac_Out						//	�֐���
#define DLLFUNC_STR		"Adapio_Dac_Out"					//	�֐���
#define DLLFUNC_ARGDEF	(short id, short ch, unsigned char dat)	//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, ch, dat)						//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_SingleSample(short id,short ch,short *dat);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Adc_SingleSample				//	�֐���
#define DLLFUNC_STR		"Adapio_Adc_SingleSample"			//	�֐���
#define DLLFUNC_ARGDEF	(short id, short ch, short *dat)	//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, ch, dat)						//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_DigitalTrg(short id,short EndCh,short BufSize);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Adc_DigitalTrg				//	�֐���
#define DLLFUNC_STR		"Adapio_Adc_DigitalTrg"				//	�֐���
#define DLLFUNC_ARGDEF	(short id,short EndCh,short BufSize)//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(id, EndCh, BufSize)				//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_AnalogTrg(short id,short EndCh,short BufSize, short Threshold,short TrgCh ,short UpOrDown);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Adc_AnalogTrg				//	�֐���
#define DLLFUNC_STR		"Adapio_Adc_AnalogTrg"				//	�֐���
#define DLLFUNC_ARGDEF	(short id,short EndCh,short BufSize, short Threshold,short TrgCh ,short UpOrDown)
#define DLLFUNC_ARGCALL	(id, EndCh, BufSize, Threshold, TrgCh, UpOrDown)
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_GetStatus(short id,short *Running, short *SampledNum);
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall							//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Adc_GetStatus
#define DLLFUNC_STR		"Adapio_Adc_GetStatus"
#define DLLFUNC_ARGDEF	(short id,short *Running, short *SampledNum)
#define DLLFUNC_ARGCALL	(id, Running, SampledNum)
#include <WinBasis/WBDllLoaderImpl.h>

//	__declspec(dllexport) short __stdcall Adapio_Adc_GetDatas(short id,short *databuf, short Len );
#define DLLFUNC_RTYPE	short								//	�Ԃ�^
#define DLLFUNC_CTYPE	__stdcall				//	�Ăяo���^
#define DLLFUNC_NAME	Adapio_Adc_GetDatas
#define DLLFUNC_STR		"Adapio_Adc_GetDatas"
#define DLLFUNC_ARGDEF	(short id,short *databuf, short Len )
#define DLLFUNC_ARGCALL	(id,databuf, Len)
#include <WinBasis/WBDllLoaderImpl.h>
}	//	extern "C"
}	//	namespace LVRM
