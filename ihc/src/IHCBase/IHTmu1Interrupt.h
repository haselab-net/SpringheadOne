#ifndef _IHTmu1Interrupt_HPP_
#define _IHTmu1Interrupt_HPP_
#include <pkgconf/kernel.h>
#include <cyg/kernel/intr.hxx>

class IHTmu1Interrupt{
	Cyg_Interrupt    interrupt;

public:
	IHTmu1Interrupt();

	static cyg_uint32 isr(cyg_vector vector, CYG_ADDRWORD data);
	static void       dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
};

#endif //_IHTmu1Interrupt_HPP_



