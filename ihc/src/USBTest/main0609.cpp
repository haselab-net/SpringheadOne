#include <cyg/kernel/kapi.h>
#include <cyg/hal/sh_stub.h>
#include "setting.h"
#include "spidar.h"
#include "USB_INTR.hpp"
#include "IHCPrinter.h"
#include "IHCSpidar4quad.hpp"
#include "IHCDeviceManager.hpp"
#include "TMUCtrl.h"
#include "OutPort.h"
#include "TMU1_INTR.hpp"
#include "iodefine.h"

#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>

#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
//extern int printf(const char * format, ...);
#ifdef __cplusplus
}
#endif

static cyg_handle_t thread_hdl[NTHREADS+1];
static cyg_thread   thread_obj[NTHREADS+1];
static char         stack[NTHREADS+1][STACKSIZE];

static void alarm_prog (cyg_addrword_t);
static void thread_prog(cyg_addrword_t);

cyg_alarm_t alarm_func;

IHCDeviceManager* pdevMan;
IHCSpidar4quad* pSpidar[SPIDAR_4_SIZE];

#define POINTER

int main(){

	put_register(VBR, 0x08008000);   //set VBR
	HardwareSetup();                 //initialize SCIF
	SetOutPortInit();                //initialize I/O port
	USB_INTR usb_intr;               //for usb interrupt
//	TMU1_INTR tmu1_intr;             //for TMU1 interrupt

	register cyg_uint16 _icr_;
	HAL_READ_UINT16(CYGARC_REG_ICR, _icr_);
	_icr_ |= CYGARC_REG_ICR_IRLM;
	HAL_WRITE_UINT16(CYGARC_REG_ICR, _icr_);


#ifdef POINTER
	pdevMan = new IHCDeviceManager();
#else
	IHCDeviceManager devMan;	     // create Device Manager
	pdevMan = &devMan;			     // set global pointer
#endif //POINTER

	IHCSpidar4quad spidar[SPIDAR_4_SIZE];	// create SPIDAR
	for(int i=0;i<SPIDAR_4_SIZE;i++){
		pSpidar[i] = &spidar[i];			// set global pointer

#ifdef POINTER
		pdevMan->Init(spidar[i]);
#else
		devMan.Init(spidar[i]);				// set device manager
#endif //POINTER
		
	}
//	devMan.PrintStatus();
//	pdevMan->PrintStatus();
	

	/*
	CYG_ADDRESS* pvsr;
	for(int i = 0; i < CYGNUM_HAL_VSR_COUNT; i++){
		HAL_VSR_GET(i, &pvsr);
		printf("pvsr = %d, i = %d\n\r", pvsr, i);
	}
	
	int intr_in_use;
	for(int i = 0; i < CYGNUM_HAL_ISR_COUNT; i++){
		HAL_INTERRUPT_IN_USE(i, intr_in_use);
		printf("intr_in_use(%2d) = %d\n\r", i, intr_in_use);
		printf("hal_interrupt_handlers[%d] = %d\n\r", i, hal_interrupt_handlers[i]);
	}
	*/
	

	//create alarm thread
	cyg_thread_create(15, alarm_prog, (cyg_addrword_t) NTHREADS,
		"alarm_thread", (void *) stack[NTHREADS], STACKSIZE,
		&thread_hdl[NTHREADS], &thread_obj[NTHREADS]);

	//create user defined threads
	for(int i = 0; i < NTHREADS; i++){
		cyg_thread_create(priority[i], thread_prog, (cyg_addrword_t) i, 
			"alarm_thread", (void *) stack[i],
			STACKSIZE, &thread_hdl[i], &thread_obj[i]);
	}

	cyg_thread_resume(thread_hdl[NTHREADS]);  //resume alarm thread

	for(;;){}
	return 0;
}

static void thread_prog(cyg_addrword_t data)
{
	while(1){
		thread_func[(int)data]();
		cyg_thread_suspend(thread_hdl[(int)data]);
	}
}

static void alarm_prog(cyg_addrword_t data)
{
	cyg_handle_t system_counter, system_clock;
	cyg_handle_t alarm_hdl[NTHREADS];
	cyg_alarm    alarm_obj[NTHREADS];
	cyg_tick_count_t ticks;

	system_clock = cyg_real_time_clock();
	if(!system_clock){
		printf("No system clock...\n\r");
		return;
	}
	cyg_clock_to_counter(system_clock, &system_counter);

	//create alarms
	for(int i = 0; i < NTHREADS; i++){
		cyg_alarm_create(system_counter, alarm_func,
			(cyg_addrword_t) i,
			&alarm_hdl[i], &alarm_obj[i]);
		cyg_alarm_initialize(alarm_hdl[i], cyg_current_time()+interval[i], interval[i]);
	}

	for (;;) {
		ticks = cyg_current_time();
		printf("Time is %llu\n\r", ticks);
		cyg_thread_delay(1000);
	}
}

//wake up proper thread at proper time
void alarm_func(cyg_handle_t alarm_hdl, cyg_addrword_t data)
{
	cyg_thread_resume(thread_hdl[(int)data]);
}

