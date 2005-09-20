#include "IHTmu1Interrupt.h"
#include "iodefine.h"
#include "IHTMUControl.h"
#include <pkgconf/kernel.h>
#include <Base/BaseDebug.h>
#define IHTmu1Interrupt_PRIORITY  5 //priorty of interrupt (1-15)
#define IHTmu1Interrupt_LVL       CYGNUM_HAL_INTERRUPT_TMU1_TUNI1



IHTmu1Interrupt::IHTmu1Interrupt()
:interrupt(IHTmu1Interrupt_LVL, IHTmu1Interrupt_PRIORITY, (CYG_ADDRWORD)this, isr, dsr)
{
	TMU1.TCR.WORD = 0x0020;
	TMU1.TCOR = TMU1.TCNT = 8250; //タイマコンスタントレジスタ, タイマカウンタ
//	TMU.TSTR.BYTE |= 0x02;

	interrupt.attach();
	interrupt.unmask_interrupt(IHTmu1Interrupt_LVL);
}


cyg_uint32 IHTmu1Interrupt::isr(cyg_vector vector, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x2000; //Port6 Up

	CYG_INSTRUMENT_CLOCK(ISR, 0, 0);
	
	TMU1.TCR.WORD &= 0xFEFF;

	Cyg_Interrupt::acknowledge_interrupt(IHTmu1Interrupt_PRIORITY);

	PORT.PDTRA.WORD |= 0x2000;  //Port6 Down
	return Cyg_Interrupt::CALL_DSR|Cyg_Interrupt::HANDLED;
}

void IHTmu1Interrupt::dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x8000; //Port8 Up
	
	DSTR << "T1";
	
	PORT.PDTRA.WORD |= 0x8000;  //Port8 Down
}

