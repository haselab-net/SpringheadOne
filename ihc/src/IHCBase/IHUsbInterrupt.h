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
	
	char stack[16*1024];					//	Usb�p�X���b�h�̃X�^�b�N�T�C�Y 
	enum {PACKET_SIZE = 512};				//	Usb�̃p�P�b�g�T�C�Y
	unsigned short inUsbBuf[PACKET_SIZE/2];	//	Usb���̓o�b�t�@
	volatile int nWaitPacket;				//	��M�҂��p�P�b�g��
public:
	IHUsbInterrupt();
	virtual ~IHUsbInterrupt(){}
	//	���荞�݃n���h��
	static cyg_uint32 Isr(cyg_vector vector, CYG_ADDRWORD data);
	//	���荞�ݏ���(USB Thread�̋N��)
	static void Dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
	//	ReceiveThread
	void ReceiveThread();
	//	�p�P�b�g��M���̃R�[���o�b�N
	virtual void OnReceive(char* packet);
	//	�p�P�b�g���M
	virtual void Send(char* packet);
};

#endif //IHUSBINTER_H

