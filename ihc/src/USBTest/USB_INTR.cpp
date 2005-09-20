#include "USB_INTR.hpp"
#include "OutPort.h"
#include "regdefs.h"
#include "ihcdevicemanager.hpp"
#include "iodefine.h"
#include "setting.h"
#include <stdio.h>
#include <cyg/kernel/kapi.h>
#include <cyg/hal/sh_stub.h>
#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>
#include <cyg/kernel/thread.hxx>


extern IHCDeviceManager* pdevMan;

USB_INTR::USB_INTR()
:interrupt(USB_INTR_LVL, USB_INTR_PRIORITY, (CYG_ADDRWORD)this, isr, dsr)
{
	interrupt.attach();
	interrupt.unmask_interrupt(USB_INTR_LVL);
}


cyg_uint32 USB_INTR::isr(cyg_vector vector, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x0800; //Port4 Up

	CYG_INSTRUMENT_CLOCK(ISR, 0, 0);

#ifdef _KATSU_TEST
	R_CTL.BIT.IRL0_CLS = 1;	//割り込みのクリア
#else
	R_CTL = 0x400;	//割り込みのクリア
#endif //_KATSU_TEST

	Cyg_Interrupt::acknowledge_interrupt(USB_INTR_PRIORITY);

	PORT.PDTRA.WORD |= 0x0800; //Port4 Down
	return Cyg_Interrupt::CALL_DSR|Cyg_Interrupt::HANDLED;
}


#define USBDATASIZE 256
unsigned short inUSBBuf[USBDATASIZE];
extern cyg_handle_t thread_hdl[NTHREADS];

void UsbThread();
volatile int usbPacketNum;
void USB_INTR::dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data){
	cyg_scheduler_lock();
	usbPacketNum ++;
	cyg_thread_resume(thread_hdl[1]);
	cyg_scheduler_unlock();
}

void UsbThread(){
	while(1){
		PORT.PDTRA.WORD &= ~0x2000; //Port6 Up

		unsigned short *pData;
		int i = 16;

		pData = inUSBBuf;
		for(;i>0;i--){
			*pData++ = R_FIFO_R; // 0
			*pData++ = R_FIFO_R; // 1
			*pData++ = R_FIFO_R; // 2
			*pData++ = R_FIFO_R; // 3

			*pData++ = R_FIFO_R; // 4
			*pData++ = R_FIFO_R; // 5
			*pData++ = R_FIFO_R; // 6
			*pData++ = R_FIFO_R; // 7

			*pData++ = R_FIFO_R; // 8
			*pData++ = R_FIFO_R; // 9
			*pData++ = R_FIFO_R; // a
			*pData++ = R_FIFO_R; // b

			*pData++ = R_FIFO_R; // c
			*pData++ = R_FIFO_R; // d
			*pData++ = R_FIFO_R; // e
			*pData++ = R_FIFO_R; // f
		}

		pdevMan->PacketAnalyzer(inUSBBuf);//パケットアナライザーの起動

		PORT.PDTRA.WORD |= 0x2000; //Port6 Down
		cyg_scheduler_lock();
		usbPacketNum --;
		if (usbPacketNum <= 0){
			cyg_thread_suspend(thread_hdl[1]);
		}
		cyg_scheduler_unlock();
	}
}
