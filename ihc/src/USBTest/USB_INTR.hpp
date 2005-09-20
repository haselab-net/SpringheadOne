#ifndef _USB_INTR_HPP_
#define _USB_INTR_HPP_

#include <pkgconf/kernel.h>
#include <cyg/kernel/ktypes.h>
#include <cyg/kernel/intr.hxx>

#define USB_INTR_PRIORITY                         13 //priorty of interrupt (1-15)
#define USB_INTR_LVL       CYGNUM_HAL_INTERRUPT_LVL2

class USB_INTR{
	Cyg_Interrupt    interrupt;

public:
	USB_INTR();

	static cyg_uint32 isr(cyg_vector vector, CYG_ADDRWORD data);
	static void       dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
};

#endif //_USB_INTR_HPP_



