// IHCDeviceManagerBase.cpp: IHCDeviceManagerBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "IHCDeviceManagerBase.hpp"
#include "regdefs.h"
#include "iodefine.h"
#include "TMUCtrl.h"
#include <stdio.h>

#ifndef WIN32
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
#else
#include <stdio.h>
#endif //WIN32


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

IHCDeviceManagerBase::IHCDeviceManagerBase()
{

}

IHCDeviceManagerBase::~IHCDeviceManagerBase()
{

}

void IHCDeviceManagerBase::OpDASet()
{
	//DAコンバータ設定レジスタに値を書き込む -->>
	R_DAC_V0 = hexDA[0];
	R_DAC_V1 = hexDA[1];
	R_DAC_V2 = hexDA[2];
	R_DAC_V3 = hexDA[3];

	R_DAC_V4 = hexDA[4];
	R_DAC_V5 = hexDA[5];
	R_DAC_V6 = hexDA[6];
	R_DAC_V7 = hexDA[7];

#ifdef _KATSU_TEST
	R_CTL.BIT.DA_SET = 1;
#else
	R_CTL = 0x01; //DAセット
#endif //_KATSU_TEST
}


void IHCDeviceManagerBase::OpADGet()
{
	/*	

	TMU0.TCR.BIT.TPSC = 0; // PΦ/4 でカウント (PΦ=33MHz, PΦ/4=8.25MHz)
	TMU0.TCOR = 14;
	TMU0.TCNT = 14; // タイマカウンタ*/
	TMU1.TCR.WORD = 0x0020;
	TMU1.TCNT = TMU1.TCOR = 14;
	int i;
	for(i=0;i<8;i++){
		R_ADC = 4+i*8;
		TMU.TSTR.BYTE |= 0x02;
		while(!(TMU1.TCR.WORD & 0x0100));
		TMU.TSTR.BYTE &= ~0x02;
		TMU1.TCR.WORD &= 0xFEFF;
/*
		TMU.TSTR.BIT.STR0 = 1; //タイマスタート
		// 1.6μs経過後 (1.69μs経過後)
		//while(!(TMU0.TCR.WORD & 0x100));
		while(!TMU0.TCR.BIT.UNF);
		TMU.TSTR.BIT.STR0 = 0; //タイマストップ
		TMU0.TCR.BIT.UNF = 0;
*/
		hexAD[i] = R_ADC & 0xfff;
	}

	////
	//int i=0;
#if 0
	// ch0
	R_ADC = 0x04;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[0] = R_ADC & 0xfff;

	// ch1
	R_ADC = 0x0c;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[1] = R_ADC & 0xfff;

	// ch2
	R_ADC = 0x14;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[2] = R_ADC & 0xfff;

	// ch3
	R_ADC = 0x1c;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[3] = R_ADC & 0xfff;

	// ch4
	R_ADC = 0x24;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[4] = R_ADC & 0xfff;

	// ch5
	R_ADC = 0x2c;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[5] = R_ADC & 0xfff;

	// ch6
	R_ADC = 0x34;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[6] = R_ADC & 0xfff;

	// ch7
	R_ADC = 0x3c;
	TMU.TSTR.BIT.STR0 = 1; //タイマスタート
	while(!TMU0.TCR.BIT.UNF);// 1.6μs経過後 (1.69μs経過後)
	TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	TMU0.TCR.BIT.UNF = 0;
	//*(hexAD+(i++)) = R_ADC & 0xfff;
	hexAD[7] = R_ADC & 0xfff;

	//TMU.TSTR.BIT.STR0 = 0; //タイマストップ
	////
#endif
}

void IHCDeviceManagerBase::OpENCGet()
{
	R_CNT_HLD = 0xFF;// エンコーダホールド
	hexENC[0] = R_CNT_V0;
	hexENC[1] = R_CNT_V1;
	hexENC[2] = R_CNT_V2;
	hexENC[3] = R_CNT_V3;
	hexENC[4] = R_CNT_V4;
	hexENC[5] = R_CNT_V5;
	hexENC[6] = R_CNT_V6;
	hexENC[7] = R_CNT_V7;
	R_CNT_HLD = 0x00;// エンコーダホールド解除
}

void IHCDeviceManagerBase::OpENCPHSSet()
{
	R_CNT_PHS = ENC_PHS;// エンコーダ相順設定
}

void IHCDeviceManagerBase::OpENCCls()
{
	int i;
	for(i=0;i<8;i++){
		hexENC[i] = 0;
	}
#ifdef _KATSU_TEST
	R_CTL.BIT.ALL_ENC_CLS = 1;
#else
	R_CTL = 0x08;
#endif //_KATSU_TEST
}

void IHCDeviceManagerBase::OpRFIFOCls()
{
#ifdef _KATSU_TEST
	R_CTL.BIT.R_FIFO_CLS = 1;
#else
	R_CTL = 0x10;
#endif //_KATSU_TEST
}

void IHCDeviceManagerBase::OpDACls()
{
#ifdef _KATSU_TEST
	R_CTL.BIT.DA_CLS = 1;
#else
	R_CTL = 0x02;
#endif //_KATSU_TEST
}

void IHCDeviceManagerBase::OpSystemReset()
{
#if 0 // リセット命令は危険！！！システム全体をリセットしてしまう！！！
	R_RST = 0x01;
#endif
}

///////////////////////////////////////////////////////////////////////
// 標準出力 (USB Output)
void IHCDeviceManagerBase::OpUSBOutPut(){
#ifndef WIN32
#ifdef USB_IO
	int i;
	unsigned short buf[256];
	char packetIndex[12];
	if(usbBufferSize == 0){
		packetIndex[ 0] = 'o';		//output
		packetIndex[ 1] = 0xff;		//予約
		packetIndex[ 2] = 1;		//ストリーム番号(現在固定)
		packetIndex[ 3] = 0xff;		//予約
		packetIndex[ 4] = i+1;		//現在のパケット番号
		packetIndex[ 5] = 0xff;		//予約
		packetIndex[ 6] = usbBufferSize/USB_PRINT_PACKET_SIZE+1;	//トータルのパケットサイズ
		packetIndex[ 7] = 0xff;		//予約
		packetIndex[ 8] = usbBufferSize/USB_PRINT_PACKET_SIZE-i;	//残りのパケットサイズ
		packetIndex[ 9] = 0xff;		//予約
		packetIndex[10] = usbBufferSize;							//データサイズ
		packetIndex[11] = 0xff;		//予約

		memcpy(buf,packetIndex,12);
		//memcpy(buf+USB_PRINT_INITNUM/2,usbPrintBuffer+i*USB_PRINT_PACKET_SIZE,USB_PRINT_PACKET_SIZE/2);
		//memcpy(buf+6,usbPrintBuffer+i*500,500);
		buf[6] = 0;
		OpSendPC((unsigned short*)buf);
		return;
	}
	for(i=0;i<usbBufferSize/USB_PRINT_PACKET_SIZE+1;i++){
		packetIndex[ 0] = 'o';		//output
		packetIndex[ 1] = 0xff;		//予約
		packetIndex[ 2] = 1;		//ストリーム番号(現在固定)
		packetIndex[ 3] = 0xff;		//予約
		packetIndex[ 4] = i+1;		//現在のパケット番号
		packetIndex[ 5] = 0xff;		//予約
		packetIndex[ 6] = usbBufferSize/USB_PRINT_PACKET_SIZE+1;	//トータルのパケットサイズ
		packetIndex[ 7] = 0xff;		//予約
		packetIndex[ 8] = usbBufferSize/USB_PRINT_PACKET_SIZE-i;	//残りのパケットサイズ
		packetIndex[ 9] = 0xff;		//予約
		packetIndex[10] = usbBufferSize;							//データサイズ
		packetIndex[11] = 0xff;		//予約

		memcpy(buf,packetIndex,12);
		//memcpy(buf+USB_PRINT_INITNUM/2,usbPrintBuffer+i*USB_PRINT_PACKET_SIZE,USB_PRINT_PACKET_SIZE/2);
		memcpy(buf+6,usbPrintBuffer+i*500,500);
		OpSendPC((unsigned short*)buf);
	}
	usbBufferSize = 0;
#endif //USB_IO
#endif //WIN32
}

void IHCDeviceManagerBase::OpSendPC(unsigned short *sendData)
{
#ifdef _KATSU_TEST
	while(R_STS.BIT.W_FIFO_BUZY);
	R_CTL.BIT.W_FIFO_START = 1;
#else
	while((R_STS & 0x0200)!=0); //ライトFIFOビジィのときまつ
	R_CTL = 0x20; //ライトFIFOスタート
#endif //_KATSU_TEST
	int i = 16;
	/*
	for(i=0;i<256;i++){
		R_FIFO_W = sendData[i];
	}
	*/
	for(;i>0;i--){
		R_FIFO_W = *sendData++; // 0
		R_FIFO_W = *sendData++; // 1
		R_FIFO_W = *sendData++; // 2
		R_FIFO_W = *sendData++; // 3

		R_FIFO_W = *sendData++; // 4
		R_FIFO_W = *sendData++; // 5
		R_FIFO_W = *sendData++; // 6
		R_FIFO_W = *sendData++; // 7

		R_FIFO_W = *sendData++; // 8
		R_FIFO_W = *sendData++; // 9
		R_FIFO_W = *sendData++; // a
		R_FIFO_W = *sendData++; // b

		R_FIFO_W = *sendData++; // c
		R_FIFO_W = *sendData++; // d
		R_FIFO_W = *sendData++; // e
		R_FIFO_W = *sendData++; // f
	}
}


/*
//
// TimeMeasureStart
//
// Time Measure start using TMU2
void IHCDeviceManagerBase::TimeMeasureStart(void)
{
	TMU2.TCR.BIT.TPSC = 0;
	TMU2.TCOR = TMU2.TCNT = 0xFFFFFFFF;
	TMU2.TCR.BIT.UNF = 0;

	TMU.TSTR.BIT.STR2 = 1;
}

//
// TimeMeasureEnd
//
// Time Measure end using TMU2
void IHCDeviceManagerBase::TimeMeasureEnd(void)
{
	int count;
	float MHz = 8.25f;
	TMU.TSTR.BIT.STR2 = 0;

	if(TMU2.TCR.BIT.UNF == 1){
		printf("////////////////////////////////\n\r");
		TMU2.TCR.BIT.UNF = 0;
	}
	count = (0xffffffff - TMU2.TCNT);
	printf("time:%f[μs] (total count:%d,%d)\n\r",count/MHz, count,TMU2.TCNT);
}
*/

