#ifndef USBTHREAD_HPP
#define USBTHREAD_HPP

class USB_IO{
	Cyg_Interrupt    interrupt;

	static cyg_uint32 isr(cyg_vector vector, CYG_ADDRWORD data);
	static       void dsr(cyg_vector vector, cyg_ucount32 count, CYG_ADDRWORD data);
};

#endif //USBTHREAD_HPP

