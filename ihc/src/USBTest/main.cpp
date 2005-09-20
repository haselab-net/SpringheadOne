#include <cyg/kernel/kapi.h>
#include <cyg/hal/sh_stub.h>
#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>
#include <cyg/kernel/thread.hxx>

#include <stdio.h>

#include "setting.h"
#include "spidar.h"
#include "USB_INTR.hpp"
#include "IHCSpidar4quad.hpp"
#include "IHCDeviceManager.hpp"
#include "TMUCtrl.h"
#include "OutPort.h"
#include "TMU1_INTR.hpp"

#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif


cyg_handle_t thread_hdl[NTHREADS];
cyg_thread   thread_obj[NTHREADS];
static char  stack[NTHREADS][STACKSIZE];
static void thread_prog(cyg_addrword_t);

cyg_alarm_t alarm_func;

bool isActive[NTHREADS];

IHCDeviceManager* pdevMan;
IHCSpidar4quad* pSpidar[SPIDAR_4_SIZE];

#define PRINTSTATUS

int main(){

	TMUStop(0);
	HardwareSetup();                 //initialize SCIF
	SetOutPortInit();                //initialize I/O port

	USB_INTR  usb_intr;              //usb interrupt

	//use IRL0-3 pins independently
	register cyg_uint16 _icr_;
	HAL_READ_UINT16(CYGARC_REG_ICR, _icr_);
	_icr_ |= CYGARC_REG_ICR_IRLM;
	HAL_WRITE_UINT16(CYGARC_REG_ICR, _icr_);

	for(int i = 0; i < NTHREADS; i++){
		isActive[i] = false;
	}

	IHCDeviceManager devMan;	     // create Device Manager
	pdevMan = &devMan;			     // set global pointer

	IHCSpidar4quad spidar[SPIDAR_4_SIZE];	// create SPIDAR
	for(int i=0;i<SPIDAR_4_SIZE;i++){
		pSpidar[i] = &spidar[i];			// set global pointer

		devMan.Init(spidar[i]);				// set device manager		
	}

	devMan.PrintStatus();

	//create user defined threads
	for(int i = 0; i < NTHREADS; i++){
		cyg_thread_create(priority[i], thread_prog, (cyg_addrword_t) i, 
			"alarm_thread", (void *) stack[i],
			STACKSIZE, &thread_hdl[i], &thread_obj[i]);
	}

	cyg_handle_t system_counter, system_clock;
	cyg_handle_t alarm_hdl[NTHREADS];
	cyg_alarm    alarm_obj[NTHREADS];

	system_clock = cyg_real_time_clock();
	if(!system_clock){
		printf("No system clock...\n\r");
		return -1;
	}
	cyg_clock_to_counter(system_clock, &system_counter);

	//create alarms
	for(int i = 0; i < NALARMS; i++){
		cyg_alarm_create(system_counter, alarm_func,
			(cyg_addrword_t) i,
			&alarm_hdl[i], &alarm_obj[i]);
		cyg_alarm_initialize(alarm_hdl[i], cyg_current_time()+interval[i], interval[i]);
	}

	for(;;){}
	return 0;
}

static void thread_prog(cyg_addrword_t data)
{
	while(1){
		thread_func[(int)data]();
		cyg_scheduler_lock();
		isActive[(int)data] = false;
		cyg_thread_suspend(thread_hdl[(int)data]);
		cyg_scheduler_unlock();
	}
}

//wake up proper thread at proper time
void alarm_func(cyg_handle_t alarm_hdl, cyg_addrword_t data)
{
	if(!isActive[(int)data]){
		cyg_scheduler_lock();
		isActive[(int)data] = true;
		cyg_thread_resume(thread_hdl[(int)data]);
		cyg_scheduler_unlock();
	}
}
