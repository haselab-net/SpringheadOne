#ifndef IHUSBINTER_H
#define IHUSBINTER_H

#include <cyg/kernel/kapi.h>
#include <cyg/kernel/ktypes.h>
#include <cyg/kernel/intr.hxx>
#include <cyg/kernel/thread.hxx>

#define IHUsbInterrupt_PRIORITY  13 //priorty of interrupt (1-15)
#define IHUsbInterrupt_LVL       CYGNUM_HAL_INTERRUPT_LVL2

class IHUsbInterrupt{
protected:
	Cyg_Interrupt interrupt;
	cyg_handle_t threadHandle;
	cyg_thread   threadObject;
	
	char stack[16*1024];					//	Usb用スレッドのスタックサイズ 
	enum {PACKET_SIZE = 512};				//	Usbのパケットサイズ
	unsigned short inUsbBuf[PACKET_SIZE/2];	//	Usb入力バッファ
	volatile int nWaitPacket;				//	受信待ちパケット数
public:
	IHUsbInterrupt();
	virtual ~IHUsbInterrupt(){}
	//	割り込みハンドラ
	static cyg_uint32 Isr(cyg_vector vector, CYG_ADDRWORD data);
	//	割り込み処理(USB Threadの起動)
	static void Dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
	//	ReceiveThread
	void ReceiveThread();
	//	パケット受信時のコールバック
	virtual void OnReceive(char* packet);
	//	パケット送信
	virtual void Send(char* packet);
};

#endif //IHUSBINTER_H

