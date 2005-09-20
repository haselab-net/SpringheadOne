#include "IHUsbInterrupt.h"
#include "IHRegDefs.h"
#include "iodefine.h"
#include <Base/BaseDebug.h>

//	�X���b�h�̗D�揇��
const int USB_PRIORITY = 3;
//	�X���b�h�N���֐�
static void ReceiveThreadFunc(cyg_addrword_t arg){
	((IHUsbInterrupt*)arg)->ReceiveThread();
}
IHUsbInterrupt::IHUsbInterrupt()
	:interrupt(IHUsbInterrupt_LVL, IHUsbInterrupt_PRIORITY, (CYG_ADDRWORD)this, Isr, Dsr){
	nWaitPacket = 0;
	R_CTL = 0x10;		//	Read fifo clear
	//	���荞�݂̐ݒ�
	interrupt.attach();
	interrupt.unmask_interrupt(IHUsbInterrupt_LVL);
	//	�����X���b�h�̍쐬
	cyg_thread_create(USB_PRIORITY, ReceiveThreadFunc, (CYG_ADDRWORD)this,
		"ReceiveThread", (void *) stack, sizeof(stack), &threadHandle, &threadObject);
	R_CTL = 0x10;	//	FIFO�̃N���A
}

cyg_uint32 IHUsbInterrupt::Isr(cyg_vector vector, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x0800; //Port4 Up
	CYG_INSTRUMENT_CLOCK(ISR, 0, 0);
	R_CTL = 0x400;	//���荞�݂̃N���A
	Cyg_Interrupt::acknowledge_interrupt(IHUsbInterrupt_PRIORITY);
	PORT.PDTRA.WORD |= 0x0800; //Port4 Down
	return Cyg_Interrupt::CALL_DSR|Cyg_Interrupt::HANDLED;
}


void IHUsbInterrupt::Dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data){
	cyg_scheduler_lock();
	IHUsbInterrupt* usb = (IHUsbInterrupt*)data;
	usb->nWaitPacket ++;
	cyg_thread_resume(usb->threadHandle);
	cyg_scheduler_unlock();
}

void IHUsbInterrupt::ReceiveThread(){
	while(1){
		PORT.PDTRA.WORD &= ~0x2000;		//Port6 Up
		unsigned short *pData = inUsbBuf;
		//	fifo�̓ǂݏo��
		for(int i=0;i<PACKET_SIZE/2;++i) *pData++ = R_FIFO_R;
		OnReceive((char*)inUsbBuf);			//�p�P�b�g�̉��
		PORT.PDTRA.WORD |= 0x2000;		//Port6 Down
		cyg_scheduler_lock();
		nWaitPacket --;
		if (nWaitPacket <= 0) cyg_thread_suspend(threadHandle);
		cyg_scheduler_unlock();
	}
}
void IHUsbInterrupt::OnReceive(char* packet){
	for(int i=0; i<PACKET_SIZE && packet[i]; ++i){
		//	�p�P�b�g�̕\��
		if (isprint(packet[i])){
			printf("%c", packet[i]);
		}else{
			printf("\%x", (int)packet[i]);
		}
	}
	printf("\n");
	Send(packet);
}
void IHUsbInterrupt::Send(char* out){
	unsigned short *pData = (unsigned short*)out;
	while((R_STS & 0x0200)!=0); //	Waiting for write FIFO's vacancy.
	R_CTL = 0x20; 				//	Start write fifo.
	for(int i=0;i<PACKET_SIZE/2;++i)  R_FIFO_W = *pData++;
}
