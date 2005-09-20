#include "TMU1_INTR.hpp"
#include "OutPort.h"
#include "iodefine.h"
#include "TMUCtrl.h"


TMU1_INTR::TMU1_INTR()
:interrupt(TMU1_INTR_LVL, TMU1_INTR_PRIORITY, (CYG_ADDRWORD)this, isr, dsr)
{
	TMU1.TCR.WORD = 0x0020;
	TMU1.TCOR = TMU1.TCNT = 8250; //タイマコンスタントレジスタ, タイマカウンタ
//	TMU.TSTR.BYTE |= 0x02;

	interrupt.attach();
	interrupt.unmask_interrupt(TMU1_INTR_LVL);
}


cyg_uint32 TMU1_INTR::isr(cyg_vector vector, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x2000; //Port6 Up

	CYG_INSTRUMENT_CLOCK(ISR, 0, 0);
	
	TMU1.TCR.WORD &= 0xFEFF;

	Cyg_Interrupt::acknowledge_interrupt(TMU1_INTR_PRIORITY);

	PORT.PDTRA.WORD |= 0x2000;  //Port6 Down
	return Cyg_Interrupt::CALL_DSR|Cyg_Interrupt::HANDLED;
}

void TMU1_INTR::dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data){
	PORT.PDTRA.WORD &= ~0x8000; //Port8 Up
	for(int i = 0; i = 10; i++){}
	PORT.PDTRA.WORD |= 0x8000;  //Port8 Down
}

