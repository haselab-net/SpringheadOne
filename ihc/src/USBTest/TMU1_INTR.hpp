#ifndef _TMU1_INTR_HPP_
#define _TMU1_INTR_HPP_

#include <pkgconf/kernel.h>
#include <cyg/kernel/ktypes.h>
#include <cyg/kernel/intr.hxx>

#define TMU1_INTR_PRIORITY                                5 //priorty of interrupt (1-15)
#define TMU1_INTR_LVL       CYGNUM_HAL_INTERRUPT_TMU1_TUNI1

class TMU1_INTR{
	Cyg_Interrupt    interrupt;

public:
	TMU1_INTR();

	static cyg_uint32 isr(cyg_vector vector, CYG_ADDRWORD data);
	static void       dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
};

#endif //_TMU1_INTR_HPP_



